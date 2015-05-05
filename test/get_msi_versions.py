import os
import os.path
import sys
import update_meta_tools

if __name__ == '__main__':
    for dir in sys.argv:
        for root, dirs, files in os.walk(dir):
            for filename in (fn for fn in files if fn.endswith('.msi')):
                print os.path.join(root, filename),
                try:
                    print update_meta_tools.GetMsiProperty(os.path.join(root, filename), "ProductVersion")
                except:
                    print "[error]"
