from distutils.core import setup

import sys
import os.path
sys.path.append(os.path.join(os.path.dirname(__file__), '.'))

setup(name='AVMDDPLib', version='2.0', author='Adam Nagel', author_email='adam@isis.vanderbilt.edu',
 url='https://svn.isis.vanderbilt.edu/META/branches/T-META-947/meta/DesignDataPackage/lib/python',
 packages=['avm','avm.modelica','avm.cad','avm.manufacturing','avm.cyber','iFAB'],
 package_dir={'avm': 'avm', 'iFAB':'iFAB'})
