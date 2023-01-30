#!python -u

import sys
import os
import os.path
import win32com.client
import gen_dir_wxi
from gen_dir_wxi import add_wix_to_path, system, download_bundle_deps, CommentedTreeBuilder, get_file_id
import gen_analysis_tool_wxi
import glob
import subprocess
import re
import itertools
import six

import xml.etree.ElementTree as ET
import xml.etree.ElementTree as ElementTree

this_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(this_dir)

os.environ['PATH'] = os.environ['PATH'].replace('"', '')


def get_nuget_packages():
    import vc_info
    from xml.etree import ElementTree
    cad_packages = ElementTree.parse(r'CAD_Installs\packages.config')
    destination_files = [r'..\bin\CAD\Creo\bin\CADCreoParametricCreateAssembly.exe',
        r'..\bin\CAD\Creo\0Readme - CreateAssembly.txt',
        r'..\bin\CAD\Creo\bin\ExtractACM-XMLfromCreoModels.exe',
        r'..\bin\CAD\Creo\bin\CADCreoParametricMetaLink.exe',
        'WixStdBA.dll',
        ]
    for filename in destination_files:
        if os.path.isfile(filename):
            os.unlink(filename)
    for package in cad_packages.findall('package'):
        svnversion = {"META.CadCreoParametricCreateAssembly": vc_info.last_cad_rev,
            "META.ExtractACM-XMLfromCreoModels": vc_info.last_cad_rev,
            "META.MDL2MGACyber": vc_info.last_mdl2mga_rev,
            "META.CADCreoParametricMetaLink": vc_info.last_cad_rev,
            "Wix.WixStdBAMod": None}[package.get('id')]
        version = package.get('version')
        if svnversion is not None:
            version = vc_info.update_version(version, svnversion())
        print("NuGet install " + package.get('id') + " " + version)
        # n.b. don't specify -ConfigFile, as it makes nuget.exe ignore %APPDATA%\NuGet\NuGet.config
        system([r'..\src\.nuget\nuget.exe', 'install', '-PreRelease', '-Version', version, package.get('id')], os.path.join(this_dir, 'CAD_Installs'))
        package_dir = r'CAD_Installs\%s.%s' % (package.get('id'), version)
        for filename in itertools.chain(glob.glob(package_dir + '/*'), glob.glob(package_dir + '/lib/*')):
            #if os.path.basename(filename) == 'svnversion':
            #    with open(os.path.join(this_dir, filename), 'rb') as svnversion:
            #        print filename + ': ' + svnversion.read()
            destination_file = [fn for fn in destination_files if os.path.basename(fn) == os.path.basename(filename)]
            if not destination_file:
                continue
            destination_file = destination_file[0]

            from win32file import CreateHardLink
            print("Linking %s to %s" % (os.path.join(this_dir, filename), os.path.join(this_dir, destination_file)))
            CreateHardLink(os.path.join(this_dir, destination_file), os.path.join(this_dir, filename))
            destination_files.remove(destination_file)

    if destination_files:
        raise Exception('Could not find files %s in NuGet packages' % repr(destination_files))

def bin_mods():
    output_filename = 'bin.wxi'
    ElementTree.register_namespace("", "http://schemas.microsoft.com/wix/2006/wi")
    tree = ElementTree.parse(output_filename, parser=ElementTree.XMLParser(target=CommentedTreeBuilder())).getroot()
    parent_map = dict((c, p) for p in tree.iter() for c in p)
    fragment = tree.findall(".//{http://schemas.microsoft.com/wix/2006/wi}Fragment")[0]
    for element in ("""<Property Id="pywin219"/>""",
            """<CustomActionRef Id="WixRemoveFoldersEx" />""",
            r"""<CustomAction Id="Setpywin219" Property="pywin219" Value="[ProgramFilesFolder]META\bin\Python311"/>""",
            """<InstallExecuteSequence>
                <Custom Action="Setpywin219" Before="WixRemoveFoldersEx" />
            </InstallExecuteSequence>"""):
        fragment.insert(0, ElementTree.fromstring(element))

    python_exe = tree.findall(r".//{http://schemas.microsoft.com/wix/2006/wi}Component[@Directory='dir_bin_Python311']/{http://schemas.microsoft.com/wix/2006/wi}File[@Source='..\bin\Python311\python.exe']")[0]
    parent_map[python_exe].insert(0, ElementTree.fromstring("""<util:RemoveFolderEx xmlns:util="http://schemas.microsoft.com/wix/UtilExtension" On="install" Property="pywin219" />"""))

    #for element in parent_map.keys():
    #    if element.tag == '{http://schemas.microsoft.com/wix/2006/wi}File':
    #        source = element.attrib['Source']
    #        import pdb
    #        pdb.set_trace()
    #        if source.startswith(r'..\bin\Python311\Lib') and source.endswith('.py'):
    #            element.attrib['Source'] = source + 'c'
    python_zip = tree.findall(r".//{http://schemas.microsoft.com/wix/2006/wi}" + "File[@Id='{}']".format(get_file_id(r"..\bin\Python311\Python311.zip")))[0]
    python_zip.attrib['Source'] = 'Python311.zip'

    ElementTree.ElementTree(tree).write(output_filename, xml_declaration=True, encoding='utf-8')

def generate_license_rtf():
    with open('../license.rtf', 'wt') as rtf:
        txt = open('../license.txt', 'rt').read()
        txt = txt.replace('\r', '')
        txt = re.sub('([^\\n])\\n(?!\\n)', '\\1 ', txt)
        txt = re.sub(r'([\\{}])', r'\\\1', txt)
        rtf.write('{\\rtf1\n')
        rtf.write(txt.replace('\n\n', '\\par\n'))
        rtf.write('\n}')


def add_vcs_defines(defines):
    def get_command_output(cmd):
        p = subprocess.Popen(cmd, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        out, err = out.decode('utf8'), err.decode('utf8')
        return out.strip()

    def get_vcsversion():
        p = subprocess.Popen("git rev-list HEAD --count".split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        out, err = out.decode('utf8'), err.decode('utf8')
        return str(int(out.strip() or '651') - 650)

    vcsversion = get_vcsversion()
    def get_vcsdescribe():
        out = get_command_output("git describe --match v*".split())
        if out:
            return out
        # if there is no tag, use v0.1.1-[revision count]-g[short hash]
        return "v0.1.1-" + vcsversion + "-g" + get_command_output("git rev-parse --short HEAD")
    def parse_describe(describe):
        m = re.match('^v(\\d+\\.\\d+\\.\\d+)((?:-[a-zA-Z_]+)?-(\\d+)-\\w+)?$', describe)
        if not m:
            raise ValueError('Invalid tag "{}". Must be in format v0.1.2 or v0.1.2-vendor'.format(describe))
        return m.groups()[0] + '.' + (m.groups()[2] or '0')

    assert parse_describe('v0.16.1') == '0.16.1.0'
    assert parse_describe('v0.16.1-21-g57742becee') == '0.16.1.21'
    assert parse_describe('v0.16.1-vendor-21-g57742becee') == '0.16.1.21'
    vcsdescription = get_vcsdescribe()
    version = parse_describe(vcsdescription)

    def get_githash():
        p = subprocess.Popen("git rev-parse --short HEAD".split(), stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        out, err = p.communicate()
        out, err = out.decode('utf8'), err.decode('utf8')
        #if p.returncode:
        #    raise subprocess.CalledProcessError(p.returncode, 'svnversion')
        return out.strip() or 'unknown'

    vcshash = get_githash()

    print("Version description: " + vcsdescription)
    print("Installer version: " + version)
    defines.append(('VERSIONSTR', version))
    defines.append(('VCSHASH', vcshash))
    defines.append(('VCSDESCRIPTION', vcsdescription))


def get_wixobj(file):
    return os.path.splitext(file)[0] + ".wixobj"


def compile_wix_source(sources, defines):
    from multiprocessing.pool import ThreadPool
    pool = ThreadPool()
    pool_exceptions = []

    def candle(source):
        try:
            arch = [ '-arch', ('x86' if source.find('x64') == -1 else 'x64') ]
            try:
                dest_mtime = os.stat(get_wixobj(source)).st_mtime
                source_mtime = os.stat(source).st_mtime
                if dest_mtime >= source_mtime:
                    pass # return
            except OSError as e:
                pass
            system(['candle', '-ext', 'WiXUtilExtension'] + ['-d' + d[0] + '=' + d[1] for d in defines ] + arch + [ '-out', get_wixobj(source), source] + ['-nologo'])
        except Exception as e:
            pool_exceptions.append(sys.exc_info())
            raise
    candle_results = pool.map_async(candle, sources, chunksize=1)
    pool.close()
    pool.join()
    if pool_exceptions:
        six.reraise(*pool_exceptions[0])
    assert candle_results.successful()


def build_msi(with_offline=False):
    source_wxs='META_x64.wxs'
    get_nuget_packages()

    generate_license_rtf()

    add_wix_to_path()

    defines = [ ('InterpreterBin', '../src/bin') ]

    def adjacent_file(file):
        return os.path.join(this_dir, file)

    gen_analysis_tool_wxi.main(r"..\analysis_tools", diskId='5')

    # gen_dir_from_vc: "explicit is better than implicit"
    #  consider: generated files are left on disk after an svn switch, and get included in an installer that shouldn't have them
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\PCC\PCC",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\isis_meta\isis_meta",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\material_library\MaterialLibraryInterface",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\meta_nrmm\meta_nrmm",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica\py_modelica",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica_exporter\py_modelica_exporter",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\cad_library\cad_library",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\run_mdao",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\testbenchexecutor",)
    gen_dir_wxi.gen_dir_from_vc(r"..\meta\DesignDataPackage\lib\python", "DesignDataPackage_python.wxi", "DesignDataPackage_python")
    #gen_dir_wxi.main(r"CAD_Installs\Proe ISIS Extensions", "Proe_ISIS_Extensions_x64.wxi", "Proe_ISIS_Extensions_x64", diskId='4')  # do not call gen_dir_from_vc, it would exclude CADCreoCreateAssembly.exe
    gen_dir_wxi.gen_dir_from_vc(r"..\meta\CyPhyML\icons",)
    gen_dir_wxi.gen_dir_from_vc(r"..\models\Validation",)

    import package_python
    bin_file_map = package_python.compileall()
    bin_file_map.update(package_python.zipall())
    gen_dir_wxi.gen_dir_from_vc(r"..\bin", diskId='3', file_map=bin_file_map)
    bin_mods()
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\PCC\PCC",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\isis_meta\isis_meta",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\meta_nrmm\meta_nrmm",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica\py_modelica",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica_exporter\py_modelica_exporter",)
    gen_dir_wxi.gen_dir_from_vc(r"..\meta\DesignDataPackage\lib\python", "DesignDataPackage_python.wxi", "DesignDataPackage_python")

    add_vcs_defines(defines)

    sourcedir = os.path.relpath(this_dir) + '/'

    import glob
    sources_all = glob.glob(sourcedir + '*.wxi') + glob.glob(sourcedir + source_wxs)
    sources = []
    include_wxis = []

    # For each each ComponentGroupRef in "source_wxs" and "analysis_tools.wxi",
    # add its corresponding file to "include_wxis"
    for wxs in glob.glob(sourcedir + source_wxs) + glob.glob(sourcedir + 'analysis_tools.wxi'):
        print('Processing WXS: ' + wxs)
        tree = ET.parse(wxs)
        root = tree.getroot()
        # print root
        all_nodes = root.findall('.//')
        for node in all_nodes:
            if node.tag == '{http://schemas.microsoft.com/wix/2006/wi}ComponentGroupRef':
                include_wxis.append(node.attrib['Id'] + '.wxi')
                include_wxis.append(node.attrib['Id'] + '_x64.wxi')
            if node.tag == '{http://schemas.microsoft.com/wix/2006/wi}ComponentRef':
                include_wxis.append(node.attrib['Id'].rsplit(".", 1)[0] + '.wxi')
                include_wxis.append(node.attrib['Id'].rsplit(".", 1)[0] + '_x64.wxi')

    # For each file in include_wxis, check for ComponentGroupRef and ComponentRef.
    # Add any that you find
    index = 0
    while index < len(include_wxis):
        wxi = include_wxis[index]
        index += 1

        if not os.path.exists(wxi):
            continue

        tree = ET.parse(wxi)
        root = tree.getroot()

        all_nodes = root.findall('.//')
        for node in all_nodes:
            if node.tag == '{http://schemas.microsoft.com/wix/2006/wi}ComponentGroupRef':
                include_wxis.append(node.attrib['Id'] + '.wxi')
                include_wxis.append(node.attrib['Id'] + '_x64.wxi')
            if node.tag == '{http://schemas.microsoft.com/wix/2006/wi}ComponentRef':
                include_wxis.append(node.attrib['Id'].rsplit(".", 1)[0] + '.wxi')
                include_wxis.append(node.attrib['Id'].rsplit(".", 1)[0] + '_x64.wxi')

    include_wxis = set((wxi.lower() for wxi in include_wxis))
    sources = [source for source in sources_all if (os.path.basename(source).lower() in include_wxis)]
    sources.append(source_wxs)

    if len(sources) == 0:
        raise Exception("0 sources found in " + sourcedir)

    def get_mta_versions(mta_file):
        import uuid
        metaproject = win32com.client.Dispatch("MGA.MgaMetaProject")
        metaproject.Open('MGA=' + mta_file)
        try:
            return ("{" + str(uuid.UUID(bytes_le=metaproject.GUID.tobytes())).upper() + "}", metaproject.Version)
        finally:
            metaproject.Close()

    cyphy_versions = get_mta_versions(adjacent_file('../generated/CyPhyML/models/CyPhyML.mta'))
    defines.append(('GUIDSTRCYPHYML', cyphy_versions[0]))
    defines.append(('VERSIONSTRCYPHYML', cyphy_versions[1]))

    compile_wix_source(sources, defines)

    # ignore warning 1055, ICE82 from VC10 merge modules
    # ICE69: Mismatched component reference. Entry 'reg491FAFEB7F990D99C4A4D719B2A95253' of the Registry table belongs to component 'CyPhySoT.dll'. However, the formatted string in column 'Value' references file 'CyPhySoT.ico' which belongs to component 'CyPhySoT.ico'
    # ICE60: The file fil_5b64d789d9ad5473bc580ea7258a0fac is not a Font, and its version is not a companion file reference. It should have a language specified in the Language column.
    pool_exceptions = []
    if source_wxs.startswith("META"):
        def download():
            try:
                download_bundle_deps('META_bundle_x64.wxs')
            except Exception as e:
                pool_exceptions.append(sys.exc_info())

        import threading
        download_thread = threading.Thread(target=download)
        download_thread.start()

        import datetime
        starttime = datetime.datetime.now()
	# '-sice:ICE03' is generated by some merge modules
        system(['light', '-sice:ICE60', '-sice:ICE69', '-sice:ICE57',
        #'-sw1055', '-sice:ICE03', '-sice:ICE82', 
         '-ext', 'WixNetFxExtension', '-ext', 'WixUIExtension', '-ext', 'WixUtilExtension',
            # '-cc', os.path.join(this_dir, 'cab_cache'), '-reusecab', # we were getting errors during installation relating to corrupted cab files => disable cab cache
            '-o', os.path.splitext(source_wxs)[0] + ".msi"] + [get_wixobj(file) for file in sources])


        download_thread.join()
        if pool_exceptions:
            six.reraise(*pool_exceptions[0])
        system('candle.exe META_bundle_x64.wxs -ext WixBalExtension -ext WixUtilExtension -ext WixDependencyExtension'.split() + ['-d' + d[0] + '=' + d[1] for d in defines])
        system('candle.exe META_bundle_ba.wxi -ext WixBalExtension -ext WixUtilExtension -ext WixDependencyExtension'.split() + ['-d' + d[0] + '=' + d[1] for d in defines])
        system('light.exe -o META_bundle_x64.exe META_bundle_ba.wixobj META_bundle_x64.wixobj -ext WixBalExtension -ext WixUtilExtension -ext WixDependencyExtension -ext WixNetFxExtension'.split())

        print("elapsed time: %d seconds" % (datetime.datetime.now() - starttime).seconds)
    else:
        msm_output = os.path.splitext(source_wxs)[0] + ".msm"
        system(['light', '-ext', 'WixUtilExtension', '-o', msm_output] + [get_wixobj(file) for file in sources])


class MSBuildErrorWriter(object):
    def write(self, d):
        sys.stderr.write("error: ")
        sys.stderr.write(d)

if __name__ == '__main__':
    os.chdir(this_dir)
    build_msi()

    import traceback
    try:
        build_msi()
    except:
        traceback.print_exc(None, MSBuildErrorWriter())
        sys.exit(2)
