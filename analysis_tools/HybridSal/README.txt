README for installing on Windows
================================

Cyber Composition Verification with HybridSal


Requirements:
-------------

 * Java 7 (JRE) -- install according to online instructions
 * SAL 3.3 for Cygwin from http://sal.csl.sri.com

optional:

 * Google Chrome (if using the Dashboard to display results)
 * MATLAB (if using Simulink models and generation of input files from counter examples is desired)


Caveat:
-------

For most of the system parts, make sure that there are no spaces in the paths where you install tools or save models.  In addition, make sure that the files also reside on a local drive and not a mounted, network share.


Install SAL 3.3:
-------------------

First, install Cygwin (32-bit) -- note: 64-bit is not working!
 1. Get 'setup-x86.exe' from http://cygwin.com and save it, for example, to your Desktop
 2. Run the basic installation by opening the saved file; this takes a while!
 3. Open a Cygwin Terminal (with shortcut that the initial installation created) and type 'pwd' to look at the path; make sure that there are no spaces in the path!

Then, install SAL 3.3:
 1. Get the current SAL for Cygwin distribution from http://sal.csl.sri.com/download.shtml; you will have to accept the license terms first. Save under your Cygwin home directory.
 2. Extract the downloaded file sal-3.3-bin-i686-pc-cygwin.tar.gz in a directory of your choice (for example 'bin/'), then run install.sh there:
   $ mkdir bin
   $ cd bin
   $ tar xzf ../sal-3.3-bin-i686-pc-cygwin.tar.gz
   $ cd sal-3.3
   $ ./install.sh
 3. Add the 'sal-3.3/bin' directory to the user PATH environment variable; in this example
   C:\cygwin\home\<user>\bin\sal-3.3\bin

To test, you can run this from a Cygwin terminal (assuming that you have already added the 'bin\' location to the PATH and restarted the terminal):
  $ sal-inf-bmc ~/bin/sal-3.3/examples/inf-bakery/inf_bakery.sal mutex 
  no counterexample between depths: [0, 10].


Hints when Running:
-------------------

1) Dashboard in Google Chrome

If using the dashboard, prepare Chrome by using the following command:
  $> "C:\Program Files (x86)\Google\Chrome\Application\chrome.exe" --allow-file-access-from-files
in a terminal window.

Once a run has finished, use a CTRL-click on the banner to show the "Formal Verification" tab.  There, you can see a number of squares that are colored green, red, or yellow.  One square corresponds to one tested property.  If they are green, the property was verified.  Yellow means that something went wrong -- possibly missing properties in the model.  Look at the details by clicking the yellow square.  Red means that the verification found a counter example and could not prove the property.  Click on the red square to see details.  Also, if using Simulink models and MATLAB is installed, we generate input files for Simulink to allow further investigation.

If the squares do not show up after the completed run, try closing the Dashboard (but not Google Chrome) and re-open the top-level index.html file, then doing the CTRL-click in the banner to reveal the formal verification tab.

2) MATLAB Inspection of Counter Examples

If MATLAB was installed and .mdl files are found in the salted result directory (in the same place as the generated DAE XML file), you can use the generated MATLAB input files in the Simulink model to step through the counter example.  Assuming you are at the top of the salted results directory, look at the output of
  stdout.txt
for the details on which properties have generated counter examples in HybridSal and then use the respective files under
  CyPhy/Cyber/*.mat
as inputs when opening the .mdl files under
  CyPhy/Cyber/*.mdl

If our program has generated such input files, you will find output similar to the following in the 'stdout.txt' file:

===
  ~~~ Property:
      G(((shift_requested = 3  AND  gear = 7)) => F(gear_selected  =  4))
  ~~~ Find the MATLAB input files under C:\Users\Linda\MetaX\ShiftController\results\ywucumfh\CyPhy\Cyber\1-ResponseProperty2f-*.mat
  ~~~ When adding a scope then limit to these variables from LTL:
      ['shift_requested', 'gear_selected', 'gear']
===

3) Troubleshooting

We generate various log files under the salted results directory's
  log/
directory.  The top-level log file is
  log/CyberComp_runner.log
and any files '*-stdout.log' and '*-stderr.log' contain the STDOUT or STDERR, respectively , of the launched applications, namely the Open Modelica compiler (omc.exe) and the Cyber Composition verification with HybridSal (cc_modelica_hra_verifier.exe).

Make sure that there are no spaces in any of the paths you are using.
