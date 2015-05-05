from MaterialLibraryInterface import LibraryManager
import getpass
import uuid
import ctypes
import os

if __name__ == "__main__": #run only if main
    #get path to public documents
    path = ctypes.c_wchar_p(chr(0x00) * 256)
    FOLDERID_Documents = ctypes.c_char_p(uuid.UUID("ED4824AF-DCE4-45A8-81E2-FC7965083634").bytes_le)
    ctypes.windll.shell32.SHGetKnownFolderPath(FOLDERID_Documents, 0, None, ctypes.byref(path))
    libraryLocation = os.path.join(path.value, "META Documents", "MaterialLibrary", "material_library.json")
    
    library_manager = LibraryManager(libraryLocation)
    username = raw_input("Username: ")
    #We use getpass so that we don't echo the password to the screen
    password = getpass.getpass("Password: ")
    matList = library_manager.checkVersion(username, password)