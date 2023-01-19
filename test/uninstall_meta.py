import subprocess
import update_meta_tools

if __name__ == '__main__':
    META_install = update_meta_tools.query_META_install()

    if 'UninstallString' in META_install:
        print(META_install['DisplayVersion'])
        print(META_install['UninstallString'] + ' /qb')
        print('Uninstalling old version of META tool chain ...')
        subprocess.call(META_install['UninstallString'] + ' /qb', shell=True)
    else:
        print("META is not installed")

