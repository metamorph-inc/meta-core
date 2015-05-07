# split LTL formulas to retrieve variable names
import logging
import re


logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())


# constants and regular expressions:
OPERATORS = frozenset({'NOT', 'AND', 'OR', 'X', 'U', 'F', 'G'}) # alphanumeric operators
NUMERALS = re.compile('\d\S*') # just testing for beginning with a numeral and then non-whitespace


def split(formula):
    """
    Split the given formula (as string) into variable names, that do not match symbols, white space, or operators and
    start with an alphanumeric character.

    :param formula: LTL formula as a string
    :return: set of variable names (as strings) found in the LTL formula
    """
    logger.debug('Splitting formula:\n  "{}"'.format(formula))

    if not formula:
        return set([])

    # split up into candidates
    candidates = filter(None,
                        re.split(r"""\s+|,|    # white space and comma
                               \(|\)|          # parentheses
                               /=|=|<=|>=|<|>| # operators
                               \+|-|\*|/         # more operators""", formula, 0, re.VERBOSE))
    logger.debug('Candidates after simple operators are:\n  {}'.format(candidates))

    # remove numerals and alphanumeric operators
    candidates = filter(lambda x: not (NUMERALS.match(x) or x in OPERATORS), candidates)

    return set(candidates) # turn list into set with unique variable names


# if running stand-alone
if __name__ == "__main__":
    # configure logging if running stand-alone:
    logger.removeHandler(logging.NullHandler())
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(levelname)-8s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S %Z')

    try:
        import argparse
        parser = argparse.ArgumentParser(description='Split given LTL formula into set of variable names.')
        parser.add_argument('property', help='LTL formula to be split')
        parser.add_argument('-l', dest='level', help='set logging to given level', action='store')
        args = parser.parse_args()

        # reconfigure logging level (if given):
        if args.level:
            if not args.level.upper() in {'DEBUG','INFO','WARNING','ERROR','CRITICAL'}:
                logger.warning('Cannot configure logging to given level {}'.format(args.level.upper()))
            else:
                logger.setLevel(args.level.upper())
                logger.info('Logging level set to {}'.format(args.level.upper()))

        # call split functions with argument:
        print split(args.property) # display vars
    finally:
        logging.shutdown()