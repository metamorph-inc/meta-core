__author__ = 'adam'

import sys
import os
import xml
import avm
import avm.modelica
import avm.cad
import avm.manufacturing
import avm.cyber

if __name__ == "__main__":
    import_file_path = sys.argv[1]

    f_input_file = open(import_file_path, "r")
    xml_input = f_input_file.read()
    dom_input = xml.dom.minidom.parseString(xml_input)

    extension = str.lower(os.path.splitext(import_file_path)[1])
    if extension == ".acm":
        c = avm.Component.createFromDOM(dom_input)
    elif extension == ".adm":
        d = avm.Design.createFromDOM(dom_input)
    else:
        print("Input was neither an ACM or ADM file")
        exit(-1)