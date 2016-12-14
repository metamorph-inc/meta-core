from __future__ import absolute_import
from __future__ import print_function
from openmdao.api import Component
import os
import os.path
import win32com.client
from xml.etree import ElementTree as ET
import json
import six
import pythoncom
import winerror
import shutil


class ExcelWrapper(Component):

    """An Excel OpenMDAO Wrapper."""

    def __init__(self, excelFile, varFile, macros=[]):
        super(ExcelWrapper, self).__init__()
        self.var_dict = None
        self.xlInstance = None
        self.workbook = None
        self.macroList = list(macros)
        self.excelFile = None

        if not varFile.endswith('.json'):
            self.xmlFile = varFile
            self.create_xml_dict()
        else:
            self.jsonFile = varFile
            self.create_json_dict()

        for key, value in self.var_dict.items():
            if key == "params":
                for z in value:
                    # print(repr(z))
                    self.add_param(**z)
            elif key == "unknowns":
                for z in value:
                    # print z["name"]
                    self.add_output(**z)

        self.xl_sheet = None
        self.ExcelConnectionIsValid = True
        if not os.path.exists(excelFile):
            open(excelFile)  # fail fast

        # Excel opens the file with sharing=none. Make a copy so we can run multi-process
        excelCopy = '{1}_tmp{0}{2}'.format(os.getpid(), *os.path.splitext(excelFile))
        shutil.copyfile(excelFile, excelCopy)
        self.excelFile = excelCopy
        self.excelFile = os.path.abspath(self.excelFile)
        xl = self.openexcel()
        self.xlInstance = xl
        self.workbook = xl.Workbooks.Open(self.excelFile)
        self.workbook = xl.ActiveWorkbook

    def __del__(self):
        if self.workbook is not None:
            self.workbook.Close(SaveChanges=False)

        if self.xlInstance is not None:
            del self.xlInstance
            self.xlInstance = None

        if self.excelFile:
            os.unlink(self.excelFile)

    def _coerce_val(self, variable):
        if variable['type'].lower() == 'bool':
            variable['val'] = variable['val'].lower() == 'true'
        elif variable['type'].lower() == 'str':
            variable['val'] = six.text_type(variable['val'])
        else:
            variable['val'] = getattr(six.moves.builtins, variable['type'].lower())(variable['val'])

    def create_xml_dict(self):
        tree = ET.parse(self.xmlFile)

        self.var_dict = {
            "unknowns": [],
            "params": []
        }
        variables = tree.findall("Variable")
        for v in variables:
            kwargs = dict(
                [(key, v.attrib[key]) for key in ('name', 'val', 'desc', 'units', 'row', 'column', 'sheet', 'type') if
                 key in v.attrib])
            self._coerce_val(kwargs)
            if v.attrib['iotype'] == 'in':
                self.var_dict["params"].append(kwargs)
            elif v.attrib['iotype'] == 'out':
                self.var_dict["unknowns"].append(kwargs)

    def create_json_dict(self):
        with open(self.jsonFile) as jsonReader:
            self.var_dict = json.load(jsonReader)
            for vartype in ('params', 'unknowns'):
                for var in self.var_dict.get(vartype, []):
                    self._coerce_val(var)
            self.macroList.extend(self.var_dict.get('macros', []))

    def openexcel(self):
        try:
            return win32com.client.DispatchEx("Excel.Application")
        except pythoncom.com_error as e:
            if e.hresult & 0xffffffff in (0x800401f3, 0x80040154):
                raise RuntimeError('Excel is not installed')
            raise

    def letter2num(self, letters, zbase=False):
        letters = str(letters)
        letters_up = str(letters.upper())
        res = 0
        weight = len(letters_up) - 1
        for i, c in enumerate(letters_up):
            res += (ord(c) - 64) * 26 ** (weight - i)
        if not zbase:
            return res
        return res - 1

    def solve_nonlinear(self, params, unknowns, resids):

        if not self.ExcelConnectionIsValid or \
                        self.xlInstance is None or \
                        self.workbook is None:
            print("Aborted Execution of Bad ExcelWrapper Component Instance")
            return

        wb = self.workbook
        # namelist = [x.name for x in wb.Names]

        data_x = self.var_dict

        value = data_x.get("params", tuple())
        for z in value:
            name = z["name"]
            if 'row' in z and 'column' in z:
                xl_sheet = self.xlInstance.Sheets(z.get('sheet', 1))

                xl_sheet.Select()
                xl_sheet.Cells(z["row"], self.letter2num(z["column"])).value = params[name]
            else:
                try:
                    cell = wb.Names(name)
                except pythoncom.com_error as e:
                    if e.hresult == winerror.DISP_E_EXCEPTION:
                        if (0xffffffff & e.excepinfo[-1]) == 0x800a03ecL:  # seems to be a catch-all Excel error
                            raise ValueError("Unknown named cell '{}'".format(name))
                    raise e
                self.xlInstance.Range(cell.RefersToLocal).Value = params[name]

        for macro in self.macroList:
            self.xlInstance.Run(macro)

        value = data_x.get("unknowns", tuple())
        for z in value:
            name = z["name"]
            if "row" in z and "column" in z:
                xl_sheet = self.xlInstance.Sheets(z.get('sheet', 1))

                xl_sheet.Select()
                excel_value = xl_sheet.Cells(z["row"], self.letter2num(z["column"])).value
            else:
                excel_value = self.xlInstance.Range(wb.Names(name).RefersToLocal).Value
            # print excel_value
            if z["type"] == 'Float':
                unknowns[name] = float(excel_value)
            elif z["type"] == 'Int':
                unknowns[name] = int(excel_value)

            elif z["type"] == 'Bool':
                unknowns[name] = excel_value
            elif z["type"] == 'Str':
                unknowns[name] = str(excel_value)


if __name__ == '__main__':
    import sys
    # print(repr(sys.argv[1:]))
    c = ExcelWrapper(*sys.argv[1:])
    print((json.dumps({'params': c._init_params_dict, 'unknowns': c._init_unknowns_dict})))
