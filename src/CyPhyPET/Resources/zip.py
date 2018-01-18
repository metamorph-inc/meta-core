import zipfile
import sys
import io
import os
import os.path
import json
import six

path_join = os.path.join
if sys.platform == 'win32':
    def path_join(*args):
        return '\\\\?\\' + os.path.join(os.getcwd(), os.path.join(*args))

output_filename = 'source_data.zip'

if os.path.exists(output_filename):
    os.remove(output_filename)


with zipfile.ZipFile(output_filename, 'w', allowZip64=True) as z:
    parent_dir_name = os.path.basename(six.moves.getcwd())

    with io.open('mdao_config.json', 'r', encoding='utf8') as config_file:
        mdao_config = json.load(config_file)

    for name, component in six.iteritems(mdao_config['components']):
        types = {
            'excel_wrapper.excel_wrapper.ExcelWrapper': 'excelFile',
            'matlab_wrapper.MatlabWrapper': 'mFile',
            'run_mdao.python_component.PythonComponent': 'filename',
        }
        detailsKey = types.get(component.get('type'))
        if detailsKey is None:
            continue
        filename = component['details'][detailsKey]
        destFilename = name + '/' + os.path.basename(filename)
        z.write(filename, arcname=parent_dir_name + '/' + destFilename, compress_type=zipfile.ZIP_DEFLATED)
        component['details'][detailsKey] = destFilename

    with open('mdao_config.json', 'w') as config_file:
        json.dump(mdao_config, config_file, indent=4)

    os.chdir('..')
    for dirpath, dirs, files in os.walk(parent_dir_name):
        for f in files:
            if output_filename == f:
                continue
            fn = path_join(dirpath, f)
            # print fn
            z.write(fn, arcname=os.path.join(dirpath, f), compress_type=zipfile.ZIP_DEFLATED)
