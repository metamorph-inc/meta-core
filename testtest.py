import sys
sys.path.append(r'C:\Users\kevin\Documents\meta-core\test\PythonTest')
import test_CyPhyPython

test_CyPhyPython.main()
# sys.exit(0)

del test_CyPhyPython
del sys.modules['test_CyPhyPython']
if 'CyPhyPython' in sys.modules:
    del sys.modules['CyPhyPython']
import gc
gc.collect()

import ctypes
ctypes.windll.Ole32.CoFreeUnusedLibraries()
ctypes.windll.Ole32.CoFreeUnusedLibraries()
ctypes.windll.Ole32.CoFreeUnusedLibraries()


r'''
import sys
p = lambda m: gme.ConsoleMessage(repr(m), 1)

'''