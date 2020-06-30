from __future__ import absolute_import
import threading
import json
import os.path
import csv
import shutil
import io

from openmdao.util.array_util import evenly_distrib_idxs

from openmdao.core.mpi_wrap import MPI
import six
from six.moves import range


class RestartRecorder(object):
    RESTART_PROGRESS_FILENAME = 'restart_progress'
    RESTART_RUNLIST_FILENAME = 'restart_runlist'
    # FIXME is there a good way to pick these
    ITERATION_DONE_FLAG = 2415
    DOE_DONE_FLAG = 2416

    @classmethod
    def is_restartable(cls, original_dir):
        return os.path.isfile(os.path.join(original_dir, RestartRecorder.RESTART_PROGRESS_FILENAME)) and \
            os.path.isfile(os.path.join(original_dir, RestartRecorder.RESTART_RUNLIST_FILENAME))

    @classmethod
    def deserialize_runlist(cls, original_dir):
        # FIXME check mdao_config.json mtime and against these
        if cls.is_restartable(original_dir):
            with open(os.path.join(original_dir, RestartRecorder.RESTART_PROGRESS_FILENAME)) as restart_progress_file:
                restart_progress = json.load(restart_progress_file)
            runlist = []
            with open(os.path.join(original_dir, RestartRecorder.RESTART_RUNLIST_FILENAME), 'r') as restart_runlist:
                len_runlist = int(restart_runlist.readline())
                run_sizes, run_offsets = evenly_distrib_idxs(len(restart_progress), len_runlist)
                already_done_ranges = [(offset, offset + restart_progress[str(i)]) for i, offset in enumerate(run_offsets)] + [(float("inf"), float("inf"))]
                reader = csv.reader(restart_runlist)
                header = next(iter(reader))
                for i, run in enumerate(reader):
                    while already_done_ranges[1][0] <= i:
                        already_done_ranges = already_done_ranges[1:]
                    if not already_done_ranges[0][0] <= i < already_done_ranges[0][1]:
                        runlist.append([(field, run[j]) for j, field in enumerate(header)])
            return runlist

    @classmethod
    def serialize_runlist(cls, original_dir, runlist, comm_size):
        output_filename = os.path.join(original_dir, RestartRecorder.RESTART_RUNLIST_FILENAME)
        if six.PY2:
            output_file = open(output_filename, 'wb')
        else:
            output_file = io.open(output_filename, 'w', newline='')
        with output_file as restart_runlist:
            restart_runlist.write(str(len(runlist)) + '\n')
            writer = csv.writer(restart_runlist)
            header = list(p[0] for p in runlist[0])
            writer.writerow(header)
            for run in runlist:
                writer.writerow([p[1] for p in run])
        with open(os.path.join(original_dir, RestartRecorder.RESTART_PROGRESS_FILENAME), 'w') as restart_progress_file:
            restart_progress_file.write(json.dumps({i: 0 for i in range(comm_size)}))

    def __init__(self, original_dir, comm):
        super(RestartRecorder, self).__init__()

        self._parallel = False
        self.comm = None
        self.thread = None
        self.original_dir = original_dir
        self.output_filename = os.path.join(original_dir, RestartRecorder.RESTART_PROGRESS_FILENAME)
        self.progress_by_rank = None
        self.output_lock = threading.Lock()

        if MPI:
            self.comm = comm
            if self.comm.rank == 0:
                self.progress_by_rank = {i: 0 for i in range(self.comm.size)}

                def recv():
                    while True:
                        rank = self.comm.recv(source=MPI.ANY_SOURCE, tag=RestartRecorder.ITERATION_DONE_FLAG)
                        if rank is None:
                            return
                        self._serialize_progress(rank)
                self.thread = threading.Thread(target=recv)
                self.thread.daemon = True
                self.thread.start()
        else:
            self.progress_by_rank = {0: 0}

    def _serialize_progress(self, rank):
        self.output_lock.acquire()
        try:
            self.progress_by_rank[rank] = self.progress_by_rank[rank] + 1
            tmp_output = self.output_filename + '.tmp'
            with open(tmp_output, 'w') as output:
                json.dump(self.progress_by_rank, output)
            shutil.move(tmp_output, self.output_filename)
        finally:
            self.output_lock.release()

    def record_iteration(self):
        if self.comm:
            if self.comm.rank != 0:
                self.comm.send(self.comm.rank, dest=0, tag=RestartRecorder.ITERATION_DONE_FLAG)
            else:
                self._serialize_progress(0)
        else:
            self._serialize_progress(0)

    def close(self):
        if self.comm:
            if self.comm.rank == 0:
                for i in range(self.comm.size - 1):
                    self.comm.recv(source=MPI.ANY_SOURCE, tag=RestartRecorder.DOE_DONE_FLAG)
                self.comm.send(None, dest=0, tag=RestartRecorder.ITERATION_DONE_FLAG)
                os.unlink(self.output_filename)
                os.unlink(os.path.join(self.original_dir, RestartRecorder.RESTART_RUNLIST_FILENAME))
            else:
                self.comm.send('Done', dest=0, tag=RestartRecorder.DOE_DONE_FLAG)
        else:
            os.unlink(self.output_filename)
            os.unlink(os.path.join(self.original_dir, RestartRecorder.RESTART_RUNLIST_FILENAME))
