%% Generate the MGA and Udm files from the ESMoL xme.

cd Udm
"c:\Program Files\ISIS\UDM\etc\CreateMGA.vbs" ..\..\EsMoL.test\ESMoL_uml.xme ESMoL_uml.mga UML
"c:\Program Files\ISIS\UDM\etc\CreateUdmXml.vbs" ESMoL_uml.mga
"c:\Program Files\ISIS\UDM\bin\Udm.exe" -g ESMoL_udm.xml
cd ..




