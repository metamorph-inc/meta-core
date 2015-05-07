
import unittest
import subprocess
import re
import sys
import os
import os.path
_script_dir = os.path.dirname(os.path.realpath(__file__))


class UnconnectedPortsTest(unittest.TestCase):
    def setUp(self):
        os.chdir(_script_dir)
    
    def execute(self, command, regexps=[], should_fail=False):
        if isinstance(command, basestring):
            command = command.split()
        if isinstance(regexps, basestring):
            regexps = [regexps]
        sub = subprocess.Popen(command, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        (stdout,stderr) = sub.communicate()
        if should_fail:
            self.assertNotEqual(sub.returncode, 0)
        else:
            self.assertEqual(sub.returncode, 0)
        for regexp in regexps:
            if re.search(regexp, stdout) is None:
                print stdout
                self.fail("Output of command '%s' did not match '%s'" % (repr(command), regexp))

    def test1(self):
        '''Unconnected dataflow input port inside component.  
        Should give an error in InnerLoop_InnerLoop_InnerLoop.  
        qint_inputport_unconnected_error.mga
        '''
        self.execute("Stage1D -f qint_inputport_unconnected_error.mga -o qint_inputport_unconnected_error.xml", 
            "ERROR.*Deployment_InnerLoop_InnerLoop_InnerLoop.*Input port has no connections", should_fail=True)

    def test2(self):
        '''Unconnected dataflow output port inside component.  
        Should give a warning in InnerLoop_InnerLoop_InnerLoop.
        Should create a dummy message structure.
        qint_outputport_unconnected_dummy.mga
        '''
        self.execute("Stage1D -f qint_outputport_unconnected_dummy.mga -o qint_outputport_unconnected_dummy.xml", 
            "WARNING.*Deployment_InnerLoop_InnerLoop_InnerLoop.*Output port ang_vel has no connector")
        self.execute("Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy.scs -t Sched -r 1ms")
        self.execute("SchedTool -f qint_outputport_unconnected_dummy.scs -o qint_outputport_unconnected_dummy.rslt")
        self.execute("SchedResults -r qint_outputport_unconnected_dummy.rslt -a qint_outputport_unconnected_dummy.xml")
        self.execute("Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy -p qint_outputport_unconnected_dummy_tt -t TrueTime")
        self.execute("Stage2D -f qint_outputport_unconnected_dummy.xml -o qint_outputport_unconnected_dummy -p qint_outputport_unconnected_dummy_frodo -t FRODO")
    
    def test3(self):
        '''
        Unconnected field in an input message.  
        Should give a warning for port vel of message ang_ref.
        qint_inmsgfield_unconnected_warning.mga
        '''
        self.execute("Stage1D -f qint_inmsgfield_unconnected_warning.mga -o qint_inmsgfield_unconnected_warning.xml",
            "WARNING.*Models_RS-GS Deployment.*Port vel of input message instance ang_ref is unconnected.")

    def test4(self):
        '''Unconnected field in an output message.  
        Should give an error for port ang4 of message thrust_commands.
        qint_outmsgfield_unconnected_error.mga
        '''
        self.execute("Stage1D -f qint_outmsgfield_unconnected_error.mga -o qint_outmsgfield_unconnected_error.xml",
            "ERROR.*Models_RS-GS Deployment.*Port ang4 of output message instance thrust_commands is unconnected.", should_fail=True)

    def test5(self):
        '''Unconnected component input message port. 
        Should give errors for OuterLoop/pos_msg and InnerLoop/ang_ref. 
        qint_inputmsg_unconnected_error.mga
        '''
        self.execute("Stage1D -f qint_inputmsg_unconnected_error.mga -o qint_inputmsg_unconnected_error.xml",
            ["ERROR.*Input message instance OuterLoop/pos_msg is not connected.",
            "ERROR.*Message instance OuterLoop/pos_msg is unconnected, but has an execInfo timing specification",
            "ERROR.*Input message instance InnerLoop/ang_ref is not connected.",
            "ERROR .*Message instance InnerLoop/ang_ref is unconnected, but has an execInfo timing specification",
            ], should_fail=True)

    def test6(self):
        '''
        Unconnected component output message port. 
        Should give a warning for output message InnerLoop/thrust_commands.
        Ignore other warnings.
        qint_outputmsg_unconnected_warning.mga
        '''
        self.execute("Stage1D -f qint_outputmsg_unconnected_warning.mga -o qint_outputmsg_unconnected_warning.xml",
            "WARNING.*Output message instance InnerLoop/thrust_commands is not connected")
        self.execute("Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning.scs -t Sched -r 1ms")
        self.execute("SchedTool -f qint_outputmsg_unconnected_warning.scs -o qint_outputmsg_unconnected_warning.rslt")
        self.execute("SchedResults -r qint_outputmsg_unconnected_warning.rslt -a qint_outputmsg_unconnected_warning.xml")
        self.execute("Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning -p qint_outputmsg_unconnected_warning_tt -t TrueTime")
        self.execute("Stage2D -f qint_outputmsg_unconnected_warning.xml -o qint_outputmsg_unconnected_warning -p qint_outputmsg_unconnected_warning_frodo -t FRODO")

    def test7(self):
        '''
        Partially connected message port -- 
        CommMapping to a Bus channel, but no dependency.  
        Should give an error for four different CommMapping connections.
        qint_inputmsg_partially_connected_error.mga
        '''
        self.execute("Stage1D -f qint_inputmsg_partially_connected_error.mga -o qint_inputmsg_partially_connected_error.xml",
            [
            "ERROR.*Message instance InnerLoop/ang_ref has a CommMapping connection, but no Dependency connection",
            "ERROR.*Message instance DataHandling/pos_msg has a CommMapping connection, but no Dependency connection",
            "ERROR.*Message instance OuterLoop/pos_msg has a CommMapping connection, but no Dependency connection",
            "ERROR.*Message instance OuterLoop/ang_ref has a CommMapping connection, but no Dependency connection",
            ], should_fail=True)

    def test8(self):
        '''
        Unconnected dataflow output port inside component.  
        Should give a warning in InnerLoop_InnerLoop_InnerLoop, and in
         DataHandling_DataHandling_DataHandling.
        Ignore other warnings.
        Should create a dummy message structure with two fields.
         qint_outputport_unconnected_2dummy.mga
         '''
        self.execute("Stage1D -f qint_outputport_unconnected_2dummy.mga -o qint_outputport_unconnected_2dummy.xml",
            [
            "WARNING.*Deployment_InnerLoop_InnerLoop_InnerLoop.*Output port ang_vel has no connector - creating a dummy message.",
            "WARNING.*Deployment_DataHandling_DataHandling_DataHandling.*Output port pos_data_out2 has no connector - creating a dummy message.",
            ])
        self.execute("Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy.scs -t Sched -r 1ms")
        self.execute("SchedTool -f qint_outputport_unconnected_2dummy.scs -o qint_outputport_unconnected_2dummy.rslt")
        self.execute("SchedResults -r qint_outputport_unconnected_2dummy.rslt -a qint_outputport_unconnected_2dummy.xml")
        self.execute("Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy -p qint_outputport_unconnected_2dummy_tt -t TrueTime")
        self.execute("Stage2D -f qint_outputport_unconnected_2dummy.xml -o qint_outputport_unconnected_2dummy -p qint_outputport_unconnected_2dummy_frodo -t FRODO")

    def test9(self):
        '''
        Partially connected message port --
            No CommMapping or Dependency, but it still has an execinfo object.
        Should give an error for output message InnerLoop/thrust_commands.
        Ignore other warnings.
         qint_outputmsg_partial_execinfo_error.mga
         '''
        self.execute("Stage1D -f qint_outputmsg_partial_execinfo_error.mga -o qint_outputmsg_partial_execinfo_error.xml",
            "ERROR.*Message instance InnerLoop/thrust_commands is unconnected, but has an execInfo timing specification",
            should_fail=True)

def suite():
    return unittest.TestLoader().loadTestsFromTestCase(UnconnectedPortsTest)

if __name__ == '__main__':
    import optparse
    parser = optparse.OptionParser()
    parser.add_option("-x", "--xml", action="store_true", dest="xml")

    (options, args) = parser.parse_args()
    if options.xml:
        path = os.path.join(_script_dir, '../../../common-scripts')
        if not path in sys.path:
            sys.path.insert(1, path)
        del path
        import xmlrunner

        with open(os.path.join(_script_dir, "tests.xml"), "w") as output:
            output.write("<testsuites>")
            for test in suite()._tests:
                runner = xmlrunner.XMLTestRunner(output)
                runner.run(test)
            output.write("</testsuites>")
    else:
        runner = unittest.TextTestRunner()
        runner.run(suite())

