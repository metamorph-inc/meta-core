from __future__ import absolute_import

import unittest

import testbenchexecutor


class ParserTest(unittest.TestCase):
    def test_parse(self):
        self.assertEqual(testbenchexecutor.parse('python file.py -- args'), ['python', 'file.py', '--', 'args'])
        self.assertEqual(testbenchexecutor.parse('python quote"quoted"unquote  args'), ['python', 'quotequotedunquote', 'args'])
        self.assertEqual(testbenchexecutor.parse('python "file.py -- args"'), ['python', 'file.py -- args'])
        self.assertEqual(testbenchexecutor.parse(r'python "file.py --\" args\"" 2'), ['python', 'file.py --" args"', '2'])
        self.assertEqual(testbenchexecutor.parse(r'python \\fileshare\share\file.py "-- args"'), ['python', r'\\fileshare\share\file.py', '-- args'])
        self.assertEqual(testbenchexecutor.parse(r'python "\\fileshare\share\" "-- args"'), ['python', '\\\\fileshare\\share" --', 'args'])
        self.assertEqual(testbenchexecutor.parse(r'python "\\fileshare\share\\" "-- args"'), ['python', '\\\\fileshare\\share\\', '-- args'])
        self.assertEqual(testbenchexecutor.parse(r'python "\\fileshare\share\\\" "-- args"'), ['python', '\\\\fileshare\\share\\" --', 'args'])
        self.assertEqual(testbenchexecutor.parse(r'python "five_backslashes\\\\\" "-- args"'), ['python', r'five_backslashes\\" --', 'args'])
