"""
This script will go through all folders from where it is replaced and
replace all occurrences of file_to_replace with the file_to_replace directly
in this directory.
"""

file_to_replace = 'post_processing_class.py'

import os
import shutil
pps = []
for root, dirs, files in os.walk('.'):
    if root == '.':
        continue
    elif file_to_replace in files:
        pps.append(os.path.join(root, file_to_replace))

for pp in pps:
    os.remove(pp)
    print 'Removed :: ' + pp
    shutil.copy(file_to_replace, pp)
    print ':: Copied'

print 'Number of replaced files :: {0}'.format(len(pps))