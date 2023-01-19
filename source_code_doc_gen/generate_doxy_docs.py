import os
import shutil
import subprocess
    
# Use 64-bit if possible
dox_exe = os.path.abspath(os.path.join('..', '3rdParty', 'doxygen', 'doxygen-1.8.5.windows.x64.bin', 'doxygen.exe'))
# If not switch to 32-bit
#dox_exe = os.path.abspath(os.path.join('..', '3rdParty', 'doxygen', 'doxygen-1.8.5.windows.bin', 'doxygen.exe'))

selected_projects = [] # All found projects with Doxyfile will be generated if this list empty
paths_given = False
if os.path.isfile('selected_projects.txt'):
    with open('selected_projects.txt', 'r') as f_in:
        selected_projects = [sp.strip() for sp in f_in.readlines() if sp]

if selected_projects:
    print('Selected projects : {0}'.format(selected_projects))
else:
    print('Selected projects empty, all projects with Doxyfiles will be considered.')
# Custom css files will be put in html-directory and overwrite if already there
css_files = ['doxygen.css', 'tabs.css'] 

root_dir = os.path.dirname(os.path.abspath(__file__))
doxy_dirs = []

print('Searching trunk for "Doxyfile"s...')
for dir_name, d_names, f_names in os.walk('..'):
    for fn in f_names:
        if fn == 'Doxyfile':
            project_name = os.path.basename(dir_name)
            dir_path = os.path.abspath(dir_name)
            if project_name in selected_projects or not selected_projects:
                print('Found file for {0}'.format(project_name))
                print('Adding directory {0}'.format(dir_path))
                doxy_dirs.append(dir_path)

doc_dirs = []
for dd in doxy_dirs:
    os.chdir(dd)
    try:
        print('Calling doxygen inside {0}...'.format(dd))
        subprocess.call(dox_exe)
        doc_dir = os.path.join(dd, 'doc')
        if os.path.exists(doc_dir):
            doc_dirs.append(doc_dir)
        else:
            print('Could not find ' + doc_dir)
            print('Please configure the Doxyfile to output files at ./doc')
    except:
        print('Failed running doxygen on project at ' + dd)
    finally:
        os.chdir(root_dir)

for dd in doc_dirs:
    project_name = os.path.basename(os.path.abspath(os.path.join(dd, '..')))
    dst = os.path.join(root_dir, project_name)
    print(dst)
    if os.path.exists(dst):
        shutil.rmtree(dst)
    shutil.copytree(dd, dst)
    for css_file in css_files:
        custom_css = os.path.join(dst, 'html', css_file)
        if os.path.isfile(custom_css):
            print('About to replace ' + custom_css)
            os.remove(custom_css)
            shutil.copy(css_file, custom_css)
            print('Moved file!')
