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

destdir = vcp_path + r"\generated\SF_CodeGen\models\\" + paradigm
destfile = destdir + r"\ESM2SFC.mga"

if not os.path.exists( destdir ):
    os.makedirs( destdir )

# these output a file of the same name in the same directory
gme.xme2mga( vcp_path + r"\src\SF_CodeGen\models\ESM2SFC.xme", destfile )

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


def getChildObjects( obj, className ):
    retval = []
    for object in obj.ChildObjects:
        if hasattr( object, "Meta" ) and object.Meta.Name == className:
            retval.append( object )
    return retval


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
esmol_classdiagram_map = lookup( models[ 'ESMoL' ] )
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


Stateflow_ClassDiagram_map = lookup( esmol_classdiagram_map[ 'Stateflow' ] )
Stateflow_Class = Stateflow_ClassDiagram_map[ 'Stateflow' ]

Dataflow_ClassDiagram_map = lookup(  signalflow_classdiagram_map[ 'Dataflow' ]  )
Block_Class = Dataflow_ClassDiagram_map[ 'Block' ]

swap_ref( Stateflow_Class, Block_Class )


Import_ClassDiagram_map = lookup( esmol_classdiagram_map[ 'Import' ] )
ModelsFolder_Class = Import_ClassDiagram_map[ 'ModelsFolder' ]

BehavioralModels_ClassDiagram_map = lookup( CyPhyML_classdiagram_map[ 'BehaviorModels' ] )
SignalFlowModel_Class = BehavioralModels_ClassDiagram_map[ 'SignalFlowModel' ]

swap_ref( ModelsFolder_Class, SignalFlowModel_Class )



_1_Component_ClassDiagram_map = lookup( CyPhyML_classdiagram_map[ '1_Component' ] )
Component_Class = _1_Component_ClassDiagram_map[ 'Component' ]


ComponentTypes_ClassDiagram_map = lookup( esmol_classdiagram_map[ 'ComponentTypes' ] )
DesignFolder_Class = ComponentTypes_ClassDiagram_map[ 'DesignFolder' ]

Folders_ClassDiagram_map = lookup( CyPhyML_classdiagram_map[ 'Folders' ] )
Components_Class = Folders_ClassDiagram_map[ 'Components' ]

swap_ref( DesignFolder_Class, Components_Class )


ESMoL_RootFolder_ClassDiagram_map = lookup( esmol_classdiagram_map[ 'zC_RootFolderCompositionSheet' ] )
ESMoL_RootFolder_Class = ESMoL_RootFolder_ClassDiagram_map[ 'RootFolder' ]

CyPhyML_RootFolder_ClassDiagram_map = lookup( CyPhyML_classdiagram_map[ 'zC_RootFolderCompositionSheet' ] )
CyPhyML_RootFolder_Class = CyPhyML_RootFolder_ClassDiagram_map[ 'RootFolder' ]

swap_ref( ESMoL_RootFolder_Class, CyPhyML_RootFolder_Class )


#
# START TRANSFORM MODIFICATION
#
ESM2SFC_Transform_map = lookup( models[ 'ESM2SFC' ] )

models[ 'ESMoL' ].DestroyObject()


#
# MODIFY ESM2SLC/GetSubsystems
#
TL_Block_map = lookup( ESM2SFC_Transform_map[ 'TL' ] )

CreateTypes_Block_map = lookup( TL_Block_map[ 'CreateTypes' ] )
GetTypes_Block_map = lookup( CreateTypes_Block_map[ 'GetTypes' ] )
GetContainer2_Rule = GetTypes_Block_map[ 'GetContainer2' ]
GetContainer2_Rule_map = lookup( GetContainer2_Rule )

ModelsFolder_ref = GetContainer2_Rule_map[ "ModelsFolder" ]
DesignFolder_ref = GetContainer2_Rule_map[ "DesignFolder" ]

ModelsFolder_ref_metaRole = ModelsFolder_ref.MetaRole

PatternComposition_metaRole = GetContainer2_Rule_map[ 'PatternComposition' ].MetaRole

Component_ref = GetContainer2_Rule.CreateReference( ModelsFolder_ref_metaRole, Component_Class )
Component_ref.Name = "Component"

for patternComposition in getChildObjects( GetContainer2_Rule, "PatternComposition" ):
    if ( patternComposition.Src == ModelsFolder_ref ):
        patternComposition.DestroyObject()
        break

GetContainer2_Rule.CreateSimplerConnDisp( PatternComposition_metaRole, ModelsFolder_ref, Component_ref )
GetContainer2_Rule.CreateSimplerConnDisp( PatternComposition_metaRole, Component_ref, DesignFolder_ref )

#HRESULT CreateSimpleConnDisp([in] IMgaMetaRole *meta, [in] IMgaFCO *Src, [in] IMgaFCO *Dst,
#[in] IMgaFCO *Srcref, [in] IMgaFCO *Dstref, [out, retval] IMgaFCO **newobj);
# newconn = model.CreateSimpleConnDisp(oldconn.Meta, , , None, None)

mga.Name = "ESM2SFC"
mga.RootFolder.Name = "ESM2SFC"
mga.CommitTransaction()

mga.Save( "MGA=" + destfile )

print "done."
