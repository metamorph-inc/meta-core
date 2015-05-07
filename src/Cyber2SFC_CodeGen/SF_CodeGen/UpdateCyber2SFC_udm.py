import os
import sys
import shutil
import os.path
sys.path.append(os.path.join(os.path.dirname(os.path.abspath(__file__)), r'..\..\..\externals\common-scripts'))
import gme
import win32com.client

paradigm = "CyberComposition"

meta_path = ''
if paradigm == "CyberComposition" and ( not 'META_PATH' in os.environ or os.environ[ 'META_PATH' ] == '' ):
    print "META_PATH environment variable not set and CyberComposition paradigm specified"
    print "If CyberComposition paradigm is specified, META_PATH environment variable must be set to trunk of Cyber code base to run this program"
    sys.exit( 1 )
else:
    meta_path = os.environ[ 'META_PATH' ]

destdir = meta_path + r"\src\Cyber2SFC_CodeGen\SF_CodeGen"
destfile = destdir + r"\Cyber2SFC_udm.mga"

if not os.path.exists( destdir ):
    os.makedirs( destdir )

gme.xme2mga( destdir + r"\Cyber2SFC_udm.xme", destfile )

mga = win32com.client.DispatchEx("Mga.MgaProject")
mga.Open( "MGA=" + destfile )

gme.mga2xme( meta_path + r"\generated\Cyber\models\CyberComposition_uml.mga" )
xme = win32com.client.DispatchEx("Mga.MgaParser")
xme.ParseProject(mga, meta_path + r"\generated\Cyber\models\CyberComposition_uml.xme")

mga.BeginTransactionInNewTerr()

SF_TypeBase = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@Types|kind=ClassDiagram/@SF_TypeBase|kind=Class')
TypeBase = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram/TypeBase')
TypeBase.Referred = SF_TypeBase

State = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@Stateflow|kind=ClassDiagram/@State|kind=Class')
State_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram/State')
State_ref.Referred = State

Data = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@Stateflow|kind=ClassDiagram/@Data|kind=Class')
Data_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram/Data')
Data_ref.Referred = Data

Event = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@Stateflow|kind=ClassDiagram/@Event|kind=Class')
Event_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram/Event')
Event_ref.Referred = Event

SF_TypeBaseRef = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@Types|kind=ClassDiagram/@TypeBaseRef|kind=Class')
TypeBaseRef = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram/TypeBaseRef')
TypeBaseRef.Referred = SF_TypeBaseRef

SFState = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@SFStates|kind=ClassDiagram/@SFState|kind=Class')
SFState_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram2/SFState')
SFState_ref.Referred = SFState

SFData = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@SFStates|kind=ClassDiagram/@SFData|kind=Class')
SFData_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram2/SFData')
SFData_ref.Referred = SFData

SFEvent = mga.RootFolder.ObjectByPath('/@CyberComposition|kind=Package/@Simulink|kind=Namespace/@SFStates|kind=ClassDiagram/@SFEvent|kind=Class')
SFEvent_ref = mga.RootFolder.ObjectByPath('/LINKS/NewClassDiagram2/SFEvent')
SFEvent_ref.Referred = SFEvent

mga.Name = "Cyber2SFC"
mga.RootFolder.Name = "Cyber2SFC"

mga.CommitTransaction()
mga.Save( "MGA=" + destfile )