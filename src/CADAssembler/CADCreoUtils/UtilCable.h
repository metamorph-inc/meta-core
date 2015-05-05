



#ifndef UTILCABLE_H
#define UTILCABLE_H

/*----------------------------------------------------------*\
    Headers
\*----------------------------------------------------------*/

#include <ProToolkit.h>
#include <ProMdl.h>
#include <ProSelection.h>
#include <ProArray.h>
#include <ProCabling.h>
/*----------------------------------------------------------*\
    Data types
\*----------------------------------------------------------*/
typedef  struct pro_cbl_harness
{
    ProPart     harness;
    ProName     name;
    ProAssembly owner;
} ProCblHarness;

typedef  struct pro_cbl_cable
{
    int id;
    ProName    name;
    ProCblHarness *p_owner;
} ProCblCable;

typedef  struct pro_cbl_spool
{
    ProName     name;
    ProAssembly owner;
} ProCblSpool;

typedef ProCableparam ProCblParameterInfo;

typedef struct pro_comp_path ProCblConnector;
typedef struct entity * ProEntryport;
typedef struct pro_cbl_cable ProCblBundle;

typedef struct procbl_cable_geom ProCblCableGeom;

typedef ProCablelocationType ProCblLocationType;

typedef struct procbl_cable_point ProCblLocationPnt;

/*===========================================================================*/
ProError ProCblCableHarnessesGet (
    ProCblCable     *p_cable,
    ProCblHarness   **p_harnesses,
    int             *har_num);
/*
  Purpose  : Retrieves pointers to the harnesses in the specified assembly
             that contain the specified wire.

 Input Arguments:
    p_wire                - The wire's handle.

 Output Arguments:
    p_harnesses           - ProArray of harnesses. Function allocate memory for
                            this argument. User must free memory by ProArrayFree
    har_num               - The number of retrieved harnesses.

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully retrieved harnesses.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/

ProError ProCblCableLengthGet (
    ProCblCable      *p_cable,
    double          *p_length);
/*
  Purpose  : Retrieves the length of the specified wire

 Input Arguments:
    p_wire                - The wire's handle.

 Output Arguments:
    p_length              - The wire length.

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully retrieved length.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/


ProError ProCblCableInit (
    int            id, 
    ProName        name, 
    ProCblHarness  *p_owner,
    ProCblCable    *p_cable);
/*
  Purpose:     Initialize cable's handle 

  Input Arguments:
      p_cable             - The handle of cable to initialize
      id                  - The cable id
      name                - The cable name
      p_owner             - The cable owner

  Output Arguments:
      None

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully initialized the cable.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/

ProError ProCblCableNameGet (
    ProCblCable    *p_cable,
    ProName        name);
/*
  Purpose  : Retrieves cable name

  Input Arguments:
    p_cable               - The handle of cable.

  Output Arguments:
    name                  - The retrieves name.

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully initialized the cable.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/

ProError ProCblCableIdGet (
    ProCblCable    *p_cable,
    int            *p_id);
/*
  Purpose  : Retrieves cable id

  Input Arguments:
      p_cable             - The handle of cable.

  Output Arguments:
      p_id                - The retrieved cable's id. 

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully retrieved id.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/

ProError ProCblCableTypeGet (
    ProCblCable     *p_cable,
    ProCableType *p_type);
/*
  Purpose  : Retrieves cable type 

  Input Arguments:
      p_cable             - The handle of cable.

  Output Arguments:
      p_id                - The retrieved cable's type. 

  Return Values:
      PRO_TK_NO_ERROR     - The function successfully retrieved type.
      PRO_TK_BAD_INPUTS   - One or more of the input arguments are invalid.
      PRO_TK_GENERAL_ERROR- Other error.
*/

ProError ProCblCableCreate(
    ProCblHarness  *p_harness,
    ProCblSpool    *p_spool, 
    ProName        cable_name, 
    ProCblCable    *p_cable);
/*
  Purpose  : Creates new cable or wire in specified harness

   Input Arguments:
      p_harness            - The handle of harness
      spool_name           - The spool name
      cable_name           - The cable name

   Output Arguments:
      p_cable              - The handle of  new cable


   Return Values:
      PRO_TK_NO_ERROR      - The function successfully creates new cable.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_E_FOUND       - Spool does not exist yet, cable already exist.
      PRO_TK_GENERAL_ERROR - Other error.

   See Also:
      ProCblCableInit()
*/    

ProError ProCblBundleCreate(
    ProCblHarness  *p_harness,
    ProCblSpool    *p_spool, 
    ProName        bundle_name,
    int            cable_num, 
    ProCblCable    *p_cables,
    ProBoolean     bundle_type,
    ProCblBundle   *p_bundle);
/*
    Purpose  : Creates new bundle in specified harness

   Input Arguments:
      p_harness            - The handle of harness
      spool_name           - The spools name
      bundle_name          - The bundles name
      cable_num            - The number of cables
      p_cables             - The array of cables to add
      bundle_type          - The bundle type :
                              TRUE  - flat
                              FALSE - round

   Output Arguments:
      p_bundle             - The handle of new bundle

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully creates new bundle.
      PRO_TK_BAD_INPUTS    - Some arguments are invalid.
      PRO_TK_E_FOUND       - Spool does not exist yet, bundle already exist.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblBundleCablesGet(
    ProCblBundle   *p_bundle,
    ProBundleCableLevel level,
    ProCblCable    **p_cables,
    int            *p_n_cables);
/*
  Purpose  : Retrieves one parameter for the specified cable

   Input Arguments:
      p_bundle             - bundle handle
      level                - Retrieves level:
                             -1 - collect all cables;
                              1 - retrieve all cables included
                                  in the input bundle
                              2 - retrieve all cables included
                                  in the first level bundle

   Output Arguments:
     p_cables              - ProArray of bundles. Function allocate memory 
                             for this argument. User must free it
                             using ProArrayFree.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully creates new bundle.
      PRO_TK_BAD_INPUTS    - Some arguments are invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/


ProError ProCblCableParameterGet(
    ProCblCable    *p_cable,
    ProName        param_name,
    ProCableparam *p_param_info);
/*
  Purpose  : Retrieves one parameter for the specified cable

   Input Arguments:
      p_cable              - The handle of cable
      param_name           - The parameter name

   Output Arguments:
      p_param_info         - The handle of parameters info

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved the parameter.
      PRO_TK_E_NOT_FOUND   - cable or parameter with this name does not exist.
      PRO_TK_BAD_INPUTS    - Some arguments are invalid.
      PRO_TK_GENERAL_ERROR - Other error
*/

ProError ProCblCableParameterDelete(
    ProCblCable *p_cable,
    ProName     param_name);

/*
  Purpose  : Deletes one parameter for specified cable

   Input Arguments:
      p_cable              - The handle of cable
      param_name           - The name of deletes parameter

   Output Arguments:
      None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully delete parameter.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other error

*/

ProError ProCblCableParametersGet(
    ProCblCable          *p_cable,
    ProCableparam **p_params_info);
/*
  Purpose  : Retrieves the parameters, including all values associated with
             multivalued parameters, for the specified cable

  Input Arguments:
     p_cable               - The handle of cable

   Output Arguments:
     p_params_info         - Pointer to ProArray of parameters.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree. 

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieves parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - Spool with this name does not exist.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCableParametersSet(
    ProCblCable         *p_cable,
    ProCableparam *p_params_info);
/*
  Purpose  : Sets parameters for the specified cable

   Input Arguments:
      p_cable              - The handle of cable
      param_num            - The number of parameters
      p_params_info        - The array of parameters

   Output Arguments:
      None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully set parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - Spool with this name does not exist or 
                             array does not contain required params or
                             read only params are changed.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

/*********************
*************
***********
***********
***********
***********
***********
***********
***********
***********
***********
*/
ProError ProCblCableLogicalEndsGet(
    ProCblCable    *p_cable,
    ProSelection*	   end1,
	ProSelection*	   end2);        /* Out: The entry ports */
/*  
  Purpose  : Retrieves logical ends of specified cable

  Input Arguments:
     p_cable               - The handle of cable

   Output Arguments:
      ends                 - The entry ports.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully get parameter.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - one or both ends are not defined.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCablesFromLogicalCreate(
    ProCblHarness  *p_harness,
    int            cable_num,
    ProName        *p_cable_names); 
/*
  Purpose  : Creates cables from logical
             If corresponding spools are not created they are also created by
             this func.

  Input Arguments:
     p_harness             - The harness handle.
     cable_num             - The number of cables to create.
     p_cable_names         - The cables names.

   Output Arguments:
     None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully create cables.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_FOUND       - Specified cables not imported from logical or
                             already created.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCablesFromLogicalGet(
    ProAssembly    asm_mdl,
    ProName        **p_cable_names,
    int *cable_num);
/*
  Purpose  : Returns list of imported but not created cables

  Input Arguments:
     asm_mdl               - The  assembly's handle.

   Output Arguments:
     p_cable_names         - Pointer to ProArray of names.
                             The function allocates memory for this arguments.
                             To free it, call the function ProArrayFree. 
     cable_num             - Number of retrieved names.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully get list of  cables.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - No cables imported from logical or all 
			     already created.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCableLocationsGet(
    ProCblCable  *p_cable,
    int          **p_location_array,
    int          *n_locations);
/*
  Purpose  : Finds the locations for the specified cable or wire

  Input Arguments:
     p_cable               - The cable's handle. 

   Output Arguments:
     p_location_array      - Pointer to ProArray of ids.
                             The function allocates memory for this arguments.
                             To free it, call the function ProArrayFree.
     n_location            - Number of retrieved ids.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully get list of ids.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblLocationCablesGet(
    ProCblHarness  *p_harness,
    int            loc_id,
    ProCblCable    **p_cables,
    int            *p_n_cables);
/*
 Purpose  : Find cables at the specified location

  Input Arguments:
     p_harness             - The harness's handle.  
     loc_id                - location's id.

   Output Arguments:
     p_cables              - Pointer to ProArray of cables.
                             The function allocates memory for this arguments.
                             To free it, call the function ProArrayFree.
     p_n_cables            - Number of retrieved cables.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully get list of  cables.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCableGeomGet(
    ProCblCable    *p_cable,
    ProCablesegment  **p_segs);
/*
 Purpose  : Find cables at the specified location

  Input Arguments:
     p_cable               - The cable's handle.

   Output Arguments:
     p_geom_arr            - The cable geometry.
                             The function allocates the memory
                             for this argument, and reuses it on subsequent
                             calls. You should not free the memory or 
                             keep a pointer to it.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully get list of  cables.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblCableClearance (
    ProSelection      sel1,
    ProSelection      sel2,
    ProBoolean   *interference,
    ProPoint3d      pnt_arr[2]);
/*
  Purpose  : Determines the clearance of the specified cable, and reports if
             interference was found

  Input Arguments:
    sel1                  - The first selection.
    sel2                  - The second selection.

   Output Arguments:
    interference          - The interference.
    pnt_arr               - The coordinates of the closest points.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully clearance.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/


/*---------------------------------------------------------------------------*\
    Functions to manag e connectors
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorInit (
    ProAsmcomppath  *path,	    
    ProCblConnector   *p_connector);
/*
  Purpose  : Initializes connector structure

  Input Arguments:
     p_path		    - The connector's component path.

   Output Arguments:
     p_connector	    - Connector, user's memory

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully initialize connector.
      PRO_TK_E_NOT_FOUND   - specified component is not designated 
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
*/

ProError ProCblConnectorParameterGet(
    ProCblConnector        *p_connector,
    ProName              w_param_name,
    ProCableparam *p_param);
/*
  Purpose  : Gets connector's parameter

  Input Arguments:
     p_connector           - The connectors handle.
     w_param_name          - Parameter's name.

   Output Arguments:
     p_param               - Pointer to parameters structure. 

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully gets parameter.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblConnectorParameterDelete(
    ProCblConnector   *p_connector,
    ProName        w_param_name);
/*
  Purpose  : Deletes parameter of specified connector

  Input Arguments:
     p_connector           - The connectors handle.
     w_param_name          - The name of parameter.

   Output Arguments:
     None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully deletes parameter.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - Connector or parametetr with this name does
                             not exist,  parameter is required.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblConnectorParametersGet(
    ProCblConnector         *p_connector,
    ProCableparam **p_param);
/*
  Purpose  : Retrives parameters of specified connector

  Input Arguments:
     p_connector           - The connectors handle.

   Output Arguments:
     p_param               - Pointer to ProArray of parameters.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree. 

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieves parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblConnectorParametersSet(
    ProCblConnector *p_connector,
    ProCableparam *p_param);
/*
  Purpose  : Sets parameters of specified connector

  Input Arguments:
     p_connector           - The connectors handle.
     p_param               - ProArray of the new parameters. 

   Output Arguments:
      None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully set parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - The specified assembly is invalid or
                             the member is not a connector
      PRO_TK_GENERAL_ERROR - Other errors.

*/

ProError ProCblConnectorFromLogical(
    ProAssembly asm_mdl,
    ProName    **p_w_name,
    int *con_num);
/*
  Purpose  : Returns list of imported, but not designated connectors

  Input Arguments:
    asm_mdl                - The handle of cablings assembly.

   Output Arguments:
    p_w_name               - Pointer to ProArray of logical connectors  names.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree.
    con_num                - Number of retrieved connectors names.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUND   - no cables imported from logical or
                             all already created
      PRO_TK_GENERAL_ERROR - Other errors.

*/

ProError ProCblAssemblyConnectorsGet(
    ProAssembly assy,
    ProCblConnector **p_cons,
    int              *num_cons);
/*
  Purpose  : Returns list of imported, but not designated connectors

  Input Arguments:
    assy                  - The handle of cablings assembly.

   Output Arguments:
    p_cons                 - Pointer to ProArray of logical connectors.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree.
    num_cons               - Number of retrieved connectors.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.

*/

ProError ProCblConnectorDesignate(
    ProCblConnector  *p_connector,
    ProName          name);
/*
  Purpose  : Designate specified connector

  Input Arguments:
    p_connector            - Handle of connector to designate
    name                   - Name of designator of the logical connector
                             (can be NULL)
   Output Arguments:
    None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully designate.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblConnectorUndesignate(
    ProCblConnector  *p_connector);
/*
  Purpose  : Undesignate specified connector

  Input Arguments:
    p_connector            - Handle of connector to undesignate

   Output Arguments:
    None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully undesignate.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblConnectorEntryPortsGet(
    ProCblConnector  *p_connector,
    int             **p_csys_id_array,
    int              *p_num_csys);
/*
  Purpose  : Retrieves the entry ports for the specified connector.

  Input Arguments:
    p_connector            - Handle of connector.

   Output Arguments:
    p_csys_id_array        - ProArray of csys's ids.
                             Function allocate memory for this argument.
                             User must free it with ProArrayFree.
    p_num_csys             - Number of retrieved csys.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblAssemblyHarnessesGet(
    ProAssembly     asm_mdl,
    ProCblHarness **p_harness,
    int            *num_harness);
/*
  Purpose  : Retrieves the entry ports for the specified connector.

  Input Arguments:
    asm_mdl                - Handle of connector's assembly.

   Output Arguments:
    p_harness              - ProArray of harnesses.
                             Function allocate memory for this argument.
                             User must free it with ProArrayFree.
    num_harness            - Number of retrieved harnesses.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/



/*-----------------------------------------------------------------------*\
    Functions to manage harnesses.
\*-----------------------------------------------------------------------*/

ProError ProCblHarnessInit(
    ProPart        harness_mdl,
    ProName        name,
    ProAssembly    asm_mdl,
    ProCblHarness  *p_harness);
/*
  Purpose  : Initialize harness handle 

  Input Arguments:
    harness_mdl            - The handle of harness part.
    name                   - The harness name.
    asm_mdl                - The harness owner.

   Output Arguments:
    p_harness              - Harness handle to initialize.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully initialize harness.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
*/

ProError ProCblHarnessCreate(
    ProAssembly    asm_mdl,
    ProName        w_name,
    ProCblHarness *p_harness); 
/*
  Purpose  : Creates new harness

  Input Arguments:
    asm_mdl                - The cabling assembly's handle.
    w_name                 - Harness name.

   Output Arguments:
    p_harness              - The harness handle.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully create harness.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_FOUND       - Harness with same name already exist.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblHarnessNameGet(
    ProCblHarness *p_harness,
    ProName       w_name);
/*
  Purpose  : Retrive the name of harness

   Input Arguments:
    p_harness              - The handle of harness.

   Output Arguments:
    w_name                 - Retrieved name.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved name. 
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
*/

ProError ProCblHarnessCablesGet(
    ProCblHarness *p_harness,
    ProCblCable   **p_cables,
    int           *p_n_cables);
/*
  Purpose  : Retrive the cables for specified harness

   Input Arguments:
    p_harness              - The handle of harness.

   Output Arguments:
    p_cables               - ProArray of cables's handles. Function allocate
                             memory for this argument. User must free it's 
                             using ProArrayFree.
    p_n_cables             - Number of retrieved cables.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved cables.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblHarnessLocationsGet(
    ProCblHarness *p_harness,
    int **p_location_array,
    int  *n_locations);
/*
  Purpose  : Retrive the locations for specified harness

   Input Arguments:
    p_harness              - The handle of harness.

   Output Arguments:
    p_location_array       - ProArray of cables's handles. Function allocate 
                             memory for this argument. User must free it's 
                             using ProArrayFree.
    n_locations            - Number of retrieved locations.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved locations.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblHarnessLocationsTypeGet(
    ProCblHarness *p_harness,
    int            location_id,
    ProCblLocationType  *type);
/*
  Purpose  : Retrive the location's type for specified harness

   Input Arguments:
    p_harness              - The handle of harness.
    location_id            - Location's id.

   Output Arguments:
    type                   - Location type.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved locations.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblHarnessLocationsPntGet(
    ProCblHarness *p_harness,
    int            location_id,
    ProCblLocationPnt  *pnt);
/*
  Purpose  : Retrive the location's point for specified harness

   Input Arguments:
    p_harness              - The handle of harness.
    location_id            - Location's id.

   Output Arguments:
    pnt                    - Location's point.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved locations.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/


/*-----------------------------------------------------------------------*\
    Functions to manage spools.
\*-----------------------------------------------------------------------*/
ProError ProCblAssemblySpoolsGet(
    ProAssembly    asm_mdl,
    ProCblSpool    **p_spool,
    int            *num_spools);
/*
  Purpose  : Collect spools in specified assembly

   Input Arguments:
    asm_mdl                - Assembly owner of the spool

   Output Arguments:
    p_spool                - ProArray of spool's handles. Function allocate
                             memory for this argument.
                             Use ProArrayFree to free it.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully initialize spool.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_NOT_FOUN    - No spool found.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolInit(
    ProAssembly    asm_mdl,
    ProName        w_name,
    ProCblSpool    *p_spool);
/*
  Purpose  : Initialize spool handle

   Input Arguments:
    asm_mdl                - Assembly owner of the spool
    w_name                 - Spool name.

   Output Arguments:
    p_spool                 - Spool handle to initialize 

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully initialize spool. 
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
*/

ProError ProCblSpoolCreate(
    ProAssembly        assy,
    ProName            w_name,
    ProCableType    cable_type,
    ProBundleType   sheath_type,
    ProCblSpool        *p_spool);
/*
   Purpose  : Creates new spool

   Input Arguments:
    assy                  - The handle of spools assembly owner.
    w_name                - The harness name.
    cable_type            - The type of created spool.
    sheath_type           - The type of bundle.

   Output Arguments:
    p_spool               - The handle of created spool.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully creates new spool.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_E_FOUND       - Spool with this name already exist.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolParametersGet(
    ProCblSpool          *p_spool,
    ProCableparam **p_param);
/*
  Purpose  : Retrieves parameters of specified spool

   Input Arguments:
    p_spool                - Handle of spool.

   Output Arguments:
    p_param                - Pointer to ProArray of parameters.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree.
    param_num              - Number of retrieved parameters.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved parameters.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_E_NOT_FOUND   - Spool with this name does not exist.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolParameterGet(
    ProCblSpool         *p_spool,
    ProName             w_param_name,
    ProCableparam *p_param);
/*
  Purpose  : Retrieves one parameter of specified spool

   Input Arguments:
    p_spool                - Handle of spool.
    w_param_name           - Name of retrieved parameter
    
   Output Arguments:
    p_param                - Parameter handle.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieved parameter.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolParametersSet(
    ProCblSpool          *p_spool,
    ProCableparam *p_param);
/*
  Purpose  : Set parameters for specified spool

   Input Arguments:
    p_spool                - Handle of spool.
    p_param                - ProArray of new parameters.

   Output Arguments:
    None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully sets new parameters.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_E_NOT_FOUND   - spool with this name does not exist or
                             array does not contain required params or
                             read only params are changed.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolParameterDelete(
    ProCblSpool         *p_spool,
    ProName             w_param_name);
/*
  Purpose  : Deletes parameter in specified spool

   Input Arguments:
    p_spool                - Handle of spool.
    w_param_name           - Name of retrieved parameter
    
   Output Arguments:
    None

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully delete parameter.
      PRO_TK_BAD_INPUTS    - One or more of the arguments are invalid.
      PRO_TK_E_NOT_FOUND   - spool with this name does not exist.
      PRO_TK_GENERAL_ERROR - Other error.
*/

ProError ProCblSpoolFromLogicalGet (
    ProAssembly assy,
    ProName     **p_w_name,
    int         *num_spool);
/*
  Purpose  : Return list of imported, but not created spools

   Input Arguments:
    assy                   - The handle of assembly

   Output Arguments:
    p_w_name               - Pointer to ProArray of names.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree.
    num_spool              - Number of retrieves names.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieves parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_FOUND       - no spools imported from logica or
                             all already created. 
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblSpoolFromLogicalCreate (
    ProAssembly assy,
    ProName     *p_w_name,
    int         spool_num,
    ProCblSpool **p_spools);
/*
  Purpose  : creates spools from logical

   Input Arguments:
    assy                   - The handle of assembly
    p_w_name               - Spool names.
    spool_num              - Number of spools.

   Output Arguments:
    p_spools               - Pointer to ProArray of spools.
                             The function allocates memory for these arguments.
                             To free it, call the function ProArrayFree.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieves parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_E_FOUND       - specified spools not imported from or
                             already created.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblSpoolNameGet (
    ProCblSpool         *p_spool,
    ProName             w_name);

/*
  Purpose  : Return Spool Name

   Input Arguments:
    p_spool              - The spool

  Output Arguments:
    w_name		 -  Spool name

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully return name
*/
    

ProError ProCblRoutingStart (
    ProCblCable    *p_cables,
    void      **p_cable_route_data);
/*
  Purpose  : Initializes routing procedure

   Input Arguments:
    p_cables              - The ProArray of cables to route.

  Output Arguments:
    p_cable_route_data    -  The cable routing data required for subsequent
                            function calls to route cables through locations.
                            The function allocates the space for this argument.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully start routing.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblRouteThruLocation (
    void          *p_cable_route_data,
    ProCablelocationref ref,
    ProCablelocation* p_location,
	ProCablelocation* second_axis_location);
/*
  Purpose  : Finds or creates the location, and routes cables through it

   Input Arguments:
    p_cable_route_data     -  The cable routing data required for subsequent
    ref                    - Referances's handle.

  Output Arguments:
    p_location         - The location identifiers.
                             The function allocates the space for
                             this argument, and reuse it
    second_axis_location   - The number of locations.
                            
   Return Values:
      PRO_TK_NO_ERROR      - The function successfully retrieves parameters.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

ProError ProCblRoutingEnd (
    void      **p_cable_route_data);
/*
  Purpose  : Ends the cable routing

   Input Arguments:
    p_cable_route_data     -  The cable routing data.

  Output Arguments:
    None.

   Return Values:
      PRO_TK_NO_ERROR      - The function successfully ends routing.
      PRO_TK_BAD_INPUTS    - The some arguments is invalid.
      PRO_TK_GENERAL_ERROR - Other errors.
*/

#endif  /* UTILCABLE */
