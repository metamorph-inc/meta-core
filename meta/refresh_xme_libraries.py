"""
Opens an xme, runs AddOns (to refresh libraries), then saves the xme

After updating a library, run on all xmes under META\test\InterchangeTest or regression tests fail
cd test\InterchangeTest
find -iname *.xme | xargs -d \n -n 1 -P 8 c:\python27\python.exe ..\..\meta\refresh_xme_libraries.py
For merge conflict, take tf and run this after:
svn status -q | grep xme | sed s/........// | sed s!\\!/!g | xargs -n 1 c:\python27\python.exe ..\..\meta\refresh_xme_libraries.py
"""

import pythoncom
import sys
import win32com.client

xmefile = sys.argv[1]

xme = win32com.client.DispatchEx("Mga.MgaParser")
(paradigm, parversion, parguid, basename, ver) = xme.GetXMLInfo(xmefile)

mga = win32com.client.DispatchEx("Mga.MgaProject")
mga.EnableAutoAddOns(True)
mga.Create("MGA=" + xmefile + ".mga", "CyPhyML") # uncomment and GME crashes
xme = win32com.client.DispatchEx("Mga.MgaParser")
resolver = win32com.client.DispatchEx("Mga.MgaResolver")
resolver.IsInteractive = False
xme.Resolver = resolver
xme.ParseProject(mga, xmefile)
pythoncom.PumpWaitingMessages()
pythoncom.PumpWaitingMessages()

def use_si_units():
    mga.BeginTransactionInNewTerr()
    try:
        si_units = {}
        filter = mga.CreateFilter()
        filter.Kind = "si_unit"
        for si_unit in mga.AllFCOs(filter):
            si_units[si_unit.Name] = si_unit

        filter = mga.CreateFilter()
        filter.Kind = "conversion_based_unit"
        for conversion_based_unit in mga.AllFCOs(filter):
            si_unit = si_units.get(conversion_based_unit.Name)
            if si_unit is None:
                continue
            for fco in conversion_based_unit.ReferencedBy:
                if fco.IsLibObject:
                    continue
                fco.Referred = si_unit
                print fco.Name
    finally:
        mga.CommitTransaction()
# use_si_units()

mga.Save("MGA=" + xmefile + ".mga", False)

dumper = win32com.client.DispatchEx("Mga.MgaDumper")
dumper.DumpProject(mga, xmefile)

mga.Close(True)
