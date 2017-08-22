from __future__ import absolute_import
from __future__ import print_function
import sys
import argparse
from subprocess import call
import os.path
import six
from six.moves import winreg
from six.moves import range


def exitwitherror(msg, code):
    f = open('_FAILED.txt', 'a')
    f.write(msg + '\n')  # python will convert \n to os.linesep
    f.close()  # you can omit in most cases as the destructor will call if
    sys.exit(code)


def get_latest_nastran_directory():
    def enum_keys(handle):
        for i in range(2**10):
            try:
                yield winreg.EnumKey(handle, i)
            except WindowsError as e:
                if e.winerror != 259:
                    raise
                return

    with winreg.OpenKey(winreg.HKEY_LOCAL_MACHINE, "SOFTWARE\\MSC.Software Corporation", winreg.KEY_READ | winreg.KEY_WOW64_32KEY) as software_msc:
        nastrans = sorted([name for name in enum_keys(software_msc) if "MSC Nastran" in name and "MSC Nastran Documentation" not in name])
        if not nastrans:
            exitwitherror("Nastran.py: Can't read NASTRAN path from registry. Do you have it installed?", -1)
        latest_nastran = nastrans[-1]
        with winreg.OpenKey(software_msc, latest_nastran + "\\Latest") as latest_key:
            version = six.text_type(winreg.QueryValueEx(latest_key, None)[0])
        with winreg.OpenKey(software_msc, "{}\\{}".format(latest_nastran, version)) as nastran_key:
            return six.text_type(winreg.QueryValueEx(nastran_key, "Path")[0])


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Run Nastran on input .nas Deck')

    parser.add_argument('nas_filename', help='.nas File Name')
    # parser.add_argument('Nastran directory')
    # parser.add_argument('paramFile', help='.xml ComputedValues File Name')
    args = parser.parse_args()

    if not os.path.exists(args.nas_filename):
        print("Error: %s file not found" % nas_filename)

    NasPath = get_latest_nastran_directory()
    print("\nStarting Nastran \n")
    retcode = call([NasPath + "\\bin\\nastran.exe", "estimate", args.nas_filename], shell=True)

    if retcode == 0:
        print("Nastran Execution Successful")

    sys.exit(0)
