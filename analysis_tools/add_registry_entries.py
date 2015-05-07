""" Adds registry entries on developer's machine. Emulating what the installer will do. """

import sys
import os
import json
from _winreg import *


def register_tool(tool):
    reg_path = "SOFTWARE\\META\\AnalysisTools\\%s\\" % (tool['name'])
    reg_key = "InstallLocation"
    reg_value = "C:\\sadasfa\\afasf"

    with CreateKeyEx(HKEY_LOCAL_MACHINE, reg_path, 0, KEY_ALL_ACCESS) as reg:
        SetValueEx(reg, 'InstallLocation', 0, REG_SZ, tool['InstallLocation'])
        SetValueEx(reg, 'Version', 0, REG_SZ, tool['version'])
        SetValueEx(reg, 'OutputDirectory', 0, REG_SZ, tool['outputDirectory'])
        SetValueEx(reg, 'RunCommand', 0, REG_SZ, tool['runCommand'])
        SetValueEx(reg, 'RequiredInterpreter', 0, REG_SZ, tool['requiredInterpreter'])

    print ' {0} is registered with parameters {1}'.format(tool['name'], tool)


def get_tool(tool_dir):
    tools = []

    with open(os.path.join(tool_dir, 'analysis_tool.manifest.json'), 'r') as f_p:
        manifest = json.load(f_p)
        for tool_name in manifest:
            tool = {}
            tool['InstallLocation'] = os.path.abspath(tool_dir)
            tool.update(manifest[tool_name])
            tool['name'] = tool_name
            tools.append(tool)
            
    return tools


def main():
    # get all analysis tools
    analysis_tools_dir = os.path.abspath(os.path.join(os.path.dirname(__file__)))

    print 'Registering analysis tools from {0}'.format(analysis_tools_dir)

    directories = [os.path.join(analysis_tools_dir, name) for name in os.listdir(analysis_tools_dir) if os.path.isdir(os.path.join(analysis_tools_dir, name)) and not name == '.svn']
    tools = []
    for tool_dir in directories:
        tools += get_tool(tool_dir)

    for tool in tools:
        register_tool(tool)

    print 'Analysis tool registration is done.'

if __name__ == '__main__':
    sys.exit(main())