__author__ = 'rowens'


class CADError(Exception):
    """
    Base class for all CAD exceptions
    """

    def __init__(self, message):
        self.message = message
