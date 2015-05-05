
3/7/2012 Starting with CADCreoParametricCreateAssembly version 1.2, CodeSynthesis was removed.  It was replaced with UDM.


9/19/2011
CodeSynthesis  - Used in CADCreoParametricCreateAssembly.exe versions v1.1.1.0 through v1.1.1... 

Prerequisites: 
-------------
To run “xsd cxx-tree” , XSD: XML Data Binding for C++ (  http://www.codesynthesis.com/products/xsd/ ) must be installed on your machine.

License:
--------
See the .\License directory.

************************************************************
Build AssemblyInterface.hxx and AssemblyInterface.cxx Files:
************************************************************

xsd  cxx-tree --sloc-limit 3000  --proprietary-license --show-sloc AssemblyInterface.xsd


// --show-sloc 
// did not use wchar_t
// xsd cxx-tree  --sloc-limit 3000  --char-type wchar_t --show-sloc AssemblyInterface.xsd


AssemblyInterface.hxx and AssemblyInterface.cxx File Location:
-------------------------------------------------------------
The generated .hxx and .cxx files are stored in the CADCreoParametricCreateAssembly
directory.


************************************************************
Build CADMetrics.hxx and CADMetrics.cxx Files:
************************************************************

xsd  cxx-tree --sloc-limit 5000 --show-sloc --proprietary-license --generate-serialization --namespace-map =CADMetrics CADMetrics.xsd


// --show-sloc 
// did not use wchar_t
// xsd cxx-tree  --sloc-limit 50000  --char-type wchar_t  CADMetrics.xsd


CADMetrics.hxx and CADMetrics.cxx File Location:
-------------------------------------------------------------
The generated .hxx and .cxx files are stored in the CADCreoParametricCreateAssembly
directory.



Libraries:
---------
 x32
  Release C:\Program Files (x86)\CodeSynthesis XSD 3.3\lib\vc-9.0\xerces-c_3.lib
  Debug C:\Program Files (x86)\CodeSynthesis XSD 3.3\lib\vc-9.0\xerces-c_3D.lib

x64
   Release  C:\Program Files (x86)\CodeSynthesis XSD 3.3\lib64\vc-9.0\xerces-c_3.lib
   Debug     C:\Program Files (x86)\CodeSynthesis XSD 3.3\lib64\vc-9.0\xerces-c_3D.lib
