import sys
import logging
from logging.handlers import RotatingFileHandler
from time import gmtime, strftime


def setup_logger(log_file='debug.log'):
    logger = logging.getLogger('')
    logger.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s: %(levelname)-8s %(message)s',
                                  datefmt="%Y-%m-%d %H:%M:%S")
    
    console = logging.StreamHandler()
    console.setLevel(logging.ERROR)
    console.setFormatter(formatter)    
    logger.addHandler(console)

    file_log = RotatingFileHandler(log_file)
    file_log.setFormatter(formatter)
    logger.addHandler(file_log)
    
    return logger


def right_trim(input,suffix):
    fileName = None
    pos = input.find(suffix)
    if (pos == -1):
        fileName = input
    else:
        fileName = input[0:pos]
    return fileName