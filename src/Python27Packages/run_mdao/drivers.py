from __future__ import print_function
from __future__ import absolute_import
import numpy
import itertools
import traceback
import os
import os.path
import errno

import run_mdao
from run_mdao.restart_recorder import RestartRecorder

import testbenchexecutor.progress_service

from openmdao.util.array_util import evenly_distrib_idxs
from openmdao.util.record_util import create_local_meta, update_local_meta

from openmdao.core.mpi_wrap import MPI, debug
import openmdao.api

# from random import shuffle, randint
import numpy as np
from six import itervalues, iteritems
from six.moves import range
from six.moves import zip

try:
    AnalysisError = openmdao.api.AnalysisError
except AttributeError:
    # older version of openmdao
    class AnalysisError(Exception):
        pass


def _get_seed_and_random(seed):
    if seed is None:
        seed = numpy.random.randint(0, 2**31 - 1) + numpy.random.randint(0, 2**31 - 1)
    return seed, numpy.random.RandomState(seed)


class PredeterminedRunsDriver(openmdao.api.PredeterminedRunsDriver):

    def __init__(self, original_dir, num_samples=None, *args, **kwargs):
        if type(self) == PredeterminedRunsDriver:
            raise Exception('PredeterminedRunsDriver is an abstract class')
        if MPI:
            comm = MPI.COMM_WORLD
            kwargs.setdefault('num_par_doe', comm.Get_size())
        else:
            kwargs.setdefault('num_par_doe', 1)
        super(PredeterminedRunsDriver, self).__init__(*args, **kwargs)
        self.supports['gradients'] = False
        self.original_dir = original_dir

        # Make sure self.original_dir exists (if this was instantiated from a subproblem, it might not)
        try:
            os.makedirs(self.original_dir)
        except OSError as e:
            if e.errno == errno.EEXIST and os.path.isdir(self.original_dir):
                pass
            else:
                raise

        self.use_restart = True

    def _setup_communicators(self, comm, parent_dir):
        super(PredeterminedRunsDriver, self)._setup_communicators(comm, parent_dir)
        if self.use_restart:
            self.restart = RestartRecorder(self.original_dir, comm)

    def run(self, problem):
        """Build a runlist and execute the Problem for each set of generated parameters."""
        self.iter_count = 0

        if MPI and self._num_par_doe > 1:
            runlist = self._distrib_build_runlist()
        else:
            runlist = self._deserialize_or_create_runlist()

        testbenchexecutor.progress_service.update_progress("Iteration 0/{} completed".format(len(runlist)), 0, len(runlist))

        # For each runlist entry, run the system and record the results
        for run in runlist:
            run_success = self.run_one(problem, run)
            testbenchexecutor.progress_service.update_progress(
                "Iteration {}/{} {}".format(
                    self.iter_count, 
                    len(runlist),
                    "completed" if run_success else "failed"
                ), 
                self.iter_count, 
                len(runlist)
            )

        if self.use_restart:
            self.restart.close()

    def run_one(self, problem, run):
        run_success = True

        for dv_name, dv_val in run:
            self.set_desvar(dv_name, dv_val)

        metadata = create_local_meta(None, 'Driver')

        update_local_meta(metadata, (self.iter_count,))

        try:
            problem.root.solve_nonlinear(metadata=metadata)
        except AnalysisError:
            metadata['msg'] = traceback.format_exc()
            metadata['success'] = 0
            run_success = False
        self.recorders.record_iteration(problem.root, metadata)
        self.iter_count += 1
        if self.use_restart:
            self.restart.record_iteration()

        return run_success

    def _distrib_build_runlist(self):
        """
        Returns an iterator over only those cases meant to execute
        in the current rank as part of a parallel DOE. A latin hypercube,
        unlike some other DOE generators, is created in one rank and then
        the appropriate cases are scattered to the appropriate ranks.
        """
        comm = self._full_comm
        job_list = None
        if comm.rank == 0:
            debug('Parallel DOE using %d procs' % self._num_par_doe)
            run_list = [list(case) for case in self._deserialize_or_create_runlist()]  # need to run iterator

            run_sizes, run_offsets = evenly_distrib_idxs(self._num_par_doe,
                                                         len(run_list))
            job_list = [run_list[o:o + s] for o, s in zip(run_offsets,
                                                          run_sizes)]

        run_list = comm.scatter(job_list, root=0)
        debug('Number of DOE jobs: %s' % len(run_list))

        for case in run_list:
            yield case

    def _deserialize_or_create_runlist(self):
        runlist = None
        if self.use_restart:
            runlist = RestartRecorder.deserialize_runlist(self.original_dir)
        if not runlist:
            runlist = [list(run) for run in self._build_runlist()]
        if self.use_restart:
            RestartRecorder.serialize_runlist(self.original_dir, runlist, self._num_par_doe)
        return runlist


class FullFactorialDriver(PredeterminedRunsDriver):
    def __init__(self, num_levels=1, *args, **kwargs):
        super(FullFactorialDriver, self).__init__(*args, **kwargs)
        self.num_levels = num_levels

    def _build_runlist(self):
        # Set up Uniform distribution arrays
        value_arrays = dict()
        for name, value in iteritems(self.get_desvar_metadata()):
            if value.get('type', 'double') == 'double':
                low = value['lower']
                high = value['upper']
                if low == high:
                    value_arrays[name] = [low]
                else:
                    value_arrays[name] = numpy.linspace(low, high, num=self.num_levels).tolist()
            elif value.get('type') == 'enum':
                value_arrays[name] = list(value['items'])
            elif value.get('type') == 'int':
                # TODO limit len to num_levels?
                value_arrays[name] = list(range(int(value['lower']), int(value['upper']) + 1))
        # log["arrays"] = value_arrays

        keys = list(value_arrays.keys())
        for combination in itertools.product(*value_arrays.values()):
            yield zip(keys, combination)


class UniformDriver(PredeterminedRunsDriver):
    def __init__(self, num_samples=1, seed=None, *args, **kwargs):
        super(UniformDriver, self).__init__(*args, **kwargs)
        self.num_samples = num_samples
        self.seed, self.random = _get_seed_and_random(seed)

    def _build_runlist(self):
        def sample_var(metadata):
            if metadata.get('type', 'double') == 'double':
                if metadata['lower'] == metadata['upper']:
                    return metadata['lower']
                return self.random.uniform(metadata['lower'], metadata['upper'])
            elif metadata.get('type') == 'enum':
                return self.random.choice(metadata['items'])
            elif metadata.get('type') == 'int':
                return self.random.randint(metadata['lower'], metadata['upper'] + 1)

        for i in range(self.num_samples):
            yield ((key, sample_var(metadata)) for key, metadata in iteritems(self.get_desvar_metadata()))


class LatinHypercubeDriver(PredeterminedRunsDriver):
    def __init__(self, num_samples=1, seed=None, *args, **kwargs):
        super(LatinHypercubeDriver, self).__init__(*args, **kwargs)
        self.num_samples = num_samples
        self.seed, self.random = _get_seed_and_random(seed)

    def _build_runlist(self):
        design_vars = self.get_desvar_metadata()
        design_vars_names = list(design_vars)
        buckets = dict()
        for design_var_name in design_vars_names:
            metadata = design_vars[design_var_name]
            if metadata.get('type', 'double') == 'double':
                bucket_walls = numpy.linspace(metadata['lower'], metadata['upper'], num=self.num_samples + 1)
                buckets[design_var_name] = [self.random.uniform(low, high) for low, high in zip(bucket_walls[0:-1], bucket_walls[1:])]
            elif metadata.get('type') == 'enum':
                # length is generated such that all items have an equal chance of appearing when num_samples % len(items) != 0
                length = self.num_samples + (-self.num_samples % len(metadata['items']))
                buckets[design_var_name] = list(itertools.islice(itertools.cycle(metadata['items']), length))
            elif metadata.get('type') == 'int':
                # FIXME: should do buckets instead
                num_items = int(metadata['upper'] - metadata['lower'] + 1)
                length = self.num_samples + (-self.num_samples % num_items)
                buckets[design_var_name] = list(itertools.islice(itertools.cycle(range(int(metadata['lower']), int(metadata['upper'] + 1))), length))

            self.random.shuffle(buckets[design_var_name])

        for i in range(self.num_samples):
            yield ((key, values[i]) for key, values in iteritems(buckets))


class OptimizedLatinHypercubeDriver(PredeterminedRunsDriver):
    """Design-of-experiments Driver implementing the Morris-Mitchell method for an Optimized Latin Hypercube.
    """

    def __init__(self, num_samples, seed=None, population=20, generations=2, norm_method=1, *args, **kwargs):
        super(OptimizedLatinHypercubeDriver, self).__init__(*args, **kwargs)
        self.qs = [1, 2, 5, 10, 20, 50, 100]  # List of qs to try for Phi_q optimization
        self.num_samples = num_samples
        if num_samples < 2:
            raise ValueError('num_samples must be >= 2')
        self.seed = seed
        self.population = population
        self.generations = generations
        self.norm_method = norm_method
        self.seed, self.random = _get_seed_and_random(self.seed)

    def _build_runlist(self):
        """Build a runlist based on the Latin Hypercube method."""
        design_vars = self.get_desvar_metadata()
        design_vars_names = list(design_vars)
        self.num_design_vars = len(design_vars_names)

        # Generate an LHC of the proper size
        rand_lhc = self._get_lhc()

        enums = {}
        for design_var_name, metadata in iteritems(design_vars):
            if metadata.get('type', 'double') == 'enum':
                # length is generated such that all items have an equal chance of appearing when num_samples % len(items) != 0
                length = self.num_samples + (-self.num_samples % len(metadata['items']))
                values = list(itertools.islice(itertools.cycle(metadata['items']), length))
                self.random.shuffle(values)
                enums[design_var_name] = values
            elif metadata.get('type', 'double') == 'int':
                low, high = int(metadata['lower']), int(metadata['upper'])
                values = list(range(low, high + 1))
                self.random.shuffle(values)
                enums[design_var_name] = values

        # Return random values in given buckets
        for i in range(self.num_samples):
            def get_random_in_bucket(design_var, bucket):
                metadata = design_vars[design_var]
                if metadata.get('type') == 'enum':
                    return enums[design_var][bucket]
                elif metadata.get('type', 'double') == 'double':
                    low, high = metadata['lower'], metadata['upper']
                    bucket_size = (high - low) / self.num_samples
                    return self.random.uniform(low + bucket_size * bucket, low + bucket_size * (bucket + 1))
                elif metadata.get('type') == 'int':
                    low, high = int(metadata['lower']), int(metadata['upper'])
                    num_items = int(metadata['upper'] - metadata['lower'] + 1)

                    if self.num_samples <= num_items:
                        # FIXME do we need to round max up sometimes
                        return self.random.randint(low + (num_items * bucket // self.num_samples), low + (num_items * (bucket + 1) // self.num_samples))
                    else:
                        if bucket < self.num_samples - (self.num_samples % num_items):
                            return low + bucket % num_items
                        else:
                            return enums[design_var][bucket % num_items]

            yield ((design_var, get_random_in_bucket(design_var, rand_lhc[i, j])) for j, design_var in enumerate(design_vars_names))

    def _get_lhc(self):
        """Generate an Optimized Latin Hypercube
        """

        rand_lhc = _rand_latin_hypercube(self.random, self.num_samples, self.num_design_vars)

        # Optimize our LHC before returning it
        best_lhc = _LHC_Individual(rand_lhc, self.random, q=1, p=self.norm_method)
        for q in self.qs:
            lhc_start = _LHC_Individual(rand_lhc, self.random, q, self.norm_method)
            lhc_opt = _mmlhs(lhc_start, self.population, self.generations)
            if lhc_opt.mmphi() < best_lhc.mmphi():
                best_lhc = lhc_opt

        return best_lhc._get_doe().astype(int)


class _LHC_Individual(object):
    def __init__(self, doe, random, q=2, p=1):
        self.q = q
        self.p = p
        self.doe = doe
        self.phi = None  # Morris-Mitchell sampling criterion
        self.random = random
        random.randint

    @property
    def shape(self):
        """Size of the LatinHypercube DOE (rows,cols)."""

        return self.doe.shape

    def mmphi(self):
        """Returns the Morris-Mitchell sampling criterion for this Latin hypercube."""

        if self.phi is None:
            n, m = self.doe.shape
            distdict = {}

            # Calculate the norm between each pair of points in the DOE
            arr = self.doe
            for i in range(1, n):
                nrm = np.linalg.norm(arr[i] - arr[:i], ord=self.p, axis=1)
                for j in range(0, i):
                    nrmj = nrm[j]
                    if nrmj in distdict:
                        distdict[nrmj] += 1
                    else:
                        distdict[nrmj] = 1

            distinct_d = np.array(list(distdict))

            # Mutltiplicity array with a count of how many pairs of points have a given distance
            J = np.array(list(itervalues(distdict)))

            self.phi = sum(J * (distinct_d ** (-self.q))) ** (1.0 / self.q)

        return self.phi

    def perturb(self, mutation_count):
        """ Interchanges pairs of randomly chosen elements within randomly chosen
        columns of a DOE a number of times. The result of this operation will also
        be a Latin hypercube.
        """

        def randint(low, high):
            if low == high:
                return low
            return self.random.randint(low, high)

        new_doe = self.doe.copy()
        n, k = self.doe.shape
        for count in range(mutation_count):
            col = randint(0, k - 1)

            # Choosing two distinct random points
            el1 = randint(0, n - 1)
            el2 = randint(0, n - 2)
            if el1 == el2:
                el2 = el2 + 1

            new_doe[el1, col] = self.doe[el2, col]
            new_doe[el2, col] = self.doe[el1, col]

        return _LHC_Individual(new_doe, self.random, self.q, self.p)

    def __iter__(self):
        return self._get_rows()

    def _get_rows(self):
        for row in self.doe:
            yield row

    def __repr__(self):
        return repr(self.doe)

    def __str__(self):
        return str(self.doe)

    def __getitem__(self, *args):
        return self.doe.__getitem__(*args)

    def _get_doe(self):
        return self.doe


def _rand_latin_hypercube(random, n, k):
    # Calculates a random Latin hypercube set of n points in k dimensions within [0,n-1]^k hypercube.
    arr = np.zeros((n, k))
    row = list(range(0, n))
    for i in range(k):
        random.shuffle(row)
        arr[:, i] = row
    return arr


def _is_latin_hypercube(lh):
    """Returns True if the given array is a Latin hypercube.
    The given array is assumed to be a numpy array.
    """

    n, k = lh.shape
    for j in range(k):
        col = lh[:, j]
        colset = set(col)
        if len(colset) < len(col):
            return False  # something was duplicated
    return True


def _mmlhs(x_start, population, generations):
    """Evolutionary search for most space filling Latin-Hypercube.
    Returns a new LatinHypercube instance with an optimized set of points.
    """

    x_best = x_start
    phi_best = x_start.mmphi()
    n = x_start.shape[1]

    level_off = np.floor(0.85 * generations)
    for it in range(generations):
        if it < level_off and level_off > 1.:
            mutations = int(round(1 + (0.5 * n - 1) * (level_off - it) / (level_off - 1)))
        else:
            mutations = 1

        x_improved = x_best
        phi_improved = phi_best

        for offspring in range(population):
            x_try = x_best.perturb(mutations)
            phi_try = x_try.mmphi()

            if phi_try < phi_improved:
                x_improved = x_try
                phi_improved = phi_try

        if phi_improved < phi_best:
            phi_best = phi_improved
            x_best = x_improved

    return x_best


class CsvDriver(PredeterminedRunsDriver):
    def __init__(self, original_dir, filename, *args, **kwargs):
        super(CsvDriver, self).__init__(original_dir=original_dir, *args, **kwargs)
        self.use_restart = False
        self.filename = filename

    def _deserialize_or_create_runlist(self):
        runlist = []
        with open(self.filename, 'rU') as csv_input:
            import csv
            reader = csv.reader(csv_input)
            header = next(iter(reader))
            indices = [i for i in range(len(header)) if run_mdao.get_desvar_path(header[i]) in self._desvars.keys()]

            def filter_desvar(l):
                """Filter out non-desvars (in case the .csv has other values)."""
                return (l[i] for i in indices)
            for values in reader:
                runlist.append(zip(map(run_mdao.get_desvar_path, filter_desvar(header)), filter_desvar(values)))
            return runlist
