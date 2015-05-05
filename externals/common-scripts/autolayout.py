import gme

import threading
import win32gui
import win32con
import win32com
import re
import time
import sys

def start_autolayout():
	def windowEnumerationHandler(hwnd, resultList):
		resultList.append((hwnd, win32gui.GetWindowText(hwnd)))
	while True:
		topWindows = []
		win32gui.EnumWindows(windowEnumerationHandler, topWindows)
		windows = filter(lambda x: re.match("GME AutoLayout Component", x[1]), topWindows)
		if windows:
			hwnd = windows[0][0]
			break
		else:
			time.sleep(0.2)
	childWindows = []
	win32gui.EnumChildWindows(hwnd, windowEnumerationHandler, childWindows)
	optimize = filter(lambda x: re.match("Optimize Selected", x[1]) , childWindows)[0][0]
	def click(hwnd):
	    win32gui.SendMessage(hwnd, win32con.WM_LBUTTONDOWN, 0, 0)
	    win32gui.SendMessage(hwnd, win32con.WM_LBUTTONUP, 0, 0)
	click(optimize)


if __name__ == '__main__':
	gme = win32com.client.Dispatch("GME.Application")
	try:
		gme.Visible = 0
		gme.OpenProject("MGA=%s" % sys.argv[1])
		try:
			launcher = win32com.client.Dispatch("Mga.MgaLauncher")
			territory = gme.MgaProject.BeginTransactionInNewTerr()
			try:
				models = filter(lambda x: x.mga_type() == "Model" and not x.in_library(), gme.MgaProject.AllFCOs(gme.MgaProject.CreateFilter()))
			finally:
				gme.MgaProject.AbortTransaction()
				territory.Destroy()
				territory = None
			for model in models:
				gme.ShowFCO(model, False)
				threading.Thread(target=start_autolayout, name="Autolayout Starter").start()
				gme.RunComponent("MGA.Interpreter.AutoLayout")
			gme.SaveProject()
		finally:
			gme.CloseProject(0)
	finally:
		gme.Exit()

