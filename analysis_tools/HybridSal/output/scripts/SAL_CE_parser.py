# Author: Linda Briesemeister <linda.briesemeister@sri.com>
# Copyright: 2014 SRI International.

"""
A module for parsing SAL generated counterexamples (CE) into JSON structures or MATLAB files.


Functions:
- `parse_multiple()`   -- when input contains 1 or more counterexample(s) denoted by property names
- `parse_single()`     -- when input contains no more than 1 counterexample
- `parse_assignments() -- for assignment blocks inside counterexample steps

Exceptions:
`CEParseError` when parsing fails.


How To Use This Module
======================

From command line:
------------------

If using this module from the command line, it will parse the file specified as an argument.  Use
the log level switch "-l" to set the level to "info" or higher in order to see the JSON structure
printed to the standard output stream.

If no file is specified, then the standard input stream is parsed.

An example invocation to view the result in the terminal is, assuming that the SAL_CE_parser
package was installed at "$INSTALL_DIR":

$> python $INSTALL_DIR/SAL_CE_parser.py <TEXT FILE> -n -l info

To generate MATLAB output instead:

$> python $INSTALL_DIR/SAL_CE_parser.py <TEXT FILE> -m /Applications/MATLAB_R2013b.app -o ~/tmp/results
...
  ~~~ Find the MATLAB input files under /Users/linda/tmp/results-<PROP>-*.mat

And to view the online help:
$> python $INSTALL_DIR/SAL_CE_parser.py -h


From other Python code:
-----------------------

Obtain the data to be parsed as an array of strings that is then passed to either of the two
functions (optionally with more arguments):

  import SAL_CE_parser

  result = SAL_CE_parser.parse_multiple(data)
  result = SAL_CE_parser.parse_single(data)

"""

import logging
import logging.config
import argparse
import re
import json
import collections
import tempfile
import os
import subprocess
import sys
import LTL_splitting


logger = logging.getLogger(__name__)
logger.addHandler(logging.NullHandler())


class tcolors:
    """ANSI escape codes to generate colored output in terminals."""
    BLUE = '\033[94m' if not sys.platform.startswith('win') else ''
    RED = '\033[91m' if not sys.platform.startswith('win') else ''
    GREEN = '\033[92m' if not sys.platform.startswith('win') else ''
    BOLD = '\033[1m' if not sys.platform.startswith('win') else ''
    ENDC = '\033[0m' if not sys.platform.startswith('win') else ''


class CEParseError(Exception):
    """Custom exception raised when parsing the SAL counter example(s) fails."""
    pass


class PairsAction(argparse.Action):
    def __call__(self, parser, namespace, values, option_string=None):
        if len(values) % 2 == 0:
            # If valid, store the values as a list of tuples.
            setattr(namespace, self.dest, zip(values[::2], values[1::2]))
        else:
            # Otherwise, invoke a parser error with a message.
            parser.error('variable name and type must be supplied as pairs')


# constants and regular expressions:
CE_DELIM   = '~~~~'
STEP_DELIM = '----'

stepRE = re.compile(r'Step\s+(\d+):')                          # Step <N>:
assignRE = re.compile(r'---\s+(.*\S)\s+\(assignments\)\s+---') # --- <assignments title> (assignments) ---
varsRE = re.compile(r'(\S+)\s*=(\s*\S+)?')                     # <var> =[ <value>]
propertyNameRE = re.compile(r'PropertyName:\s+(\S+)')          # PropertyName: <name>
propertyStrRE = re.compile(r'PropertyStr:\s+(.+)$')            # PropertyStr:  <formula>[EOL]


def parse_assignments(lines, lineNo=0, input_vars=None):
    """
    Parse given `lines` as an assignment title and either single oder double lines of assignments.
    A single line of assignment is "<var> = <value".
    A double line of assignment is "<var> =\n<value>".

    :param lines: array of strings to be parsed into assignments
    :param lineNo: if given and not zero, prepend each line with a number to preserve sorting order in JSON
    :param input_vars: if given as a list of strings, only generate input assignments for these variable names
    :return: assignments as arrays of strings,
        dictionary of input assignments: either all of those found under the assignment title with "input" (if None
        are given as a parameter) or only those specified in the given parameter
    """
    if len(lines) == 0:
        return [], {}

    assignments_array = []
    input_assignments = {}

    # markers and collectors in loop:
    assignTitle = None
    assigns = {}

    lines.append('--- END (assignments) ---') # add last delimiter to make loop work
    it = iter(lines)
    for line in it:
        # look for assignment title
        assignM = assignRE.match(line.strip())
        if assignM:
            if assignTitle:
                if input_vars:
                    for k, v in assigns.iteritems():
                        if k in input_vars:
                            input_assignments[k] = v
                elif 'input' in assignTitle.lower():
                    for k, v in assigns.iteritems():
                        input_assignments[k] = v
                # collect assignments in one line:
                assignsLine = ""
                if len(assigns) > 0:
                    for k, v in assigns.iteritems():
                        assignsLine += "{0} = {1}, ".format(k,v)
                assignments_array.append("{0}{1}: {2}".format("{0:03} -- ".format(lineNo) if lineNo else "",
                                                              assignTitle,
                                                              assignsLine[0:len(assignsLine)-2] if len(assigns) > 0 else "<none>"))
                if lineNo: lineNo += 1
                assigns = {}  # reset
            assignTitle = assignM.group(1)
            if assignTitle is not "END":
                logger.debug("  -- found assignments for {}".format(assignTitle))
        elif assignTitle:
            # look for variables
            varsM = varsRE.match(line.strip())
            if varsM:
                if varsM.group(2):
                    assigns[varsM.group(1)] = varsM.group(2).strip()
                else: # assignment must be over 2 lines!
                    prev_line = line # save previous line
                    line = next(it) # consume next line
                    if not line.strip() or line.startswith('---'):
                        logger.warning("Couldn't find var assignment in consecutive lines:\n {}\n AND\n {}".format(prev_line, line))
                    else:
                        assigns[varsM.group(1)] = line.strip()
            else:
                logger.warning("Couldn't find var assignment in: {}".format(line))
        else:
            logger.warning("Couldn't find assignment title in: {}".format(line))

    return assignments_array, input_assignments


def parse_single(lines, lineNo=0, title="Counterexample generated by Cyber Composition Verification",
                 reduceToVars=None, LTL_form=None,
                 output=None, input_vars=None, matlab_path=None):
    """
    Parse `lines` as a single counterexample into a JSON structure.

    The general format of the lines (an array of strings) is the following.  We ignore empty lines or lines
    starting with "=" or "Path".

Step <N>:
[--- <assignments title> (assignments) ---  (0 or more blocks with assignments)
<assignments...>]                           (use parse_assignments() for these lines)
[----                                       (0 or 1 block with transition information after delimiter)
Transition Information:
<transition lines...>]                      (0 or more lines with information)
----                                        (required if more steps are following)
Step <M>:
<etc.>

    The resulting JSON structure could be the empty array if `lines` is empty or only contains lines with
    whitespace.  Otherwise, the JSON structure looks like this:

[{
  "GroupTitle": <title>,
  "GroupBody": [
    "Step <N>:",
    "<assignments title>: <accumulated assignments>",                -- 1 line for each assignment block
    "Transition <N> > <N+1>: <accumulated transition information>",  -- if given
    "Step <M>:",
    ...
  ]
}]

    If `lineNo` other than 0 is specified, then use this as the starting line number prepended in JSON strings
    inside the "GroupBody" array to allow a properly sorted display.

    If `title` is given, use this string as the "GroupTitle" in the JSON structure.

    If `output` prefix given then generate MATLAB files with input variable settings (using MATLAB executable).
    The file path names will have the given prefix and a message on STDOUT shows the wildcard pattern of the
    generated files (in blue for color terminals).
    If `LTL_form` is given, a header line will print the LTL formula for the property.
    If `reduceToVars` is given, then a trailing output shows the relevant ones on STDOUT (in red for color
    terminals).  The user will see terminal output like this:

~~~ Property:
    <LTL formula for property if given>
~~~ Find the MATLAB input files under <output>-*.mat
~~~ When adding a scope then limit to these variables from LTL:
    <list of variable names>

    :param lines: array of strings to be parsed
    :param lineNo: if given and not zero, prepend each line with a number to preserve sorting order in JSON
    :param title: if given, use as "GroupTitle" in JSON structure
    :param reduceToVars: if given and also MATLAB conversion requested then print alert about which ones to focus on
    :param LTL_form: if given and output is requested then print heading with the LTL formula used
    :param output: if given, use this stub to generate MATLAB output in file with name "<output>-<var>.mat"
    :param input_vars: if given, uses these variable names instead of input variables from counter example when
                       generating MATLAB output; expecting a list of pairs with variable name and type, e.g.
                       [('gear', 'int32'), ('torque', 'double)]
    :param matlab_path: if given, use this path to look for "bin/matlab" as the executable when output is also
                        given; otherwise assume that "matlab" executable is in the PATH.
    :return: JSON structure of counterexample
    """
    group_body = [] # array of strings for each line

    if len(lines) == 0:
        return []

    matlab_inputs = {}

    # markers and collectors in loop:
    step = None
    transition = None
    assignment_lines = []
    information = ''

    # last delimiter to make loop work:
    lines.append(STEP_DELIM)
    iterator = range(len(lines)).__iter__()
    for i in iterator:
        line = lines[i].strip()
        # skip blank or certain lines:
        if not line or line.startswith(('=','Path')):
            logger.debug("  -- skipping: {}".format(line))
            continue
        stepM = stepRE.match(line)
        if stepM:
            # parse "Step" information:
            step = int(stepM.group(1))
            transition = None
            logger.debug("  -- found step {}".format(step))
        elif step is not None:
            if line.startswith(STEP_DELIM):
                group_body.append("{0}Step {1}:".format("{0:03} -- ".format(lineNo) if lineNo else "", step))
                if lineNo: lineNo += 1
                assignments, input_assignments = parse_assignments(assignment_lines,
                                                                   lineNo,
                                                                   [name for name,_ in input_vars] if input_vars else None) # only first entry of each pair!
                # save input assignments into structure
                if output and input_assignments:
                    logger.debug('step {}: input assignments: {}'.format(step, input_assignments))
                    for name, value_str in input_assignments.iteritems():
                        if not name in matlab_inputs:
                            matlab_inputs[name] = ''
                        matlab_inputs[name] = matlab_inputs[name] + '{} '.format(value_str)
                group_body.extend(assignments)
                if lineNo: lineNo += len(assignments)
                assignment_lines = []
                transition = step
                step = None
            else:
                assignment_lines.append(line) # collect assignment lines
        elif transition is not None:
            if line.startswith(STEP_DELIM):
                group_body.append("{0}Transition {1} > {2}: {3}".format("{0:03} -- ".format(lineNo) if lineNo else "",
                                                                        transition,transition+1,information.strip()))
                if lineNo: lineNo += 1
                transition = None
            elif line.startswith('Transition Information:'):
                information = ''  # reset
            else:
                # collect transition information
                information += line + ' '
        else:
            logger.warning("Cannot parse counterexample line at {}: {}".format(i,line))

    if output and matlab_inputs:
        # convert input var list of tuples (pairs) into dictionary indexed by first elements:
        input_types = {n:t for n,t in input_vars} if input_vars \
            else {n:'double' for n in matlab_inputs.keys()}  # make 'double' the default type if information is missing
        # write temp .m file with variable assignments to output to MAT files
        fd, temp_path = tempfile.mkstemp(suffix='.m')
        logger.debug("Using temp file {}".format(temp_path))
        with os.fdopen(fd, "w") as tf:
            for name, array_str in matlab_inputs.iteritems():
                if not name in input_types:
                    input_types[name] = 'double'  # make double the default
                if len(array_str) > 0:
                    # create MATLAB code and write to temp file:
                    tf.write('TS = timeseries({}([{}]\'),\'Name\',\'{}\');\n'.format(
                             input_types[name],
                             array_str,
                             name))
                    if sys.platform.startswith('win') or output[0] == '/':
                        tf.write('outfile = \'{}-{}.mat\';\n'.format(output, name))
                    else: # add current working directory to output prefix
                        tf.write('outfile = [\'{}\' filesep \'{}-{}.mat\'];\n'.format(os.getcwd(), output, name))
                    tf.write('save(outfile,\'TS\',\'-v7.3\')\n\n')
            tf.flush()

        if not os.path.isfile(temp_path):
            logger.warning('No TEMP file {} so skipping MATLAB execution'.format(temp_path))
        else:
            # run MATLAB with generated .m file to generate MAT files
            command = [('matlab{}'.format('.exe' if sys.platform.startswith('win') else '')) if not matlab_path else matlab_path,
                       '-nosplash',
                       '-nodesktop',
                       '-nojvm',
                       '-wait',
                       '-r',
                       'run(\'{}\'); quit'.format(temp_path)]
            logger.info('Running MATLAB using command:\n{}'.format(command))
            subprocess.check_call(command)
            logger.info('MATLAB finished')

            print
            if LTL_form:
                print tcolors.BOLD + \
                      '  ~~~ Property:\n      ' + \
                      tcolors.GREEN + \
                      '{}'.format(LTL_form) + \
                      tcolors.ENDC
            print tcolors.BOLD + \
                  '  ~~~ Find the MATLAB input files under ' + \
                  tcolors.BLUE + \
                  '{}-*.mat'.format(output) + \
                  tcolors.ENDC
            # clean up manually (because tempfile.NamedTemporaryFile() prevents MATLAB in Windows to use the file):
            os.remove(temp_path)
            if reduceToVars:
                print tcolors.BOLD + \
                      '  ~~~ When adding a scope then limit to these variables from LTL:\n      ' + \
                      tcolors.RED + \
                      '{}'.format(list(reduceToVars)) \
                      + tcolors.ENDC
            print

    return [{"GroupTitle": title,
             "GroupBody": group_body}]


def parse_multiple(data, lineNo=0, title="Counterexample generated by Cyber Composition Verification",
                   output=None, input_vars=None, matlab_path=None):
    """
    Parse data as multiple counterexamples with property names and potentially LTL formula strings attached.

    The general format of the data (an array of strings for each line) is the following.  One or more
    properties are delimited by a line starting with CE_DELIM.

PropertyName: <name1>              (required)
[PropertyStr: <LTL formula>]       (optional)
no counterexample|Counterexample:  (required)
...                                (if "Counterexample:" then parse the single counterexample -- see above,
...                                 otherwise there should be nothing here)
~~~~                               (or more "~" to delineate the next property)

    For details on the format of each counterexample see the documentation for parse_single().

    The resulting JSON structure is an array of dictionaries for each property as follows:
{
  "ReqName": <property name>,
  ["ReqBody": <LTL formula>,]                     (if included in input)
  "Result": "SUCCESS"|"FAIL",                     (if "no counterexample" then "SUCCESS")
  "Details": <JSON structure for counterexample>  (from parsing single counterexample; could be empty)
}

    The parameters `lineNo`, `title`, and `matlab_path` are passed on to calling `parse_single()` for each
    counterexample/property.  Refer to its docstring for details.

    If `output` prefix given then generate MATLAB files with input variable settings (using MATLAB executable).
    The file names will have the pattern <output><N>-<prop>-<input var>.mat, which is also shown on the
    terminal.  If a property has an LTL formula attached, the variable names will be extracted and the user
    will also see a message about them to reduce the scope when simulating the model in MATLAB.

    :param data: array of strings to be parsed into multiple counterexamples
    :param lineNo: if given and not zero, prepend each line with a number to preserve sorting order in JSON
    :param title: if given, use as "GroupTitle" in JSON structures for each counterexample
    :param output: if given, use this stub to generate MATLAB output in files with names "<output><N>-<prop>.mat"
    :param input_vars: if given, uses these variable names instead of input variables from counter example when
                       generating MATLAB output; expecting a list of pairs with variable name and type, e.g.
                       [('gear', 'int32'), ('torque', 'double)]
    :param matlab_path: if given, use this path to look for "bin/matlab" as the executable when output is also
                        given; otherwise assume that "matlab" executable is in the PATH.
    :return: JSON structure with one entry per counterexample
    """
    result = []

    counter_parsing = 1 if output else 0

    # markers for outside loop
    counter_lines = []
    prop = "" # current property name
    form = "" # current LTL formula (if any)

    logger.debug('Parsing {} lines into multiple counterexamples'.format(len(data)))
    it = iter(data)
    for line in it:
        # skip blank lines:
        if not line.strip:
            continue
        matchName = propertyNameRE.match(line.strip())
        if matchName:
            prop = matchName.groups()[0]
            logger.debug('Property found: {}'.format(prop))
            line = next(it) # consume next line
            matchForm = propertyStrRE.match(line.strip())
            if not matchForm:
                logger.warning('No formula for property "{}" found!'.format(prop))
                form = '' # reset
            else:
                form = matchForm.groups()[0]
                line = next(it) # consume next line
            if not 'counterexample' in line.lower():
                raise CEParseError('No result for property "{}" found!'.format(prop))
        elif line.startswith(CE_DELIM): # delimiter for properties
            if not prop:
                raise CEParseError('Found delimiter for properties before obtaining property name!')
            # parse counterexample (possibly empty if no counterexample found):
            logger.info('Parsing counterexample for property "{}" with {} lines'.format(prop,len(counter_lines)))
            if output and len(counter_lines):
                output_single = '{}{}-{}'.format(output, counter_parsing, prop)
                counter_parsing += 1
            else:
                output_single = None
            d = {"Result": "FAIL" if counter_lines else "SUCCESS",
                 "ReqName": prop,
                 "Details": parse_single(counter_lines, lineNo=lineNo, title=title,
                                         reduceToVars=LTL_splitting.split(form) if output_single else None, LTL_form=form,
                                         output=output_single, input_vars=input_vars, matlab_path=matlab_path)}
            if form:
                d["ReqBody"] = form
            result.append(d)
            if len(d["Details"]):
                logger.debug("Result JSON for property {} with {} lines.".format(prop,len(d["Details"][0]["GroupBody"])))
            else:
                logger.debug("Result JSON for property {}:\n{}".format(prop,json.dumps(d,sort_keys=False,indent=2)))

            counter_lines = []  # stop counterexample gathering (if any)
        else:
            counter_lines.append(line.rstrip())  # add line without trailing newline

    logger.debug("Parsed {} result(s)".format(len(result)))
    return result


# if running stand-alone
if __name__ == "__main__":
    import sys

    # configure logging if running stand-alone:
    logger.removeHandler(logging.NullHandler())
    logging.basicConfig(level=logging.INFO,
                        format='%(asctime)s - %(levelname)-8s %(message)s',
                        datefmt='%a, %d %b %Y %H:%M:%S %Z')
    logger.info('Logging configured.')

    try:
        parser = argparse.ArgumentParser(description='Parse SAL counter example(s) from STDIN or optional file.')
        parser.add_argument('file', help='find counter example(s) in given input file', nargs='?')
        parser.add_argument('-l', dest='level', help='set logging to given level', action='store')
        parser.add_argument('-n', dest='numbered', help='if given, add line numbers to JSON output', action='store_true')
        parser.add_argument('-j', dest='json_output', help='if given, print result as JSON output', action='store_true')
        parser.add_argument('-o', dest='output', action='store',
                            help='generate MATLAB output in file(s) named with given stub plus "(<N>-<prop>)-<var name>.mat"')
        parser.add_argument('-v', dest='vars', nargs = '+', action = PairsAction,
                            help='when generating MATLAB output, instead of input variable names of counter example use given list of pairs of variable name and type')
        parser.add_argument('-m', dest='matlab_path', action='store',
                            help='use given path to MATLAB root for executing MATLAB (only used when -o given)')
        args = parser.parse_args()

        # investigate dependencies of optionals:
        if args.matlab_path:
            if not args.output:
                logger.warning('Ignoring option \'-m\' as \'-o\' not given.')
                args.matlab_path = None  # reset to ignore
            else:
                if not os.path.isdir(args.matlab_path):
                    logger.critical('Given MATLAB root is not a directory: {}'.format(args.matlab_path))
                    sys.exit(2)
                args.matlab_path = os.path.join(args.matlab_path,"bin",
                                                "matlab{}".format(".exe" if sys.platform.startswith('win') else ""))
                if not os.path.exists(args.matlab_path):
                    logger.critical('Cannot find MATLAB binary: {}'.format(args.matlab_path))
                    sys.exit(3)
        if args.vars and not args.output:
            logger.warning('Ignoring option \'-v\' as \'-o\' not given.')
            args.vars = None  # reset to ignore

        # reconfigure logging level (if given):
        if args.level:
            if not args.level.upper() in {'DEBUG','INFO','WARNING','ERROR','CRITICAL'}:
                logger.warning('Cannot configure logging to given level {}'.format(args.level.upper()))
            else:
                logger.setLevel(args.level.upper())
                logger.info('Logging level set to {}'.format(args.level.upper()))

        # determine input (if any):
        if args.file:
            # try as file
            try:
                with open(args.file, 'rU') as f:
                    # make sure to use universal newline mode,
                    # as result files sometimes have mixed breaks (from Cygwin/SAL)
                    data = f.readlines()
            except IOError as _:
                logger.exception('Unable to open given file {}'.format(args.file))
                sys.exit(4)
        else:
            # try STDIN
            data = sys.stdin.readlines()
        if not data:
            logger.critical('Cannot find data source')
            sys.exit(1)
        logger.debug('Data has {} lines'.format(len(data)))

        # look ahead into data to determine whether
        # one or multiple counter examples are to be parsed:
        result = []
        for linenum, line in enumerate(data, start=0):
            # gobble up empty lines
            if not line.strip():
                logger.debug('Skipping empty line')
                continue
            # if first line matches "Counterexample" then only one, else parse for multiple
            logger.debug('Starting at line number {}'.format(linenum+1))
            if line.find('ounterexample') == -1:
                logger.info('Parsing multiple counterexamples')
                result = parse_multiple(data[linenum:], lineNo=1 if args.numbered else 0,
                                        output=args.output, input_vars=args.vars, matlab_path=args.matlab_path)
            else:
                logger.info('Parsing 1 counterexample')
                result = parse_single(data[linenum:], lineNo=1 if args.numbered else 0,
                                      output=args.output, input_vars=args.vars, matlab_path=args.matlab_path)
            break

        if args.json_output:
            print('Result is:\n{}'.format(json.dumps(result, sort_keys=False, indent=2)))
    finally:
        logging.shutdown()
        sys.exit(0)
