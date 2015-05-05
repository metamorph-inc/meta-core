import shutil
import os
import sys 
dir_list = ['dashboard', 'designs', 'design-space', 'results', 'test-benches' ,'tmp', 'stats']
    
file_list = ['index.html', 'manifest.project.json', 'export_for_dashboard_scoring.py']

def main():

    root = os.getcwd()
    for (path, dirs, files) in os.walk(root):
        for dir in dirs:
            if dir in dir_list:
                try:
                    shutil.rmtree(dir)
                    print 'Entire Directory deleted : {0}'.format(dir)
                except WindowsError as msg:
                    print '\n{0}\n'.format(str(msg))
                    raw_input("Press enter to continue...")
        for file in files:
            if file in file_list:
                try:
                    os.remove(file)
                    print 'File deleted : {0}'.format(file)
                except WindowsError as msg:
                    print '\n{0}\n'.format(str(msg))
                    raw_input("Press enter to continue...")
        break

if __name__ == '__main__':
    print "This script will remove files :\n"
    for f in file_list:
        print "{0}".format(f)
    print "\nand entire directories :\n"
    for d in dir_list:
        print "{0}".format(d)
    do_it = raw_input("\nWould you liked to do this clean up? [y] or n : ")
    if do_it.strip() == 'y' or do_it.strip() == '':
        main()
        raw_input("\nDone! (Hit any key to close)")
        os.exit(0)
            


