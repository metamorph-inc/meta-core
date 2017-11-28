"""Modify GME msi to open .mga and .xme files with 64bit GME by default."""

import shutil
import msilib


def gme_x64_open(input):
    msi = msilib.OpenDatabase(input, msilib.MSIDBOPEN_DIRECT)

    view = msi.OpenView("SELECT `Registry`, `Root`, `Key`, `Name`, `Value`, `Component_` FROM `Registry`")
    view.Execute(None)
    mods = 0

    while True:
        try:
            record = view.Fetch()
        except msilib.MSIError as e:
            # 0x103 == ERROR_NO_MORE_ITEMS 259
            if e.args[0] == 'unknown error 103':
                break
            raise
        if record.GetString(3) in (r"GME.mgafile\shell\open\command", r"GME.xmefile\shell\open\command"):
            record.SetString(5, "\"[#GME.exex64]\" \"%1\"")
            record.SetString(6, "GME.exex64")
            view.Modify(msilib.MSIMODIFY_UPDATE, record)
            mods = mods + 1

    if mods != 2:
        pass
        # raise ValueError('Expected to make 2 modifications, only made {}'.format(mods))
    view.Close()
    if mods != 0:
        msi.Commit()


if __name__ == '__main__':
    input = "GME.msi"
    shutil.copyfile(r"C:\Users\kevin\Downloads\GME_x64-17.12.6.msi", "GME.msi")
    gme_x64_open('GME.msi')
