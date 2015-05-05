# Author: Di Yao (dyao@isis.vanderbilt.edu)
# Name: CreateCsysFromStep.py
# Description: Contains classes and functions to an xml file conforming to CADDatumEditor schema.
#              Calls StepFileUtility to parse a STEP file to Coordinate System objects.
#              Populates the xml file with Coordinate System objects.
#
# Version: 0.1 (11/15/2012) - first draft
#          0.2 (11/29/2012) - formatted xml to look pretty using minidom
#                             bug fix so <AddCoordinateSystems> is before <AddDatums>
#                             bug fix so <Origin> is before <XVector> and <YVector>
#          0.3 (12/11/2012) - schema change: Type, Format and File attributes to <CADComponent>
#                             changed cmd line arguments to -i, -m, and -t
#          0.4 (12/20/2012) - DirectoryPath attribute changed to "." from "C:\Temp\scratch\2012_10_02\DeleteDatums"

import os
import sys
import xml.etree.cElementTree as ET
import logging
import StepFileUtility
from time import gmtime, strftime
from xml.dom import minidom             #for making output xml prettier


def setup_logger():
    logger = logging.getLogger('root')
    logger.setLevel(logging.DEBUG)
    fh = logging.FileHandler('Log_CreateCsy.log')
    fh.setLevel(logging.DEBUG)

    ch = logging.StreamHandler()
    ch.setLevel(logging.ERROR)

    formatter = logging.Formatter('%(asctime)s - %(name)s - %(levelname)s - %(message)s')
    ch.setFormatter(formatter)
    fh.setFormatter(formatter)

    logger.addHandler(ch)
    logger.addHandler(fh)

    return logger

module_logger = setup_logger()

class CADDatumXmlHandler:
    def __init__(self, part_name, part_step_file, part_type, csys):
        self._filename = part_name + '_CSYS.xml'
        self._coordinate_system = csys
        self._logger = logging.getLogger('root.XmlHandler')
        self._part_name = part_name
        self._part_type = part_type
        self._part_step_file = part_step_file
    
    def Deserialize(self):
        try:
            import shutil
            shutil.copyfile(self._filename, 'ORIGINAL_' + self._filename)
            count = 0
            tree = ET.ElementTree(file=self._filename)            
            for elem in tree.iter(tag='CADComponent'):
                count+1
            if count > 1:
                self._logger.error('%d CADComponents found in xml file, should only have 1' %count)
                sys.exit(0)
                
            for component in tree.iter(tag='CADComponent'):
                found_add_csys = False
                #print component.tag, component.attrib
                for add in component.iterfind('Add'):
                    for add_coord in add.iterfind('AddCoordinateSystems'):
                        self.SerializeCoordinateSystems(add_coord)
                        found_add_csys = True
                    if found_add_csys == False:     #create a new AddCoordinateSystems node
                        add_coord = ET.Element('AddCoordinateSystems')
                        add.insert(0, add_coord)
                        self.SerializeCoordinateSystems(add_coord)


            #No pretty print of xml
            #tree.write(self._filename,
            #                encoding='utf-8')
            
        except Exception, inst:
            self._logger.error('Unexpected error in handling %s: %s' %(self._filename, str(inst)))
            return

    def Serialize(self):
        root = ET.Element('CADDatumEditor')
        root.set('xmlns:' + 'xsi', 'http://www.w3.org/2001/XMLSchema-instance')
        root.set('xsi:' + 'noNamespaceSchemaLocation', 'CADDatumEditor.xsd')
        cad_components_node = ET.SubElement(root, 'CADComponents')
        libraries_node = ET.SubElement(root, 'Libraries')
        library_node = ET.SubElement(libraries_node, 'Library')
        library_node.attrib['DirectoryPath'] = '.'
        library_node.attrib['ID'] = 'Lib1'
        cad_component_node = ET.SubElement(cad_components_node, 'CADComponent')
        cad_component_node.attrib['LibraryID'] = 'Lib1'
        cad_component_node.attrib['Type'] = self._part_type
        cad_component_node.attrib['Name'] = self._part_name
        cad_component_node.attrib['File'] = self._part_step_file
        cad_component_node.attrib['Format'] = 'STEP'
        add_node = ET.SubElement(cad_component_node, 'Add')
        add_csys_node = ET.SubElement(add_node, 'AddCoordinateSystems')
        self.SerializeCoordinateSystems(add_csys_node)


        import shutil
        if os.path.exists(self._filename):
            shutil.copyfile(self._filename, 'ORIGINAL_' + self._filename)
            
        rough_string = ET.tostring(root, 'utf-8')
        reparsed = minidom.parseString(rough_string)
        f=open(self._filename, 'w')
        f.write(reparsed.toprettyxml(indent="  "))

        
        
    def SerializeCoordinateSystems(self, add_csys_main_node):
        import decimal
        for key, item in self._coordinate_system.items():
            add_coord = ET.SubElement(add_csys_main_node, 'AddCoordinateSystem')
            add_coord.attrib['ReplaceIfExists'] = 'False'
            add_coord.attrib['CoordinateSystemName'] = 'CSY_%d' %key
            origin = None
            if item._x is not None:
                if item._x._origin is not None:
                    origin = item._x._origin
                xvector = ET.SubElement(add_coord, 'XVector')
                xvector.attrib['X'] = "%0.19f" %item._x._vector[0]
                xvector.attrib['Y'] = "%0.19f" %item._x._vector[1]
                xvector.attrib['Z'] = "%0.19f" %item._x._vector[2]
            if item._y is not None:
                if item._y._origin is not None:
                    origin = item._x._origin
                yvector = ET.SubElement(add_coord, 'YVector')
                yvector.attrib['X'] = "%0.19f" %item._y._vector[0]
                yvector.attrib['Y'] = "%0.19f" %item._y._vector[1]
                yvector.attrib['Z'] = "%0.19f" %item._y._vector[2]
            if origin is not None:
                origin_node = ET.Element('Origin')
                origin_node.attrib['X'] = "%0.19f" %origin[0]
                origin_node.attrib['Y'] = "%0.19f" %origin[1]
                origin_node.attrib['Z'] = "%0.19f" %origin[2]
                add_coord.insert(0, origin_node)
    
if __name__ == '__main__':
    from optparse import OptionParser
    import os.path
    usage = "usage: %prog [options] arg1"
    cmd_line_parser = OptionParser(usage=usage)

    cmd_line_parser.add_option("-i", "",
                  action="store", type='string', dest="csys_file",
                  help="Name of coordinate system STEP file")
    cmd_line_parser.add_option("-m", "",
                  action="store", type='string', dest="model_step_file",
                  help="Name of component step file")
    cmd_line_parser.add_option("-t", "",
                  action="store", type='string', dest="component_type",
                  help="Type of component (Part/Assembly)")

    (options, args) = cmd_line_parser.parse_args()


    entity_name = ''

    if not os.path.exists(options.csys_file):
        module_logger.debug('Csys STEP file does not exist: {0}'.format(options.csys_file))
        sys.exit(0)

    path, filename = os.path.split(options.model_step_file)
    filename, ext = os.path.splitext(filename)
    print filename
    simple_parser = StepFileUtility.SimpleStepParser(options.csys_file)
    coordinate_system_populator = StepFileUtility.PopulateCoordinateInfo(simple_parser)
    csys_map = coordinate_system_populator.create_coordinate_vector()
    xml_handler = CADDatumXmlHandler(filename, options.model_step_file, options.component_type, csys_map)
    xml_handler.Serialize()