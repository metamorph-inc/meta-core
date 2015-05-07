import os
import sys
import shutil
import gme
import win32com.client

paradigm = "ESMoL"
if len( sys.argv ) >= 2 and sys.argv[ 1 ] == "CyPhyML":
    paradigm = "CyPhyML"

if not 'VCP_PATH' in os.environ or os.environ[ 'VCP_PATH' ] == '':
    print "VCP_PATH environment variable not set"
    print "Please set the VCP_PATH environment variable before running this program"
    sys.exit( 1 )

vcp_path = os.environ[ 'VCP_PATH' ]

meta_path = ''
if paradigm == "CyPhyML" and ( not 'META_PATH' in os.environ or os.environ[ 'META_PATH' ] == '' ):
    print "META_PATH environment variable not set and CyPhyML paradigm specified"
    print "If CyPhyML paradigm is specified, META_PATH environment variable must be set to trunk of CyPhyML code base to run this program"
    sys.exit( 1 )
else:
    meta_path = os.environ[ 'META_PATH' ]

destdir = vcp_path + r"\generated\ESM2SLC\models\\" + paradigm
destfile = destdir + r"\ESM2SLC.mga"

if not os.path.exists( destdir ):
    os.makedirs( destdir )

# these output a file of the same name in the same directory
gme.xme2mga( vcp_path + r"\src\models\ESM2SLC\ESM2SLC.xme", destfile )

if paradigm != "CyPhyML":
    print "\"" + paradigm + "\" paradigm specified:  no transform will be executed"
    sys.exit( 0 )

print "Executing transform ...",
sys.stdout.flush()

# change the metaname of the UML diagram
gme.mga2xme( meta_path + r"\generated\CyPhyML\models\CyPhyML_uml.mga" )
with file( meta_path + r"\generated\CyPhyML\models\CyPhyML_uml.xme", "rb" ) as xme:
    lines = xme.readlines()
with file( meta_path + r"\generated\CyPhyML\models\CyPhyML_uml.xme", "wb" ) as xme:
    for line in lines:
        xme.write(  line.replace( 'metaname="UML"', 'metaname="UMLModelTransformer"' )  )

mga = win32com.client.DispatchEx("Mga.MgaProject")
mga.Open( "MGA=" + destfile )

xme = win32com.client.DispatchEx("Mga.MgaParser")
xme.ParseProject(mga, meta_path + r"\generated\CyPhyML\models\CyPhyML_uml.xme")

mga.BeginTransactionInNewTerr()



def lookup( obj ):
    retval = dict()
    for object in obj.ChildObjects:
        if not object.Name in retval or hasattr( object, "Meta" ) and object.Meta.Name == "Namespace":
            retval[ object.Name ] = object
    return retval


def lookup_classObjects( obj, className ):
    retval = dict()
    for object in obj.ChildObjects:
        if object.Meta.Name != className:
            continue
        retval[ object.Name ] = object
    return retval


def getObject( object, path ):
    if len( path ) == 0:
        return object

    childDict = lookup( object )

    return getObjectFromDict( childDict, path )


def getObjectFromDict( dictionary, path ):
    
    pathSplit = path.split( '/', 1 )
    childName = pathSplit[ 0 ]
    if not childName in dictionary:
        return null

    child = dictionary[ childName ]
    return child if len( pathSplit ) == 1 else getObject( child, pathSplit[ 1 ] )


models = lookup( mga.RootFolder )

namemap = {
    'Parameter'  : 'SF_Parameter',
    'Port'       : 'SF_Port',
    'Junction'   : 'SF_Junction',
    'Matrix'     : 'SF_Matrix',
    'TypeBase'   : 'SF_TypeBase',
    'ActionPort' : 'EnablePort',
}

def swap_ref( old_target, new_target ):
    for reference in old_target.ReferencedBy:
        reference.Referred = new_target

CyPhyML_classdiagram_map = lookup( models[ 'CyPhyML' ] )
signalflow_classdiagram_map = lookup(  CyPhyML_classdiagram_map[ 'SignalFlow' ]  )
esmol_classdiagram_map= lookup( models[ 'ESMoL' ] )
for classdiagram in ( 'Dataflow', 'Stateflow', 'Types' ):
    
    esmol_class_map  = lookup( esmol_classdiagram_map[ classdiagram ] )
    signalflow_class_map = lookup( signalflow_classdiagram_map[ classdiagram ] )

    for esmol_class_name, esmol_class in esmol_class_map.iteritems():

        if esmol_class.Meta.Name != 'Class':
            continue

        signalflow_class_name = esmol_class_name
        if esmol_class_name in namemap:
            signalflow_class_name = namemap[ esmol_class_name ]


        if signalflow_class_name in signalflow_class_map:
            signalflow_class = signalflow_class_map[ signalflow_class_name ]
            if signalflow_class.Meta.Name == 'Class':
                swap_ref( esmol_class, signalflow_class )


Dataflow_Class = mga.RootFolder.ObjectByPath( '/@ESMoL/@Dataflow/@Dataflow' )
SignalFlowModel_Class = mga.RootFolder.ObjectByPath( '/@CyPhyML/@BehaviorModels/@SignalFlowModel' )
swap_ref( Dataflow_Class, SignalFlowModel_Class )


ModelsFolder_Class = mga.RootFolder.ObjectByPath( '/@ESMoL/@Import/@ModelsFolder' )
Component_Class = mga.RootFolder.ObjectByPath( '/@CyPhyML/@1_Component/@Component' )
swap_ref( ModelsFolder_Class, Component_Class )


DesignFolder_Class = mga.RootFolder.ObjectByPath( '/@ESMoL/@ComponentTypes/@DesignFolder' )
Components_Class = mga.RootFolder.ObjectByPath( '/@CyPhyML/@Folders/@Components' )
swap_ref( DesignFolder_Class, Components_Class )


ESMoL_RootFolder_Class = mga.RootFolder.ObjectByPath( '/@ESMoL/@zC_RootFolderCompositionSheet/@RootFolder' )
CyPhyML_RootFolder_Class = mga.RootFolder.ObjectByPath( '/@CyPhyML/@zC_RootFolderCompositionSheet/@RootFolder' )
swap_ref( ESMoL_RootFolder_Class, CyPhyML_RootFolder_Class )


#
# START TRANSFORM MODIFICATION
#
ESM2SLC_Transform_map = lookup( models[ 'ESM2SLC' ] )

models[ 'ESMoL' ].DestroyObject()


#
# MODIFY ESM2SLC/GetSubsystems
#
GetSubsystems_Block = ESM2SLC_Transform_map[ 'GetSubsystems' ]
GetSubsystems_Block_map = lookup( GetSubsystems_Block )

GetSubsystems_Block_map[ 'Test' ].DestroyObject();
GetSubsystems_Block_map[ 'GetRefSubsystems' ].DestroyObject();

source_port = GetSubsystems_Block_map[ 'dataflow' ]
source_port.Name = "comptype"

destination_port = getObjectFromDict( GetSubsystems_Block_map, 'GetSubSystems/dataflow' )
destination_port.Name = "comptype"

GetSubsystems_Block_Sequence_map = lookup_classObjects( GetSubsystems_Block, 'Sequence' )
Sequence_metaRole = GetSubsystems_Block_Sequence_map[ 'Sequence' ].MetaRole
GetSubsystems_Block.CreateSimplerConnDisp( Sequence_metaRole, source_port, destination_port )



TL_Block = ESM2SLC_Transform_map[ 'TL' ]
TL_Block_map = lookup( TL_Block )



#
# MODIFY ESM2SLC/TL
#
GetProject_Rule_Out_map = lookup_classObjects( TL_Block_map[ 'GetProject' ], 'Out' )
CheckPorts_Block_In_map = lookup_classObjects( TL_Block_map[ 'CheckPorts' ], 'In' )

TL_Block_map[ 'HasRefSubsystems' ].DestroyObject()

source_port = GetProject_Rule_Out_map[ 'dataflow' ]
source_port.Name = "comptype"
destination_port = CheckPorts_Block_In_map[ 'dataflow' ]
destination_port.Name = "comptype"
TL_Block.CreateSimplerConnDisp( Sequence_metaRole, source_port, destination_port )

source_port = GetProject_Rule_Out_map[ 'project' ]
destination_port = CheckPorts_Block_In_map[ 'project' ]
TL_Block.CreateSimplerConnDisp( Sequence_metaRole, source_port, destination_port )


#
# MODIFY ESM2SLC/TL/MakeClasses/GetBlocks
#
MakeClasses_Block_map = lookup( TL_Block_map[ 'MakeClasses' ] )
GetBlocks_Block = MakeClasses_Block_map[ 'GetBlocks' ]
GetBlocks_Block_map = lookup( GetBlocks_Block )

GetBlocks_Block_map[ 'Test' ].DestroyObject()
GetBlocks_Block_map[ 'GetRefSubsystemsFull' ].DestroyObject()

GetBlocks_Block_In_map = lookup_classObjects( GetBlocks_Block, 'In' )
GetSubSystems_Rule_In_map = lookup_classObjects( GetBlocks_Block_map[ 'GetSubSystems' ], 'In' )

source_port = GetBlocks_Block_In_map[ 'dataflow' ]
source_port.Name = "comptype"
destination_port = GetSubSystems_Rule_In_map[ 'dataflow' ]
destination_port.Name = "comptype"
GetBlocks_Block.CreateSimplerConnDisp( Sequence_metaRole, source_port, destination_port )

source_port = GetBlocks_Block_In_map[ 'project' ]
destination_port = GetSubSystems_Rule_In_map[ 'project' ]
GetBlocks_Block.CreateSimplerConnDisp( Sequence_metaRole, source_port, destination_port )


#
# MODIFY CONNECTORREF CHILD OF PRIMITIVE BLOCK
#
Stateflow_Class_map = lookup( signalflow_classdiagram_map[ 'Stateflow' ] )
State_Class = Stateflow_Class_map[ 'State' ]

for path in [
 '/@ESM2SLC/@TL/@CheckPorts/@CheckPortTypes/@CheckAllPorts/@ChartFilter/@block/@ConnectorRef',
 '/@ESM2SLC/@TL/@MakeClasses/@CreateAllPrograms/@CreateClasses/@SubsystemFilter/@IsStateChart/@ConnectorRef',
 '/@ESM2SLC/@SubsystemFilter/@SubsystemFilter/@IsStateChart/@ConnectorRef',
 '/@ESM2SLC/@SubsystemFilter/@SubsystemFilter/@IsStateChartInstance/@ConnectorRef',
 '/@ESM2SLC/@CreateBlockExecution/@TestChildBlock/@IsChart/@ConnectorRef'
]:

    connectorRef_reference = mga.RootFolder.ObjectByPath( path )
    connectorRef_reference.Referred = State_Class
    connectorRef_reference.Name = "State"

for path in [
 '/@ESM2SLC/@TL/@MakeClasses/@CreateAllPrograms/@CreateFunctionsAndArgs/@CreateChartFunction',
 '/@ESM2SLC/@CreateBlockExecution/@ChartChildBlock/@MakeTrigger/@CreateTriggering/@MakeTrigger/@AddTriggerVars/@AddMatrixTriggerVars/@StartAssignment',
 '/@ESM2SLC/@CreateBlockExecution/@ChartChildBlock/@MakeTrigger/@CreateTriggering/@MakeTrigger/@AddTriggerVars/@AddStructTriggerVars/@StartAssignment',
 '/@ESM2SLC/@CreateBlockExecution/@ChartChildBlock/@GetState'
]:

    Rule = mga.RootFolder.ObjectByPath( path )
    Rule_map = lookup( Rule )

    PatternComposition_metaRole = Rule_map[ 'PatternComposition' ].MetaRole
    Rule.CreateSimplerConnDisp( PatternComposition_metaRole, Rule_map[ 'State' ], Rule_map[ 'Primitive' ] )
    Rule_map[ 'ConnectorRef' ].DestroyObject()


#
# MODIFY ESM2SLC/GetRefSubsystems
#
ESM2SLC_Transform_map[ 'GetRefSubsystems' ].DestroyObject()

#HRESULT CreateSimpleConnDisp([in] IMgaMetaRole *meta, [in] IMgaFCO *Src, [in] IMgaFCO *Dst,
#[in] IMgaFCO *Srcref, [in] IMgaFCO *Dstref, [out, retval] IMgaFCO **newobj);
# newconn = model.CreateSimpleConnDisp(oldconn.Meta, , , None, None)

mga.Name = "ESM2SLC"
mga.RootFolder.Name = "ESM2SLC"
mga.CommitTransaction()

mga.Save( "MGA=" + destfile )

print "done."
