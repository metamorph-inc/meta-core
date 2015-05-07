README for installing HybridSal on Windows
==========================================

Requirements:
-------------

 * Java 7 (JRE) -- install according to online instructions
 * SAL 3.3 for Cygwin from http://sal.csl.sri.com

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

