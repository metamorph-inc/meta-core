'	CreateLicense.vbs 
'	Generate the License text file

set shell = WScript.CreateObject( "WScript.Shell" )
vcp_path = shell.ExpandEnvironmentStrings("%vcp_PATH%")

license_file = vcp_path & "\license.txt"
Set file = CreateObject("Scripting.FileSystemObject")
Set License = file.CreateTextFile(license_file, True)

License.Write("Copyright (c) Vanderbilt University, 2000-")
License.writeLine(Year(Date))
License.WriteLine ("") 
License.WriteLine("Permission is granted to use this software for academic/research purposes") 
License.WriteLine ("") 
License.WriteLine ("ALL RIGHTS RESERVED") 
License.WriteLine ("Vanderbilt University disclaims all warranties with regard to this") 
License.WriteLine ("software, including all implied warranties of merchantability") 
License.WriteLine ("and fitness.  In no event shall Vanderbilt University be liable for") 
License.WriteLine ("any special, indirect or consequential damages or any damages") 
License.WriteLine ("whatsoever resulting from loss of use, data or profits, whether") 
License.WriteLine ("in an action of contract, negligence or other tortious action,") 
License.WriteLine ("arising out of or in connection with the use or performance of") 
License.WriteLine ("this software.") 
License.Close
