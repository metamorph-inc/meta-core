from __future__ import absolute_import
import hashlib
import numpy


class array_hashable(object):
    def __init__(self, wrapped):
        self.__wrapped = wrapped  # TODO safer to deep copy
        self.__hash = int(hashlib.md5(wrapped.view(numpy.uint8)).hexdigest(), 16)

    def __eq__(self, other):
        return numpy.array_equal(self.__wrapped, other.__wrapped)

    def __hash__(self):
        return self.__hash

    def __repr__(self):
        return 'array_hashable(' + repr(self.__wrapped) + ')'

    def unwrap(self):
        return self.__wrapped
