#!python -u

import sys
import os
import os.path
import win32com.client
import gen_dir_wxi
from gen_dir_wxi import add_wix_to_path, system, download_bundle_deps
import gen_analysis_tool_wxi
import glob
import subprocess
import re

import xml.etree.ElementTree as ET

this_dir = os.path.dirname(os.path.abspath(__file__))
os.chdir(this_dir)

os.environ['PATH'] = os.environ['PATH'].replace('"', '')


def get_nuget_packages():
    import vc_info
    from xml.etree import ElementTree
    cad_packages = ElementTree.parse(r'CAD_Installs\packages.config')
    destination_files = [r'CAD_Installs\Proe ISIS Extensions\bin\CADCreoParametricCreateAssembly.exe',
        r'CAD_Installs\Proe ISIS Extensions\0Readme - CreateAssembly.txt',
        r'CAD_Installs\Proe ISIS Extensions\bin\ExtractACM-XMLfromCreoModels.exe',
        r'CAD_Installs\Proe ISIS Extensions\bin\CADCreoParametricMetaLink.exe',
        ]
    for filename in destination_files:
        if os.path.isfile(filename):
            os.unlink(filename)
    for package in cad_packages.findall('package'):
        svnversion = {"META.CadCreoParametricCreateAssembly": vc_info.last_cad_rev,
            "META.ExtractACM-XMLfromCreoModels": vc_info.last_cad_rev,
            "META.MDL2MGACyber": vc_info.last_mdl2mga_rev,
            "META.CADCreoParametricMetaLink": vc_info.last_cad_rev, }[package.get('id')]()
        version = package.get('version')
        version = vc_info.update_version(version, svnversion)
        print "NuGet install " + package.get('id') + " " + version
        # n.b. don't specify -ConfigFile, as it makes nuget.exe ignore %APPDATA%\NuGet\NuGet.config
        system([r'..\src\.nuget\nuget.exe', 'install', '-PreRelease', '-Version', version, package.get('id')], os.path.join(this_dir, 'CAD_Installs'))
        package_dir = r'CAD_Installs\%s.%s' % (package.get('id'), version)
        for filename in glob.glob(package_dir + '/*'):
            # if os.path.basename(filename) == 'svnversion':
            #    with open(os.path.join(this_dir, filename), 'rb') as svnversion:
            #        print filename + ': ' + svnversion.read()
            destination_file = [fn for fn in destination_files if os.path.basename(fn) == os.path.basename(filename)]
            if not destination_file:
                continue
            destination_file = destination_file[0]

            from win32file import CreateHardLink
            print "Linking %s to %s" % (os.path.join(this_dir, filename), os.path.join(this_dir, destination_file))
            CreateHardLink(os.path.join(this_dir, destination_file), os.path.join(this_dir, filename))
            destination_files.remove(destination_file)

    if destination_files:
        raise Exception('Could not find files %s in NuGet packages' % repr(destination_files))


def generate_license_rtf():
    with open('../license.rtf', 'wb') as rtf:
        txt = open('../license.txt').read()
        txt = re.sub('\\n(?!\\n)', '', txt.replace('\r', ''))
        rtf.write('{\\rtf1\n')
        rtf.write(txt.replace('\r', '').replace('\n', '\\par\n'))
        rtf.write('\n}')


def build_msi():
    get_nuget_packages()

    generate_license_rtf()

    add_wix_to_path()

    def get_wixobj(file):
        return os.path.splitext(file)[0] + ".wixobj"

    def adjacent_file(file):
        return os.path.join(os.path.dirname(__file__), file)

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
    gen_dir_wxi.main(r"CAD_Installs\Proe ISIS Extensions", "Proe_ISIS_Extensions_x64.wxi", "Proe_ISIS_Extensions_x64", diskId='4')  # do not call gen_dir_from_vc, it would exclude CADCreoCreateAssembly.exe
    gen_dir_wxi.gen_dir_from_vc(r"..\WebGME",)
    gen_dir_wxi.gen_dir_from_vc(r"..\meta\CyPhyML\icons",)
    gen_dir_wxi.gen_dir_from_vc(r"..\models\MassSpringDamper",)
    gen_dir_wxi.gen_dir_from_vc(r"..\models\Validation",)
    gen_dir_wxi.gen_dir_from_vc(r"..\bin", diskId='3')
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\PCC\PCC",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\isis_meta\isis_meta",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\meta_nrmm\meta_nrmm",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica\py_modelica",)
    gen_dir_wxi.gen_dir_from_vc(r"..\src\Python27Packages\py_modelica_exporter\py_modelica_exporter",)
    gen_dir_wxi.gen_dir_from_vc(r"..\meta\DesignDataPackage\lib\python", "DesignDataPackage_python.wxi", "DesignDataPackage_python")

    def get_svnversion():
        p = subprocess.Popen("git rev-list HEAD --count".split(), stdout=subprocess.PIPE)
        out, err = p.communicate()
        return out.strip() or '5'
        # import subprocess
        # p = subprocess.Popen(['svnversion', '-n', adjacent_file('..')], stdout=subprocess.PIPE)
        # out, err = p.communicate()
        # if p.returncode:
        #     raise subprocess.CalledProcessError(p.returncode, 'svnversion')
        # return out
    svnversion = get_svnversion()

    print "SVN version: " + str(get_svnversion())
    sourcedir = os.path.relpath(this_dir) + '/'

    def get_gitversion():
        p = subprocess.Popen("git rev-parse --short HEAD".split(), stdout=subprocess.PIPE)
        out, err = p.communicate()
        # if p.returncode:
        #     raise subprocess.CalledProcessError(p.returncode, 'svnversion')
        return out.strip() or 'unknown'

    gitversion = get_gitversion()

    import glob
    if len(sys.argv[1:]) > 0:
        source_wxs = sys.argv[1]
    else:
        source_wxs = 'META_x64.wxs'
    sources_all = glob.glob(sourcedir + '*.wxi') + glob.glob(sourcedir + source_wxs)
    sources = []
    include_wxis = []

    # For each each ComponentGroupRef in "source_wxs" and "analysis_tools.wxi",
    # add its corresponding file to "include_wxis"
    for wxs in glob.glob(sourcedir + source_wxs) + glob.glob(sourcedir + 'analysis_tools.wxi'):
        print 'Processing WXS: ' + wxs
        tree = ET.parse(wxs)
        root = tree.getroot()
        # print root
        all_nodes = root.findall('.//')
        for node in all_nodes:
            if node.tag == '{http://schemas.microsoft.com/wix/2006/wi}ComponentGroupRef':
                include_wxis.append(node.attrib['Id'] + '.wxi')
                include_wxis.append(node.attrib['Id'] + '_x64.wxi')
                if 'Proe' in node.attrib['Id'] + '_x64.wxi':
                    print node.attrib['Id'] + '_x64.wxi'
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

    sources = [source for source in sources_all if (os.path.basename(source) in include_wxis)]
    sources.append(source_wxs)

    if len(sources) == 0:
        raise Exception("0 sources found in " + sourcedir)

    defines = [('InterpreterBin', '../src/bin')]

    def get_mta_versions(mta_file):
        import uuid
        metaproject = win32com.client.Dispatch("MGA.MgaMetaProject")
        metaproject.Open('MGA=' + mta_file)
        try:
            return ("{" + str(uuid.UUID(bytes_le=metaproject.GUID)).upper() + "}", metaproject.Version)
        finally:
            metaproject.Close()

    cyphy_versions = get_mta_versions(adjacent_file('../generated/CyPhyML/models/CyPhyML.mta'))
    defines.append(('GUIDSTRCYPHYML', cyphy_versions[0]))
    defines.append(('VERSIONSTRCYPHYML', cyphy_versions[1]))

    version = '14.13.'
    if 'M' in svnversion:
        version = version + '1'
    else:
        # this will crash for switched or sparse checkouts
        version = version + str(int(svnversion))
    print 'Installer version: ' + version
    defines.append(('VERSIONSTR', version))
    defines.append(('SVNVERSION', svnversion))
    defines.append(('GITVERSION', gitversion))

    from multiprocessing.pool import ThreadPool
    pool = ThreadPool()
    pool_exceptions = []

    def candle(source):
        try:
            arch = ['-arch', ('x86' if source.find('x64') == -1 else 'x64')]
            system(['candle', '-ext', 'WiXUtilExtension'] + ['-d' + d[0] + '=' + d[1] for d in defines] + arch + ['-out', get_wixobj(source), source] + ['-nologo'])
        except Exception as e:
            pool_exceptions.append(e)
            raise
    candle_results = pool.map_async(candle, sources, chunksize=1)
    pool.close()
    pool.join()
    if pool_exceptions:
        raise pool_exceptions[0]
    assert candle_results.successful()

    # ignore warning 1055, ICE82 from VC10 merge modules
    # ICE69: Mismatched component reference. Entry 'reg491FAFEB7F990D99C4A4D719B2A95253' of the Registry table belongs to component 'CyPhySoT.dll'. However, the formatted string in column 'Value' references file 'CyPhySoT.ico' which belongs to component 'CyPhySoT.ico'
    # ICE60: The file fil_5b64d789d9ad5473bc580ea7258a0fac is not a Font, and its version is not a companion file reference. It should have a language specified in the Language column.
    if source_wxs.startswith("META"):
        def download():
            try:
                download_bundle_deps('META_bundle_x64.wxs')
            except Exception as e:
                pool_exceptions.append(e)

        import threading
        download_thread = threading.Thread(target=download)
        download_thread.start()

        import datetime
        starttime = datetime.datetime.now()
        system(['light', '-sw1055', '-sice:ICE82', '-sice:ICE57', '-sice:ICE60', '-sice:ICE69', '-ext', 'WixNetFxExtension', '-ext', 'WixUIExtension', '-ext', 'WixUtilExtension',
            # '-cc', os.path.join(this_dir, 'cab_cache'), '-reusecab', # we were getting errors during installation relating to corrupted cab files => disable cab cache
            # udm.pyd depends on UdmDll_VC10
            '-o', os.path.splitext(source_wxs)[0] + ".msi"] + [get_wixobj(file) for file in sources] + ['UdmDll_VS10.wixlib', 'UdmDll_VC11_x64.wixlib', 'UdmDll_VC14.wixlib'])

        download_thread.join()
        if pool_exceptions:
            raise pool_exceptions[0]
        system('candle.exe META_bundle_x64.wxs -ext WixBalExtension -ext WixUtilExtension -ext WixDependencyExtension'.split() + ['-d' + d[0] + '=' + d[1] for d in defines])
        system('light.exe META_bundle_x64.wixobj -ext WixBalExtension -ext WixUtilExtension -ext WixDependencyExtension -ext WixNetFxExtension'.split())

        print "elapsed time: %d seconds" % (datetime.datetime.now() - starttime).seconds
    else:
        msm_output = os.path.splitext(source_wxs)[0] + ".msm"
        system(['light', '-ext', 'WixUtilExtension', '-o', msm_output] + [get_wixobj(file) for file in sources])


class MSBuildErrorWriter(object):
    def write(self, d):
        sys.stderr.write("error: ")
        sys.stderr.write(d)

if __name__ == '__main__':
    os.chdir(this_dir)
    import traceback
    try:
        build_msi()
    except:
        traceback.print_exc(None, MSBuildErrorWriter())
        sys.exit(2)
