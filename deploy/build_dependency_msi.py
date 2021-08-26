from build_msi import generate_license_rtf, add_wix_to_path, add_vcs_defines, system, compile_wix_source, get_wixobj
from gen_dir_wxi import WixProcessingInstructionHandler

if __name__ == '__main__':
    generate_license_rtf()

    add_wix_to_path()

    from xml.etree import ElementTree

    source_wxs = 'META_dependencies_x64.wxs'
    defines = WixProcessingInstructionHandler()
    defines.parse(source_wxs)
    parsed_source = ElementTree.parse(source_wxs)
    product_version = defines.eval_vars(parsed_source.find('.//{http://schemas.microsoft.com/wix/2006/wi}Product').attrib['Version'])
    sources = [source_wxs]
    defines = []
    add_vcs_defines(defines)

    compile_wix_source(sources, defines)
    output_filename = "META_dependencies_v{}.msi".format(product_version)
    system(['light', '-sw1055', '-sice:ICE03', '-sice:ICE82',
        '-ext', 'WixNetFxExtension', '-ext', 'WixUIExtension', '-ext', 'WixUtilExtension',
        '-o', output_filename] + ['UdmDll_VS10.wixlib', 'UdmDll_VC11_x64.wixlib', 'UdmDll_VC14.wixlib']
        + [get_wixobj(file) for file in sources])

    print('Created {}'.format(output_filename))
