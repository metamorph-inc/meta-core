
import os
import ntpath
import os.path
import sys
#import code
#code.InteractiveConsole(locals=locals()).interact()
sys.path.append(os.path.dirname(os.path.abspath(__file__)) + "../../externals/common-scripts")
import gme
import unittest

class TestIFVTest(unittest.TestCase):
    def setUp(self):
        self.ifv = None
        testdir = os.path.dirname(os.path.abspath(__file__))
        self.ifv = gme.Project.open(os.path.join(testdir, '..', '..', 'models', 'IFV', 'IFV.mga'))

    def testSuspControl(self):
        self.ifv.begin_transaction();
        altcontainer = self.ifv.project.ObjectByPath("@Component Library|kind=Components/@Computing|kind=Components/@SuspensionControl|kind=Components/@AltAssemblies|kind=ComponentAssemblies/@SuspControlAssembly|kind=AlternativeRepresentationContainer")
        self.ifv.run_interpreter("MGA.Interpreter.CyPhy2ESMoL", focusobj=altcontainer, param=128)
        self.ifv.commit_transaction();

    def tearDown(self):
        if self.ifv:
            self.ifv.project.Close(True)

if __name__=='__main__':
    unittest.main()
