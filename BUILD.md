# Building the META Tools
The META tools can be compiled from the source code in this repository. Compilation requires a Windows PC and access to the internet (for downloading NuGet packages).

# Build Machine Setup
Follow these configuration instructions, in order, to set up your machine to build the META tools from source.

## Windows x64 (7 SP1, 8.1, 10 or Server equivalent)
Install Windows updates until your version is current

## Microsoft Visual Studio 2015 (Community or above)
_NOTE: Will not compile under Express Edition_

When installing, select these packages (at minimum):
- C++
- C#
- Office Tools

## Microsoft Visual Studio 2015 Update 2
[Download](https://www.visualstudio.com/en-us/news/vs2015-update2-vs.aspx)

## WIX
[Download 3.x here](http://wixtoolset.org/releases/) (get the newest 3.10 version).

3.6, 3.7, 3.8, 3.9, 3.10 are detected by the build_msi.py script.

## Java JDK 7 or above
[Download it here](http://www.oracle.com/technetwork/java/javase/downloads/jdk7-downloads-1880260.html)

_NOTE: Either x86 or x64 is okay_

### Set JAVA_HOME
Set environment variable JAVA_HOME to the installed directory, such as `C:\Program Files (x86)\Java\jdk1.7.0_09`

The real name of the subdirectory depends on what version you have installed.

## GME 16.3+
[Download it here](https://forge.isis.vanderbilt.edu/gme)

GME_x64 is the best-tested (but 32-bit should work too)

## UDM x64 3.2.14+
[Download it here](http://repo.isis.vanderbilt.edu/UDM/3.2.14/)

## Windows Updates
Again, install Windows updates until everything is current. Restart your computer.

## Git
[Download msysgit](https://msysgit.github.io/)

`git.exe` must be in your `PATH`. For `msysgit`, select `Use Git from the Windows Command Prompt` during installation.

## Clone Repo
Clone this repository to your disk.

# Build
1. Open Visual Studio Command Prompt (2015) with ”Run as administrator”. (Do not use a Visual Studio x64 command prompt)
2. From the root repository directory, run `make_all_without_installer.cmd`. This may take 30 minutes to build. _(Warnings may be ignored, but there should be no errors.)_

If you encounter errors, try to build once more. There may be some remaining race conditions in the build scripts.
