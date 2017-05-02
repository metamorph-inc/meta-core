from __future__ import absolute_import
import sys
import csv
import uuid
import six
from collections import OrderedDict

import numpy

from openmdao.recorders.base_recorder import BaseRecorder
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
            return str(val)
        self.writer.writerow([munge(params[param_name]) for param_name in self.param_names] + [munge(unknowns[unknown_name]) for unknown_name in self.unknown_names])

        if self.out:
            self.out.flush()

    def record_metadata(self, group):
        pass
        # TODO: what to do here?
        # self.writer.writerow([param.name for param in group.params] + [unknown.name for unknowns in group.unknowns])


class MappingCsvRecorder(BaseRecorder):

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
                id = ['_id']
            self.writer.writerow(id + list(self.params_map.values()) + list(self.unknowns_map.values()))
            self._wrote_header = True

        def munge(val):
            if isinstance(val, numpy.ndarray):
                return str(val.tolist())
            return str(val)

        def do_mapping(map_, values):
            return [munge(values[key]) for key in map_]
        # import pdb; pdb.set_trace()
        id = []
        if self._include_id:
            id = [uuid.uuid4()]
        self.writer.writerow(id + do_mapping(self.params_map, params) + do_mapping(self.unknowns_map, unknowns))

        if self.out:
            self.out.flush()

    def record_metadata(self, group):
        pass
        # TODO: what to do here?
        # self.writer.writerow([param.name for param in group.params] + [unknown.name for unknowns in group.unknowns])
