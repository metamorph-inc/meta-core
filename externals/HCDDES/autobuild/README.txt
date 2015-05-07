
Welcome to the autobuild for ESMoL.  You should have the following
things available to build:

GME
UDM/Great
Visual Studio 2003 .NET
Boost (version 1_33_1 was used last)
Antlr

In this directory, you will find a few files:

vcp_cvs_checkout.bat:    Get the latest of everything from the repository.
vcp_set_environment.bat: Sets paths for finding all of the required parts.
vcp_process_models.bat:  Create xml for models and run transformations.
vcp_build.bat:           Actually build the Debug/Release versions of ESMoL.
vcp_autobuild.bat:       Run all of the above and make a log-file. 

CreateLicense.vbs:

Generate the proper model files:
CreateMGA.vbs
CreateXMP.vbs
RunGReATMasterInt.vbs

sendmail.vbs:            Sends out an e-mail after a build (use for nightly builds).

Steps to follow:

0. Make sure you have all of the above installed.
1. Edit vcp_set_environment to suit your setup.
2. From this directory, invoke vcp_autobuild.bat.i

If you want to run things manually, you can use the other scripts to invoke
individual steps.  A common usage is:

1. Run vcp_set_environment
2. Run vcp_process_models
3. Run vcp_build as many times as needed while developing/changing config. 

One more useful trick:

1. From the command line, run vcp_set_environment.
2. cd .. (the VCP directory)
3. Run devenv VCP.sln

This gives you a visual build environment identical to the one used by
autobuild.  The alternative (setting all of those environment variables
permanently on your system) isn't very pretty.

Notes:

For those that already have Visual Studio 2005 installed, you should remove
VS 2005 before installing VS 2003.  They can coexist, but there are reports
of problems if they are installed in the wrong order.


