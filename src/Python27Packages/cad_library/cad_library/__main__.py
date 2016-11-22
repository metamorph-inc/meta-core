import os
import logging


def exitwitherror(msg, code, program=''):
    logger = logging.getLogger()
    logger.error(msg)
    with open('_FAILED.txt', 'a') as f:
        f.write(program + ': ' + msg + '\n')  # python will convert \n to os.linesep
    print(program + ': ' + msg)
    exit(code)


def setuplogger(log='log'):
    logger = logging.getLogger()
    logger.setLevel(logging.DEBUG)
    fh = logging.FileHandler(os.path.join(os.getcwd(), "log", log + '.log'), 'w')
    fh.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s: %(levelname)s - %(message)s')
    fh.setFormatter(formatter)
    logger.addHandler(fh)
    return logger