'''
"What methods and attributes are available on this udm.Object?"
'''

from __future__ import absolute_import
from __future__ import print_function
import sys
import os
import six.moves.winreg as winreg
import six
try:
    import udm
except ImportError:
    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
        meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
    sys.path.append(os.path.join(meta_path, 'bin'))
    import udm
import os.path


uml_diagram = udm.uml_diagram()
Uml = udm.map_uml_names(uml_diagram)

def print_uml_info(class_, print_=print, indent='', bases=None):
    if class_.type != Uml.Class:
        raise ValueError('class_ must be a Uml.Class. Use .type to get the class of an object')
    bases = bases or set()
    print_('{}Class {}'.format(indent, class_.name))
    if class_.association:
        print_('{}  {} --> {}'.format(indent, class_.association.children()[0].name, class_.association.children()[1].name))
    attributes = class_.children(child_type=Uml.Attribute)
    if attributes:
        print_('{}  Attributes: '.format(indent))
        for attribute in attributes:
            print_('{}    {}: {}'.format(indent, attribute.name, attribute.attr('type')))
    else:
        print_('{}  (no attributes)'.format(indent))
    associationRoles = class_.associationRoles
    if associationRoles:
        print_('{}  Association roles:'.format(indent))
        for associationRole in associationRoles:
            def theOther(role):
                return [r for r in role.parent.children() if r != role][0]
            print_('{}    {}: {}'.format(indent, theOther(associationRole).name, theOther(associationRole).target.name))
    else:
        pass
    if class_.baseTypes:
        print_('{}  Base Classes:'.format(indent))
        for baseType in class_.baseTypes:
            if baseType in bases:
                print_('{}    {}'.format(indent, baseType.name))
            else:
                bases.add(baseType)
                print_uml_info(baseType, indent=indent + '    ', bases=bases)

if __name__ == '__main__':
    meta_dn = udm.SmartDataNetwork(uml_diagram)
    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, r"Software\META") as software_meta:
        meta_path, _ = winreg.QueryValueEx(software_meta, "META_PATH")
    CyPhyML_udm = os.path.join(meta_path, r"generated\CyPhyML\models\CyPhyML_udm.xml")
    if not os.path.isfile(CyPhyML_udm):
        CyPhyML_udm = os.path.join(meta_path, r"meta\CyPhyML_udm.xml")
    meta_dn.open(CyPhyML_udm, "")
    CyPhyML = udm.map_uml_names(meta_dn.root)
    print_uml_info(CyPhyML.Component)
    print_uml_info(CyPhyML.Property)
    print_uml_info(CyPhyML.ValueFlow, print_=lambda x: print(x, file=sys.stdout))
    # print_uml_info(CyPhyML.ValueFlow.type)  # aka Uml.Class
