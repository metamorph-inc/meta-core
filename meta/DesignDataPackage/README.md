# DesignDataPackage

`EMF_Metamodel\model\AVM.ecorediag` defines the file format for .acm and .adm files.

## HOWTO: make changes to the DesignDataPackage

1. Open in eclipse-modeling-kepler-SR2-win32-x86_64  
    File>Import>General>Existing Projects into Workspace>`meta\DesignDataPackage\EMF_MetaModel`  
1. Make mods  
    Window>Show View>Project Explorer  ; AVM.ecorediag
1. Right-click canvas > File > Save as image file > `meta\DesignDataPackage\doc\ClassDiagrams\same_filename.pdf`  
1. avm.genmodel right-click > Reload, click click click click  
1. avm.genmodel right-click > Export Model, XML Schema, click click click click  
1. `cd META\meta\DesignDataPackage`  
1. `cmd /c make.bat`  
1. Use diff tool, remove some stuff

        git diff -w --no-color . | git apply --cached --ignore-whitespace
        git checkout -- .
        git reset HEAD -- .
        git apply fix_codegen.patch

TODO: apply gitattributes for EOL on xsd in EMF_MetaModel

## HOWTO: generate Python and C# code

Run `make.bat`

In `lib/python/avm/cyber/__init__.py`, change `import modelica as _ImportedBinding__modelica` to `import avm.modelica as _ImportedBinding__modelica`

## HOWTO: generate Java code

    cmd /c make.bat
    set xjc_opts=-Djavax.xml.accessExternalSchema=file
    mkdir java
    tools\jaxb-ri-2.2.6\bin\xjc.bat -d java schema
