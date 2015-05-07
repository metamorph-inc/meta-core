' Send by connecting to port 25 of the SMTP server.
Dim iMsg
Dim iConf
Dim Flds
Dim strHTML
Dim objAttach 
Const cdoSendUsingPort = 2

set iMsg = CreateObject("CDO.Message")
set iConf = CreateObject("CDO.Configuration")

Set Flds = iConf.Fields

' Set the CDOSYS configuration fields to use port 25 on the SMTP server.

With Flds
    .Item("http://schemas.microsoft.com/cdo/configuration/sendusing") = cdoSendUsingPort
    .Item("http://schemas.microsoft.com/cdo/configuration/smtpserver") = "discovery.isis.vanderbilt.edu"
    .Item("http://schemas.microsoft.com/cdo/configuration/smtpconnectiontimeout") = 10
    .Update
End With

' Build HTML for message body.
set shell = WScript.CreateObject( "WScript.Shell" )
vcp_log = "\\Atlantis\project\Escher\VCP_Build_Log\" & Right(shell.ExpandEnvironmentStrings("%VCP_LOGF%"),21)
set shell = Nothing

strHTML = "<HTML>"
strHTML = strHTML & "<HEAD>"
strHTML = strHTML & "<BODY>"
strHTML = strHTML & "The last VCP autobuild job reported an error. See the VCP build log file at:<br>" 
strHTML = strHTML & "<A href=file:///" & vcp_log & ">" & vcp_log & "</A><br>"
strHTML = strHTML & "Please attend to this at your earliest convenience. </br>"
strHTML = strHTML & "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~</br>"
strHTML = strHTML & "<small></br>This is an automated email, please do not reply.</br></small>"
strHTML = strHTML & "</BODY>"
strHTML = strHTML & "</HTML>"

' Apply the settings to the message.
With iMsg
    Set .Configuration = iConf
    .To = "<fengshi@isis.vanderbilt.edu>" 
    .Cc= ""
    .From = "<escher_autobuild@isis.vanderbilt.edu>" 
    .Subject = "SPP autobuild failed at " & Now
    .HTMLBody = strHTML
    .Send

End With

' Clean up variables.
Set iMsg = Nothing
Set iConf = Nothing
Set Flds = Nothing

'MsgBox "Mail Sent!"
