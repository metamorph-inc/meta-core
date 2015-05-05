7/5/2011  v.1.1.0.0
CodeSynthesis - Used in versions  CADCreoParametricComputeMetrics.exe  v.1.1.0.0 through ??? 

Prerequisites: 
-------------
To run “xsd cxx-tree” , XSD: XML Data Binding for C++ (  http://www.codesynthesis.com/products/xsd/ ) must be installed on your machine.

License:
--------
See the .\License directory.


Build ComputeMetricsXML.hxx and ComputeMetricsXML.cxx Files:
-----------------------------------------------------------

xsd  cxx-tree --sloc-limit 2000  --proprietary-license --show-sloc ComputeMetricsXML.xsd


// --show-sloc 
// did not use wchar_t
// xsd cxx-tree  --sloc-limit 2000  --char-type wchar_t  --show-sloc ComputeMetricsXML.xsd


ComputeMetricsXML.hxx and ComputeMetricsXML.cxx File Location:
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
