"""
Opens an xme, runs AddOns (to refresh libraries), then saves the xme

After updating a library, run on all xmes under META\test\InterchangeTest or regression tests fail
cd test\InterchangeTest
find -iname *.xme -exec c:\python27\python.exe ..\..\meta\refresh_xme_libraries.py "{}" ;
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
mga.Save("MGA=" + xmefile + ".mga", False)

dumper = win32com.client.DispatchEx("Mga.MgaDumper")
dumper.DumpProject(mga, xmefile)

mga.Close(True)
