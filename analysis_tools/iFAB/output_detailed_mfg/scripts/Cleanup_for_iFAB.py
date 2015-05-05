import os
import re
import shutil

if __name__ == "__main__":
    # Remove any CREO files from the root
    prtRegex = re.compile('.*.prt.[0-9]+', flags=re.I)
    asmRegex = re.compile('.*.asm.[0-9]+', flags=re.I)

    for file in os.listdir("."):
        if prtRegex.match(file) or asmRegex.match(file):
            os.remove(file)

    # Removes other files that tend to appear in submissions
    if os.path.exists("source_data.zip"):
        os.remove("source_data.zip")

    if os.path.exists("workspace.zip"):
        os.remove("workspace.zip")

    # Removes the AP203_E2_SINGLE_FILE directory
    if os.path.exists("AP203_E2_SINGLE_FILE"):
        shutil.rmtree("AP203_E2_SINGLE_FILE")

    # Remove the auxiliary CAD directory
    if os.path.exists("Cad_Auxiliary_Directory"):
        shutil.rmtree("Cad_Auxiliary_Directory")

    # Removes the STL directories
    if os.path.exists("STL_ASCII"):
        shutil.rmtree("STL_ASCII")

    if os.path.exists("STL_BINARY"):
        shutil.rmtree("STL_BINARY")

    # Remove the components directory
    if os.path.exists("components"):
        shutil.rmtree("components")