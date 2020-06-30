from __future__ import absolute_import
import os
import os.path
import shutil
import errno
import sys
import csv
import uuid
import six
from collections import OrderedDict
import pprint

import numpy

from openmdao.recorders.base_recorder import BaseRecorder
from openmdao.api import FileRef
from six.moves import map


class CsvRecorder(BaseRecorder):

    def __init__(self, out=sys.stdout):
        super(CsvRecorder, self).__init__()

        self._wrote_header = False
        self._parallel = False

        if out != sys.stdout:
            self.out = out
        self.writer = csv.writer(out)

    def startup(self, group):
        super(CsvRecorder, self).startup(group)

    def record_iteration(self, params, unknowns, resids, metadata):
        if self._wrote_header is False:
            self.param_names = [param for param in params]
            self.unknown_names = [unknown for unknown in unknowns]
            self.writer.writerow(self.param_names + self.unknown_names)
            self._wrote_header = True

        def munge(val):
            if isinstance(val, numpy.ndarray):
                return str(val.tolist())
            if isinstance(val, FileRef):
                return None
            if isinstance(val, six.text_type):
                return val
            if isinstance(val, float):
                return repr(val)
            return str(val)
        self.writer.writerow([munge(params[param_name]) for param_name in self.param_names] + [munge(unknowns[unknown_name]) for unknown_name in self.unknown_names])

        if self.out:
            self.out.flush()

    def record_metadata(self, group):
        pass
        # TODO: what to do here?
        # self.writer.writerow([param.name for param in group.params] + [unknown.name for unknowns in group.unknowns])


class MappingCsvRecorder(BaseRecorder):
    artifacts_directory = 'artifacts'

    def __init__(self, params_map, unknowns_map, out=sys.stdout, include_id=False):
        super(MappingCsvRecorder, self).__init__()

        self._wrote_header = False
        self._parallel = False
        self._include_id = include_id

        self.params_map = OrderedDict(((k, v) for k, v in sorted(six.iteritems(params_map))))
        self.unknowns_map = OrderedDict(((k, v) for k, v in sorted(six.iteritems(unknowns_map))))

        if out != sys.stdout:
            self.out = out
        self.writer = csv.writer(out)

    def startup(self, group):
        super(MappingCsvRecorder, self).startup(group)

    def record_iteration(self, params, unknowns, resids, metadata):
        if self._wrote_header is False:
            id = []
            if self._include_id:
                id = [u'GUID']
            self.writer.writerow([six.text_type(s) for s in id + ["AnalysisError"] + list(self.params_map.values()) +
                [h for k, v in six.iteritems(self.unknowns_map) if (isinstance(unknowns[k], FileRef) is False) for h in v]])
            self._wrote_header = True

        id = []
        if self._include_id:
            id = [str(uuid.uuid4())]

        def munge(val):
            if isinstance(val, numpy.ndarray):
                return str(val.tolist())
            if isinstance(val, FileRef):
                return None
            if isinstance(val, six.text_type):
                return val
            if isinstance(val, float):
                return repr(val)
            return str(val)

        def do_mapping(map_, values):
            return [v for v in (munge(values[key]) for key in map_) if v is not None]

        def do_multimapping(map_, values):
            return [v for v in (munge(values[key]) for key, v in six.iteritems(map_) for _ in v) if v is not None]

        analysisErrorOccurred = False
        if metadata["success"] == 0:
            analysisErrorOccurred = True

        row = id + [analysisErrorOccurred] + do_mapping(self.params_map, params) + do_multimapping(self.unknowns_map, unknowns)
        self.writer.writerow(row)

        if self._include_id:
            # write FileRefs to artifacts/GUID/mapped_name
            for name, fileref in ((n, fr) for n, fr in six.iteritems(dict(unknowns)) if isinstance(fr, FileRef)):
                mapped_names = self.unknowns_map.get(name, [])
                for mapped_name in mapped_names:
                    try:
                        with fileref.open('rb') as artifact:
                            directory = os.path.join(self.artifacts_directory, six.text_type(id[0]))
                            try:
                                os.makedirs(directory)
                            except OSError as e:
                                if e.errno == errno.EEXIST and os.path.isdir(directory):
                                    pass
                                else:
                                    raise
                            with open(os.path.join(directory, mapped_name), 'wb') as out:
                                shutil.copyfileobj(artifact, out)
                    except IOError as e:
                        if e.errno == errno.ENOENT:
                            if not analysisErrorOccurred:
                                print("WARNING: Driver Objective {} is connected to ".format(mapped_name)
                                      + "{}, but MappingCsvRecorder did not find a file.".format(name)
                                      + "\n         {}".format(str(e).replace("[Errno 2] ", "")))
                        else:
                            raise

        if self.out:
            self.out.flush()

    def record_metadata(self, group):
        pass
        # TODO: what to do here?
        # self.writer.writerow([param.name for param in group.params] + [unknown.name for unknowns in group.unknowns])
