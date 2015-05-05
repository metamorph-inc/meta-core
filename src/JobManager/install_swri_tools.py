import os
import os.path
import sys
import shutil
import urllib
import posixpath
import subprocess
import tempfile
import argparse

_tools_path = None

def uninstall():
    orig_uninstall = _tools_path + r"Uninstall.exe"
    if os.path.isfile(orig_uninstall):
        # nullsoft uninstaller exits immediately so it can remove itself. Copy first, then use the magic incantation
        uninstall = os.path.join(tempfile.gettempdir(), "nsis_uninstall.exe")
        shutil.copyfile(orig_uninstall, uninstall)

        subprocess.check_call([uninstall, '/S', '_?=' + _tools_path])
    
    if os.path.exists(_tools_path):
        shutil.rmtree(_tools_path)
    # TODO C:\Program Files\LSTC\LS-PrePost\4.1-X64

def install(url):
    urllib.urlretrieve(url, posixpath.basename(url))

    subprocess.check_call([posixpath.basename(url), "/S"])
  
if __name__ == '__main__':
    parser = argparse.ArgumentParser(prog=os.path.basename(sys.argv[0]))
    parser.add_argument("install_path", help='Path that the installer installs to')
    parser.add_argument("installer_url", help='URL to new installer')
    args = vars(parser.parse_args())
    _tools_path = args["install_path"]
    _tools_path = _tools_path.rstrip("\\/") + "\\"
    uninstall()
    install(args["installer_url"])
