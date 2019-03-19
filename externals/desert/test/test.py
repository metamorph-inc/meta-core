import os
import os.path
import unittest
import subprocess
import errno
import io

_this_dir = os.path.dirname(os.path.abspath(__file__))

class DesertError(Exception):
    pass

class TestDesert(unittest.TestCase):

    def run_desert(self, args):
        try:
            subprocess.check_output([os.path.join(_this_dir, r"..\DesertTool\..\bin\DesertTool.exe")] + args, stderr=subprocess.STDOUT)
        except subprocess.CalledProcessError as e:
            raise DesertError('..\\bin\\DesertTool.exe ' + ' '.join('"' + arg + '"' for arg in args) + '\n' + e.output)

    def test_parse_c(self):
        with self.assertRaisesRegexp(DesertError, "/c requires an argument"):
            self.run_desert(["test_input.xml", "/C", "/c"])

    def test_parse_o(self):
        with self.assertRaisesRegexp(DesertError, "/o requires an argument"):
            self.run_desert(["test_input.xml", "/c", "all", "/o"])

    def test_parse_m(self):
        with self.assertRaisesRegexp(DesertError, "/m requires an argument"):
            self.run_desert([r"test_input.xml", "/m"])

    def test_parse_M(self):
        with self.assertRaisesRegexp(DesertError, "/M requires an argument"):
            self.run_desert([r"test_input.xml", "/M"])

    def unlink(self, filename):
        try:
            os.unlink(filename)
        except OSError as e:
            if e.errno not in (errno.ENOENT, errno.ENOTDIR):
                raise

    def run_desert_and_parse(self, args):
        output_filename = "test_output.xml"
        output_configs_filename = "test_output_configs.xml"
        self.unlink(output_filename)
        self.unlink(output_configs_filename)

        self.run_desert(args)
        import xml.etree.ElementTree as ET
        configs = ET.parse(output_configs_filename).getroot()
        output = None
        if os.path.isfile(output_filename):
            output = ET.parse(output_filename).getroot()
        return output, configs

    def test_malformed_input(self):
        with io.open('test_input.xml', encoding='utf8') as test_input:
            with io.open('malformed_input.xml', 'w', encoding='utf8') as malformed_input:
                malformed_input.write(test_input.read().replace('\"id177\"', '\"id177123\"', 1))
        with self.assertRaisesRegexp(DesertError, "Error during parsing"):
            self.run_desert([r"malformed_input.xml", "/s"])

    def test_count(self):
        output, configs = self.run_desert_and_parse(["test_input.xml", "/C", "/c", "applyAll", "/o", "test_output.xml"])
        self.assertEqual(25, len(configs.findall('Count')))
        self.assertEqual('134', configs.find('NumberOfConfigurations').attrib['count'])
        self.assertEqual(4, len(configs.findall('Count[@count="15"]')))
        self.assertEqual(1, len(configs.findall('Count[@id="-300000632"]')))

    def test_constraints(self):
        output, configs = self.run_desert_and_parse(["test_input.xml", "/c", "applyAll", "/o", "test_output.xml"])
        self.assertEqual('134', configs.find('All').attrib['NumConfigs'])
        self.assertEqual(134, len(output.findall('Configuration')))
        self.assertEqual(31, len(output.findall('Element')))


if __name__ == '__main__':
    unittest.main()