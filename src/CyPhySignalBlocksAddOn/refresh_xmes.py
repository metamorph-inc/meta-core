"""
Opens an xme, runs AddOns (to refresh libraries), then saves the xme

After updating a library, run on all xmes under META\test\InterchangeTest or regression tests fail

find test/InterchangeTest -iname \*.xme | xargs -n 1 -P 4 -d \n ./bin/Python27/Scripts/python src/CyPhySignalBlocksAddOn/refresh_xmes.py
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
