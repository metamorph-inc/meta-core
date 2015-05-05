

#include <ProCabling.h>
#include <ProCsys.h>

#include "UtilCable.h"

#include "UtilString.h"
#include "TestError.h"

/*---------------------------------------------------------------------------*\
    Functions to manage wires, cables and bundless
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
  Function : ProCblWireHarnessesGet
  Purpose  : Retrieves pointers to the harnesses in the specified assembly 
             that contain the specified wire.
\*---------------------------------------------------------------------------*/
ProError ProCblCableHarnessesGet (
    ProCblCable     *p_cable,        /* In: wire handle */
    ProCblHarness   **p_harnesses,  /* Out: ProArray of harnesses 
                                       user must free memory by ProArrayFree */
    int             *har_num)        /* Out: harness number */
{
    int i;
    ProHarness *p_har_arr;
    ProName name;
	ProError err;
    
    if (p_cable == NULL || p_harnesses == NULL)
        return (PRO_TK_BAD_INPUTS);

	err = ProCableHarnessesGet(p_cable->p_owner->owner, p_cable->name, &p_har_arr);

    TEST_CALL_REPORT("ProCableHarnessesGet", "ProCblWireHarnessesGet",
        (ProError)err, 0);
    if (err != PRO_TK_NO_ERROR)
        return (err);

	ProArraySizeGet(p_har_arr, har_num);
    ProArrayAlloc (*har_num, sizeof (ProCblHarness), 1, (ProArray*)p_harnesses);
    for (i = 0; i < *har_num; i++)
    {
        ProMdlNameGet ((ProMdl)p_har_arr[i], name);
        ProCblHarnessInit ((ProPart)p_har_arr[i], name,
            (ProAssembly)p_cable->p_owner->owner, 
            &(*p_harnesses)[i]);
    }
    return (PRO_TK_NO_ERROR); 
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableLengthGet
  Purpose  : Retrieves the length of the specified wire
\*---------------------------------------------------------------------------*/
ProError ProCblCableLengthGet (
    ProCblCable *p_wire,        /* In: wire handle */
    double     *p_length)      /* Out: wire length */
{
    ProError error;
	ProCable cable;

    if (p_wire == NULL || p_length == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProCableByNameGet(p_wire->p_owner->harness, p_wire->name, &cable);
	error = ProCableLengthGet(&cable, p_length);

    TEST_CALL_REPORT("procbl_get_wire_length", "ProCblWireLengthGet",
        (ProError)error, 0);
    if (!error)
        return (PRO_TK_GENERAL_ERROR);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableInit
  Purpose  : Initialize cable structure
\*---------------------------------------------------------------------------*/
ProError ProCblCableInit (
    int            id,              /* In: cable's id */
    ProName        name,            /* In: cable's name */
    ProCblHarness  *p_owner,        /* In: cable's owner */
    ProCblCable    *p_cable)        /* Out:cable's handle to init*/
{
	ProError error = PRO_TK_NO_ERROR;
	ProCable cable;

	if (p_cable == NULL || (id == -1 && name == NULL))
		return (PRO_TK_BAD_INPUTS);
	if (id == -1)
	{
		error = ProCableByNameGet(p_owner->harness, name, &cable);
		p_cable->id = cable.id;
		TEST_CALL_REPORT("procbl_get_cable_id", "ProCblCableInit",
			(ProError)error, 0);
	}
	else	
		p_cable->id = id;
	if (name == NULL)
	{
      ProName buff;
		error = ProModelitemInit(p_owner->harness, id, PRO_CABLE, &cable);
		error = ProCableNameGet(&cable, buff);
		TEST_CALL_REPORT("procbl_get_cable_name", "ProCblCableInit",
			(ProError)error, 0);
	}
	else
		ProUtilWstrcpy (p_cable->name, name);
	p_cable->p_owner = p_owner;
	return (error ==0 ? PRO_TK_NO_ERROR : PRO_TK_GENERAL_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableNameGet
  Purpose  : Retrieves cable name
\*---------------------------------------------------------------------------*/
ProError ProCblCableNameGet (
    ProCblCable    *p_cable,        /* In: cable's handle*/
    ProName        name)            /* Out:cable's name */
{
    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);
    ProUtilWstrcpy(name, p_cable->name);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableIdGet
  Purpose  : Retrieves cable id
\*---------------------------------------------------------------------------*/
ProError ProCblCableIdGet (
    ProCblCable    *p_cable,        /* In: cable's handle*/
    int            *p_id)           /* Out:cable's id */
{
    if (p_cable == NULL || p_id == NULL)
        return (PRO_TK_BAD_INPUTS);
    *p_id = p_cable->id;
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableTypeGet
  Purpose  : Retrieves cable type
\*---------------------------------------------------------------------------*/
ProError ProCblCableTypeGet (
    ProCblCable   *p_cable,     /* in: cable handle */
    ProCableType *p_type)    /* Out: cable type */
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL || p_type == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProCableByNameGet(p_cable->p_owner->harness, p_cable->name, &cable);
    error = ProCableTypeGet(&cable, p_type);
    TEST_CALL_REPORT("procbl_get_cable_subtype", "Procbl_cable_typeGet",
        (ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (PRO_TK_GENERAL_ERROR);
    return (PRO_TK_NO_ERROR);
}


/*---------------------------------------------------------------------------*\
  Function : ProCblCableCreate
  Purpose  : Creates new cable or wire in specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblCableCreate (
    ProCblHarness  *p_harness,      /* In: harness's handle*/
    ProCblSpool    *p_spool,	    /* In: spool's name */
    ProName        cable_name,      /* In: cable's name */
    ProCblCable    *p_cable)        /* Out:Handle of new cable*/
{
	ProError status;
	ProName w_spool_name;
	ProCable cable;

	if (p_harness == NULL || p_cable == NULL)
		return (PRO_TK_BAD_INPUTS);

	status = ProCblSpoolNameGet(p_spool, w_spool_name);
	TEST_CALL_REPORT("ProCblSpoolNameGet", "ProCblCableCreate",
		status, status != PRO_TK_NO_ERROR);

	status = ProCableCreate(p_harness->harness, (ProSpool*)p_spool, cable_name, &cable);
	TEST_CALL_REPORT("procbl_create_cable", "ProCblCableCreate",
		(ProError)status, 0);
	if (status != PRO_TK_NO_ERROR)
		return (status);

	status = ProCblCableInit (cable.id, cable_name, p_harness, p_cable);

	return (status);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblBundleCreate
    Purpose  : Creates new bundle in specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblBundleCreate (
    ProCblHarness  *p_harness,      /* In: harness's handle*/
    ProCblSpool    *p_spool,	    /* In: spool's name */
    ProName        bundle_name,     /* In: bundle's name */
    int            cable_num,       /* In: cable num */
    ProCblCable    *p_cables,       /* In: the array of cables
                                           to add */
    ProBoolean     bundle_type,     /* In: TRUE=flat, FALSE=round */
    ProCblBundle   *p_bundle)       /* Out:Handle of new bundle*/
{
    ProError status;
    int i;
    ProName *cable_names;
    ProName w_spool_name;
	ProCable bundle;
	ProCable *cables;

    if (p_harness == NULL || p_cables == NULL || p_bundle == NULL)
        return (PRO_TK_BAD_INPUTS);

    status = ProCblSpoolNameGet(p_spool, w_spool_name);
    TEST_CALL_REPORT("ProCblSpoolNameGet", "ProCblBundleCreate",
				    status, status != PRO_TK_NO_ERROR);
    status = ProArrayAlloc (0, sizeof (ProName), 1, 
        (ProArray*)&cable_names);
    for (i = 0; i < cable_num; i++)
        status = ProArrayObjectAdd ((ProArray*)&cable_names, PRO_VALUE_UNUSED,
            1, p_cables[i].name);

	status = ProArrayAlloc(0, sizeof(ProCable), 1, (ProArray*)&cables);
	for(i = 0; i < cable_num; i++)
	{
		ProModelitem item;
		status = ProModelitemInit(p_cables->p_owner->harness, p_cables->id, PRO_CABLE, &item);
		status = ProArrayObjectAdd ((ProArray*)&cables, PRO_VALUE_UNUSED, 1, &item);
	}
	status = ProBundleCreate(p_harness->harness, (ProSpool*)p_spool, bundle_name, 
		cables, bundle_type, &bundle);
    TEST_CALL_REPORT("procbl_create_bundle", "ProCblBundleCreate",
						(ProError)status, 0);

	status = ProCableNameGet(&bundle, bundle_name);
    TEST_CALL_REPORT("procbl_get_cable_name", "ProCblBundleCreate",
						(ProError)status, 0);

    status = ProArrayFree ((ProArray*)&cable_names);
	status = ProArrayFree ((ProArray*)&cables);

    if (status != PRO_TK_NO_ERROR)
        return (status);

    status = ProCblCableInit (bundle.id, bundle_name,
        p_harness, (ProCblCable*)p_bundle); 

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblBundleCablesGet
  Purpose  : Retrieves one parameter for the specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblBundleCablesGet (
    ProCblBundle   *p_bundle,       /* In: bundle handle */
    ProBundleCableLevel level,      /* In: level:
                                       -1   collect all cables
                                        1   retrieve all cables included
                                            in the input bundle
                                        2   retrieve all cables included
                                            in the first level bundle */
    ProCblCable    **p_cables,      /* Out: ProArray of cables handles
                                       User must free mem using ProArrayFree*/
    int            *p_n_cables)       /* Out: cables number */
{
    int i;
	ProError status;
	ProCable bundle;
	ProCable* cables;

    if (p_bundle == NULL || p_cables == NULL || p_n_cables == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProCableByNameGet(p_bundle->p_owner->harness, p_bundle->name, &bundle);
	status = ProBundleCablesCollect(&bundle, level, &cables);
    TEST_CALL_REPORT("procbl_get_bundle_cables", "ProCblBundleCablesGet",
        (ProError)status, 0);

    if (status != PRO_TK_NO_ERROR)
        return (status);

	ProArraySizeGet(cables, p_n_cables);

    ProArrayAlloc (*p_n_cables, sizeof (ProCblCable), 1, (ProArray*)p_cables);
    for (i = 0; i < *p_n_cables; i++)
    {
		ProName cable_name;
		status = ProCableNameGet(&cables[i], cable_name);
		TEST_CALL_REPORT("procbl_get_cable_id", "ProCblBundleCablesGet",
        (ProError)status, 0);
        if (status != PRO_TK_NO_ERROR)
            break;
        ProCblCableInit (cables[i].id, cable_name, p_bundle->p_owner,
            &(*p_cables)[i]);
    }
    if (status != PRO_TK_NO_ERROR)
    {
        ProArrayFree ((ProArray*)p_cables);
        return (PRO_TK_GENERAL_ERROR);
    }
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableParameterGet
  Purpose  : Retrieves one parameter for the specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblCableParameterGet (
    ProCblCable    *p_cable,        /* In: cable's handle*/
    ProName        param_name,      /* In: parameter's name */
    ProCableparam *p_param_info) /* Out: parameter's info */
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableParameterGet(p_cable->p_owner->owner, &cable, param_name, p_param_info);
    TEST_CALL_REPORT("procbl_get_cable_param", "ProCblCableParameterGet",
    (ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

	return (PRO_TK_NO_ERROR);
}


/*---------------------------------------------------------------------------*\
  Function : ProCblCableParameterDelete
  Purpose  : Deletes one parameter for specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblCableParameterDelete (
    ProCblCable    *p_cable,        /* In: cable's handle*/
    ProName        param_name)      /* In: parameter's name*/
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableParameterDelete(p_cable->p_owner->owner, &cable, param_name);
    TEST_CALL_REPORT("procbl_delete_cable_param", "ProCblCableParameterDelete",
    (ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableParametersGet
  Purpose  : Retrieves the parameters, including all values associated with
             multivalued parameters, for the specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblCableParametersGet (
    ProCblCable          *p_cable,       /* In: cable's handle*/
    ProCableparam **p_params_info) /* Out: ProArray of parameters */
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableParametersCollect(p_cable->p_owner->owner, &cable, p_params_info);
    TEST_CALL_REPORT("procbl_get_cable_params", "ProCblCableParametersGet",
    (ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

	return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableParametersSet
  Purpose  : Sets parameters for the specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblCableParametersSet (
    ProCblCable         *p_cable,       /* In: cable's handle*/
	ProCableparam *p_params_info )	/* In: array of parameters */
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableParametersSet(p_cable->p_owner->owner, &cable, p_params_info);
    TEST_CALL_REPORT("procbl_set_cable_params", "ProCblCableParametersSet",
    (ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);


    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableLogicalEndsGet
  Purpose  : Retrieves logical ends of specified cable
\*---------------------------------------------------------------------------*/
ProError ProCblCableLogicalEndsGet (
    ProCblCable    *p_cable,        /* In: cable's handle*/
    ProSelection*	   end1,
	ProSelection*	   end2)        /* Out: The entry ports */
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableLogicalEndsGet(p_cable->p_owner->owner, &cable, end1, end2);
    TEST_CALL_REPORT("procbl_get_cable_logical_ends", 
               "ProCblCableLogicalEndsGet",(ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCablesFromLogicalCreate
  Purpose  : Creates cables from logical
             If corresponding spools are not created they are also created by
             this func.
\*---------------------------------------------------------------------------*/
ProError ProCblCablesFromLogicalCreate (
    ProCblHarness  *p_harness,      /* In: harness's handle*/
    int            cable_num,       /* In: the number of cables to create */
    ProName        *p_cable_names)  /* In: cables names */
{
    ProError status;
    int i; 
    ProName *prodev_names;

    if (p_harness == NULL || p_cable_names == NULL)
        return (PRO_TK_BAD_INPUTS);
    status = ProArrayAlloc (0, sizeof (ProName), 1, 
        (ProArray*)&prodev_names);
    for (i = 0; i < cable_num; i++)
        status = ProArrayObjectAdd ((ProArray*)&prodev_names, 
            PRO_VALUE_UNUSED, 1, p_cable_names[i]);

	status = ProCablesFromLogicalCreate(p_harness->owner, p_harness->harness, prodev_names);
    TEST_CALL_REPORT("procbl_create_cables_from_logical", 
               "ProCblCablesFromLogicalCreate",(ProError)status, 0);
    status = ProArrayFree ((ProArray*)&prodev_names);

    if (status != PRO_TK_NO_ERROR)
        return (status);

    return (PRO_TK_NO_ERROR); 
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCablesFromLogicalGet
  Purpose  : Returns list of imported but not created cables
\*---------------------------------------------------------------------------*/
ProError ProCblCablesFromLogicalGet (
    ProAssembly    asm_mdl,         /* In: assembly's handle*/
    ProName        **p_cable_names, /* Out: ProArray of cable names */
    int *cable_num)                 /* Out: cables number */
{
    ProError status;
    int i;
    ProName *prodev_names;

    if (asm_mdl == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProCablesFromLogicalGet(asm_mdl, &prodev_names);
    TEST_CALL_REPORT("procbl_get_cables_from_logical", 
               "ProCblCablesFromLogicalGet",(ProError)status, 0);
    if (status != PRO_TK_NO_ERROR)
        return (status);
	
	ProArraySizeGet(prodev_names, cable_num);

	status = ProArrayAlloc (0, sizeof (ProName), 1, (ProArray*)p_cable_names);
    if (status != PRO_TK_NO_ERROR)
        return (PRO_TK_GENERAL_ERROR);
    
	for (i = 0; i < *cable_num; i++)
        status = ProArrayObjectAdd ((ProArray*)p_cable_names, 
            PRO_VALUE_UNUSED, 1, prodev_names[i]);
    
    return (status);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableLocationsGet
  Purpose  : Finds the locations for the specified cable or wire
\*---------------------------------------------------------------------------*/
ProError ProCblCableLocationsGet (
    ProCblCable  *p_cable,             /* In: cable handle */
    int          **p_location_array,   /* Out: ProArray of locations ids */
    int          *n_locations)         /* Out: Locations number */
{
    ProError error;
	int i;
	ProCable cable;
	ProCablelocation* p_cable_loc_array;
    
    if (p_cable == NULL || p_location_array == NULL || n_locations == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);
	error = ProCableLocationsCollect(p_cable->p_owner->owner, &cable, &p_cable_loc_array);
    TEST_CALL_REPORT("procbl_get_cable_locations", 
               "ProCblCableLocationsGet",(ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

	ProArraySizeGet(p_cable_loc_array, n_locations);

    ProArrayAlloc (*n_locations, sizeof (int), 1, (ProArray*)p_location_array);
    for (i = 0; i < *n_locations; i++)
        (*p_location_array)[i] = p_cable_loc_array[i].id;

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblLocationCablesGet
  Purpose  : Find cables at the specified location
\*---------------------------------------------------------------------------*/
ProError ProCblLocationCablesGet (
    ProCblHarness  *p_harness,             /* In: harness handle */
    int            loc_id,                 /* In: location id */
    ProCblCable    **p_cables,             /* Out: ProArray of cables handles*/
    int            *p_n_cables)            /* Out: cables num */
{
    int i;
	ProError status;
	ProCablelocation p_loc;
	ProCable* cables;

    if (p_cables == NULL || p_harness == NULL || p_n_cables == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProModelitemInit(p_harness->harness, loc_id, PRO_CABLE_LOCATION, &p_loc);
	status = ProCablelocationCablesGet(&p_loc, &cables);

	ProArraySizeGet(cables, p_n_cables);

    TEST_CALL_REPORT("procbl_get_location_cables", 
               "ProCblLocationCablesGet",(ProError)status, 0);
    if (status != PRO_TK_NO_ERROR)
        return (status);
    ProArrayAlloc (*p_n_cables, sizeof (ProCblCable), 1, 
        (ProArray*)p_cables);
    for (i = 0; i < *p_n_cables; i++)
    {
		ProName cable_name;

		status = ProCableNameGet(&cables[i], cable_name);
        TEST_CALL_REPORT("procbl_get_cable_id", 
            "ProCblLocationCablesGet",(ProError)status, 0);
        if (status != PRO_TK_NO_ERROR)
            break;
        ProCblCableInit (cables[i].id, cable_name, p_harness,
            &(*p_cables)[i]);
    }
    if (status != PRO_TK_NO_ERROR)
    {
        ProArrayFree ((ProArray*)p_cables);
        return (PRO_TK_GENERAL_ERROR);
    }
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableGeomGet
  Purpose  : Retrieves cable geometry
\*---------------------------------------------------------------------------*/
ProError ProCblCableGeomGet (
    ProCblCable    *p_cable,           /* In: cable handle */
    ProCablesegment  **p_segs)     /* Out: cable geom */
                                       /* The function allocates the memory for
                     this argument, and reuses it on subsequent calls.
                     You should not free the memory or keep a pointer to it.*/
{
    ProError error;
	ProCable cable;

    if (p_cable == NULL || p_segs ==  NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_cable->p_owner->harness, p_cable->id, PRO_CABLE, &cable);

	error = ProCableSegmentsGet(&cable, p_segs);
	
    TEST_CALL_REPORT("procbl_get_cable_geom ", 
               "ProCblCableGeomGet",(ProError)error, 0);
    if (error != 0)
        return (PRO_TK_GENERAL_ERROR);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblCableClearance
  Purpose  : Determines the clearance of the specified cable, and reports if
             interference was found
\*---------------------------------------------------------------------------*/
ProError ProCblCableClearance (
    ProSelection      sel1,        /* In: first selection */
    ProSelection      sel2,        /* In: second selection */
    ProBoolean   *interference,    /* Out: interference */
    ProPoint3d      pnt_arr[2])    /* Out: The coordinates of the closest 
                                      points*/
{
    ProError error;
    ProBoolean interf;

    if (interference == NULL)
        return (PRO_TK_BAD_INPUTS);
        
	error = ProCableClearanceCompute(sel1, sel2, &interf, pnt_arr);
    TEST_CALL_REPORT("procbl_cable_clearance", 
               "ProCblCableClearance",(ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (PRO_TK_GENERAL_ERROR);
    if (interf == 1)
        *interference = PRO_B_FALSE;
    else
        *interference = PRO_B_TRUE;
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
    Functions to manage connectors
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorInit
  Purpose  : Initializes connector structure
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorInit (
    ProAsmcomppath  *path,	     /* In : Assembly component path */
    ProCblConnector   *p_connector)  /* Out: pointer to connector
                                        structure to initialize */
{
	int i, j;
	int ierr, num_connectors = 0;
	ProConnector* p_cons;

	if (!path)
		return (PRO_TK_BAD_INPUTS);
	*p_connector = *(ProCblConnector*)path;

    ierr = ProAssemblyConnectorsGet((ProAssembly)path->owner, &p_cons);
    TEST_CALL_REPORT("procbl_get_assy_connectors", 
		"ProCblConnectorInit", (ProError)ierr, 0);

    ProArraySizeGet (p_cons,&num_connectors);
    for (i=0; i<num_connectors; i++)
    {
	  for (j=0; j<p_cons[i].table_num; j++)
	    if (path->comp_id_table[j] != p_cons[i].comp_id_table[j])
		  break;

      if (j>=p_cons[i].table_num)	
	    break;
    }

    TEST_CALL_REPORT("pro_release_memb_id_tab", 
		"ProCblConnectorInit", (ProError)0, 0);
    if (i<num_connectors)
	  return (PRO_TK_NO_ERROR);
    else
	  return (PRO_TK_E_NOT_FOUND);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorParameterGet
  Purpose  : Gets connector parameter
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorParameterGet (
    ProCblConnector     *p_connector, /* In : Connector's handle */
    ProName              w_param_name,/* In : Parameter's name */
    ProCableparam *p_param)     /* Out: Parameter's handle */
{
    ProError error;

    if (p_connector == NULL || p_param == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorParamGet(p_connector, w_param_name, p_param);
    TEST_CALL_REPORT("procbl_get_connector_param", 
          "ProCblConnectorParameterGet",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (PRO_TK_GENERAL_ERROR);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorParameterDelete
  Purpose  : Deletes parameter of specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorParameterDelete (
    ProCblConnector   *p_connector,     /* In : Connector's handle*/
    ProName            w_param_name)    /* In : Parameter name */
{
    ProError error;

    if (p_connector == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorParamDelete(p_connector, w_param_name);
    TEST_CALL_REPORT("procbl_delete_connector_param ", 
          "ProCblConnectorParameterDelete",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (PRO_TK_GENERAL_ERROR);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorParametersGet
  Purpose  : Retrives parameters of specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorParametersGet (
    ProCblConnector      *p_connector,/* In : connector's handle*/
    ProCableparam **p_param)    /* Out: ProArray of params*/

{
	ProError error;

	if (p_connector == NULL)
		return (PRO_TK_BAD_INPUTS);

	error = ProConnectorParamsCollect(p_connector, p_param);
	TEST_CALL_REPORT("procbl_get_connector_params", 
		"ProCblConnectorParametersGet",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (PRO_TK_GENERAL_ERROR);
	return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorParametersSet
  Purpose  : Sets parameter of specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorParametersSet (
    ProCblConnector *p_connector,  /* In : connector's handle*/
    ProCableparam *p_param)  /* In : handles of parameter's*/
{
    ProError error;

    if (p_connector == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorParamsSet(p_connector, p_param);
    TEST_CALL_REPORT("procbl_set_connector_params ", 
          "ProCblConnectorParametersSet",(ProError)error, 0);
    if (error != PRO_TK_NO_ERROR)
        return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorFromLogical
  Purpose  : Returns list of imported, but not designated connectors
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorFromLogical (
    ProAssembly asm_mdl,        /* In : The cabling  assembly's handle */
    ProName    **p_w_name,      /* Out: list of log. con. ProArray, 
                                        to free call ProArrayFree */
    int *con_num)               /* Out: number of connectores */
{
	ProError status;

	if (asm_mdl == NULL)
		return (PRO_TK_BAD_INPUTS);

	status = ProConnectorsFromLogicalGet(asm_mdl, p_w_name);
	TEST_CALL_REPORT("procbl_get_connectors_from_logical", 
		"ProCblConnectorFromLogical",(ProError)status, 0);
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(*p_w_name, con_num);

	return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblAssemblyConnectorsGet
  Purpose  : Collect all designated connectors 
\*---------------------------------------------------------------------------*/
ProError ProCblAssemblyConnectorsGet (
    ProAssembly       assy,    /* In: The handle of cabling's assembly. */
    ProCblConnector **p_cons,  /* Out: ProArray of connectors if successfull,
                                       to free call ProArrayFree */
    int              *num_cons)/* Out: Number of retrieved connectors*/
{
	ProError status;

    if (assy == NULL || p_cons == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProAssemblyConnectorsGet(assy, (ProConnector**)p_cons);
    TEST_CALL_REPORT("procbl_get_assy_connectors ", 
                   "ProCblAssemblyConnectorsGet",(ProError)status, 0);
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(*p_cons, num_cons);

	return (PRO_TK_NO_ERROR);

}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorDesignate
  Purpose  : Designate specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorDesignate (
    ProCblConnector  *p_connector,    /* In: connector to designate */
    ProName          name)            /* In: The reference designator 
                                         of the logical connector,
                                         can be NULL */
{
    ProError error;

    if (p_connector == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorDesignate((ProAsmcomppath*)p_connector, name);
    TEST_CALL_REPORT("procbl_designate_connector", 
             "ProCblConnectorDesignate",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (error);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorUndesignate
  Purpose  : Undesignate specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorUndesignate (
    ProCblConnector  *p_connector)    /* In: connector to undesignate */
{
    ProError error;

    if (p_connector == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorUndesignate(p_connector);
    TEST_CALL_REPORT("procbl_undesignate_connector", 
             "ProCblConnectorUndesignate",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (error);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblConnectorEntryPortsGet
  Purpose  : Retrieves the entry ports for the specified connector
\*---------------------------------------------------------------------------*/
ProError ProCblConnectorEntryPortsGet (
    ProCblConnector     *p_connector,      /* In: connector handle */
    int                 **p_csys_id_array, /* Out: ProArray of Csys ids */
    int                 *p_num_csys)       /* Out: csys number */
{
    ProError error;
    int i;
	ProCsys* p_csys_array;

    if (p_connector == NULL || p_csys_id_array == NULL || p_num_csys == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProConnectorEntryPortsGet(p_connector, &p_csys_array);
    TEST_CALL_REPORT("procbl_get_connector_entry_ports", 
               "ProCblConnectorEntryPortsGet",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);

	ProArraySizeGet(p_csys_array, p_num_csys);
    ProArrayAlloc (*p_num_csys, sizeof (int), 1, (ProArray*)p_csys_id_array);

    for (i = 0; i < *p_num_csys; i++)
	{
		int id;
		error = ProCsysIdGet(p_csys_array[i], &id);
        	(*p_csys_id_array)[i] = id;
	}

    return (PRO_TK_NO_ERROR);
}


/*---------------------------------------------------------------------------*\
    Functions to manage harnesses.
\*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*\
  Function : ProCblAssemblyHarnessesGet
  Purpose  : Retrive harnesses
\*---------------------------------------------------------------------------*/
ProError ProCblAssemblyHarnessesGet (
    ProAssembly     asm_mdl,       /* In : assembly's handle */
    ProCblHarness **p_harness,     /* Out: array of harness handles to fill.
                                           User must free this with
                                           ProArrayFree */
    int            *num_harness)   /* Out: Number of harness*/
{
    int i;
    ProError status;
    ProName name;
    ProHarness* harness;
  
    if (asm_mdl == NULL || p_harness == NULL || num_harness == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProAssemblyHarnessesCollect(asm_mdl, &harness);
    TEST_CALL_REPORT("ProAssemblyHarnessesCollect", 
          "ProCblAssemblyHarnessesGet",(ProError)status, 0);
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(harness, num_harness);
    status = ProArrayAlloc (*num_harness, sizeof(ProCblHarness), 1,
        (ProArray*)p_harness);

    for( i=0; i<*num_harness; i++)
    {
        ProMdlNameGet ((ProMdl)harness[i], name);
        status = ProCblHarnessInit ((ProPart)harness[i], name,
            asm_mdl, &(*p_harness)[i]);
    }

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessInit
  Purpose  : Initialize harness structure
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessInit (
    ProPart        harness_mdl,  /* In: harness part */
    ProName        name,         /* In: harness name */
    ProAssembly    asm_mdl,      /* In: harness owner */
    ProCblHarness  *p_harness)   /* Out: handle of harness to init */
{
    if (p_harness == NULL)
        return (PRO_TK_BAD_INPUTS);
    p_harness->harness = harness_mdl;
    ProUtilWstrcpy (p_harness->name, name);
    p_harness->owner = asm_mdl;

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessCreate
  Purpose  : Creates new harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessCreate (
    ProAssembly asm_mdl,         /* In : assembly's handle */
    ProName w_name,              /* In : harness name */
    ProCblHarness *p_harness)    /* Out: handle of harness to fill */
{
    ProError status;
    ProHarness harness;

    if (asm_mdl == NULL || p_harness == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProHarnessCreate(asm_mdl, w_name, &harness);
    TEST_CALL_REPORT("procbl_create_harness", 
             "ProCblHarnessCreate",(ProError)status, 0);

	if (status != PRO_TK_NO_ERROR)
		return (status);

    status = ProCblHarnessInit (harness, w_name, asm_mdl, p_harness); 
    return (status);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessNameGet
  Purpose  : Retrive the name of harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessNameGet (
    ProCblHarness *p_harness,    /* In : harness*/
    ProName       w_name)        /* Out: harness name*/
{
    if (p_harness == NULL)
        return (PRO_TK_BAD_INPUTS);
    ProUtilWstrcpy (w_name, p_harness->name);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessCablesGet 
  Purpose  : Get cables from specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessCablesGet (
    ProCblHarness *p_harness,    /* In : harness*/
    ProCblCable   **p_cables,    /* Out: ProArray of cables, (user must call
                                         ProArrayFree)*/
    int           *p_n_cables)     /* Out: number of cables */
{
    int i;
	ProError status;
	ProCable* cables;

    if (p_harness == NULL || p_cables == NULL || p_n_cables == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProHarnessCablesCollect(p_harness->harness, &cables);
    TEST_CALL_REPORT("procbl_get_harness_cables", 
              "ProCblHarnessCablesGet",(ProError)status, 0);

	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(cables, p_n_cables);
    ProArrayAlloc (*p_n_cables, sizeof (ProCblCable), 1, (ProArray*)p_cables);

    for (i = 0; i < *p_n_cables; i++)
    {
		ProName cable_name;

		status = ProCableNameGet(&cables[i], cable_name);
        TEST_CALL_REPORT("procbl_get_cable_id", 
            "ProCblHarnessCablesGet",(ProError)status, 0);
        if (status != PRO_TK_NO_ERROR)
            break;
        ProCblCableInit (cables[i].id, cable_name, p_harness,
            &(*p_cables)[i]);
    }
    if (status != PRO_TK_NO_ERROR)
    {
        ProArrayFree ((ProArray*)p_cables);
        return (status);
    }
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessLocationsGet
  Purpose  : Finds the locations for the specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessLocationsGet (
    ProCblHarness   *p_harness,      /* In: harness handle */
    int **p_location_array,          /* Out: ProArray of locations */
    int  *n_locations)               /* Out: locations number */ 
{
    int i;
	ProError status;
	ProCablelocation* p_cable_loc_array;

    if (p_harness == NULL || p_location_array == NULL || n_locations == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProHarnessLocationsCollect(p_harness->owner, p_harness->harness, &p_cable_loc_array);
    TEST_CALL_REPORT("procbl_get_harness_locations", 
              "ProCblHarnessLocationsGet",(ProError)status, 0); 
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(p_cable_loc_array, n_locations);

    ProArrayAlloc (*n_locations, sizeof (int), 1, (ProArray*)p_location_array);
    for (i = 0; i < *n_locations; i++)
        (*p_location_array)[i] = p_cable_loc_array[i].id;

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessLocationTypeGet
  Purpose  : Retrieve location type of specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessLocationTypeGet (
    ProCblHarness    *p_harness,        /* In: harness handle */
    int              location_id,       /* In: location id */
    ProCablelocationType  *type)          /*Out: location type */ 
{
    ProError error;
    ProCablelocation location;

    if (p_harness == NULL || type == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_harness->harness, location_id, PRO_CABLE_LOCATION, &location);
	error = ProCablelocationTypeGet(&location, type);
    TEST_CALL_REPORT("procbl_get_location_type", 
       "ProCblHarnessLocationTypeGet",(ProError)error, 0);  

	if (error != PRO_TK_NO_ERROR)
		return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblHarnessLocationPntGet
  Purpose  : Retrieve location point of specified harness
\*---------------------------------------------------------------------------*/
ProError ProCblHarnessLocationPntGet (
    ProCblHarness    *p_harness,        /* In: harness handle */
    int              location_id,       /* In: location id */
    ProPoint3d pnt,				/*Out: The cable location's point */
	ProVector tan)            /*Out: The cable location's vector */
{
    ProError error;
    ProCablelocation location;

    if (p_harness == NULL || pnt == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProModelitemInit(p_harness->harness, location_id, PRO_CABLE_LOCATION, &location);
	error = ProCablelocationPointGet(&location, pnt, tan);
    TEST_CALL_REPORT("procbl_get_location_pnt", 
       "ProCblHarnessLocationPntGet",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);
    return (PRO_TK_NO_ERROR);
}

    

/*---------------------------------------------------------------------------*\
    Functions to manage spools.
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
  Function : ProCblAssemblySpoolsGet
  Purpose  : Collect spools in specified assembly
\*---------------------------------------------------------------------------*/
ProError ProCblAssemblySpoolsGet (
    ProAssembly    asm_mdl,     /* In : assembly's handle*/
    ProCblSpool    **p_spool,   /* Out: pointer on array of spool's handle's
                                       to init.Function allocate memory for
                                       this argument, use ProArrayFree to free
                                       it.*/
    int            *num_spools) /* Out: number of retrieved spools*/
{
    ProError status;

    if (asm_mdl == NULL || p_spool == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProAssemblySpoolsCollect(asm_mdl, (ProSpool**)p_spool);
    TEST_CALL_REPORT("procbl_get_assy_spools", 
           "ProCblAssemblySpoolsGe",(ProError)status, 0);
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(*p_spool, num_spools);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolInit
  Purpose  : Initializes spool structure
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolInit (
    ProAssembly    asm_mdl,     /* In : assembly's handle*/
    ProName        w_name,      /* In : spool name*/
    ProCblSpool    *p_spool)    /* Out: handle of spool to init*/
{
    if (asm_mdl == NULL || p_spool == NULL)
        return (PRO_TK_BAD_INPUTS);
    p_spool->owner = asm_mdl;
    ProUtilWstrcpy (p_spool->name, w_name);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolNameGet
  Purpose  : returns spool name
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolNameGet (
    ProCblSpool    *p_spool,    /* In: handle of spool to init*/
    ProName        w_name)      /* Out: spool name*/
{
    ProUtilWstrcpy(w_name, p_spool->name);
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolCreate
  Purpose  : Creates new spool
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolCreate (
    ProAssembly assy,                  /* In : assembly's handle*/
    ProName     w_name,                /* In : harness name*/
    ProCableType cable_type,      /* In : cable type */
    ProBundleType sheath_type,    /* In : bundle type */
    ProCblSpool  *p_spool)             /* Out :user's memory */
{
    ProError status;

    if (assy == NULL || p_spool == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProSpoolCreate(assy, w_name, cable_type, sheath_type, (ProSpool*)p_spool);
    TEST_CALL_REPORT("procbl_create_spool", 
             "ProCblSpoolCreate",(ProError)status, 0);

	if (status != PRO_TK_NO_ERROR)
		return (status);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolParametersGet
  Purpose  : Retrieves parameters of specified spool
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolParametersGet (
    ProCblSpool *p_spool,             /* In : spool's handle*/
    ProCableparam **p_param)    /* Out: ProArray */
{
    ProError error;

    if (p_spool == NULL || p_param == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProSpoolParametersCollect((ProSpool*)p_spool, p_param);
    TEST_CALL_REPORT("procbl_get_spool_params", 
          "ProCblSpoolParametersGet",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (error);


    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolParameterGet
  Purpose  : Retrieves one parameter of specified spool
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolParameterGet (
    ProCblSpool *p_spool,            /* In : spool's handle*/
    ProName w_param_name,            /* In : parametr name */
    ProCableparam *p_param)    /* Out: user's memory */
{
    ProError error;

    if (p_spool == NULL || p_param == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProSpoolParameterGet((ProSpool*)p_spool, w_param_name, p_param);
    TEST_CALL_REPORT("procbl_get_spool_param ", 
            "ProCblSpoolParameterGet",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolParametersSet
  Purpose  : Set parameters for specified spool
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolParametersSet (
    ProCblSpool         *p_spool,/* In : spool*/
    ProCableparam *p_param)/* In : the ProArray of parameters*/
{
	ProError error;

	if (p_spool == NULL || p_param == NULL)
		return (PRO_TK_BAD_INPUTS);

	error = ProSpoolParametersSet((ProSpool*)p_spool, p_param );
	TEST_CALL_REPORT("procbl_set_spool_params", 
		"ProCblSpoolParametersSet",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);
	return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolParameterDelete
  Purpose  : Deletes parameter in specified spool
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolParameterDelete (
    ProCblSpool *p_spool,       /* In : spool*/
    ProName   param_name)       /* In : param name*/
{
    ProError error;

    if (p_spool == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProSpoolParameterDelete((ProSpool*)p_spool, param_name);
    TEST_CALL_REPORT("procbl_delete_spool_param", 
          "ProCblSpoolParameterDelete",(ProError)error, 0);
	
	if (error != PRO_TK_NO_ERROR)
		return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolFromLogicalGet
  Purpose  : Return list of imported, but not created spools
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolFromLogicalGet (
    ProAssembly assy,             /* In: assemlbly */
    ProName     **p_w_name,       /* Out:  ProArray of spool names*/
    int         *num_spool)       /* Out: number of spools */
{
    ProError status;

    if (assy == NULL || num_spool == NULL || p_w_name == NULL)
        return (PRO_TK_BAD_INPUTS);

	status = ProSpoolsFromLogicalGet(assy, p_w_name);
    TEST_CALL_REPORT("procbl_get_spools_from_logical", 
                "ProCblSpoolFromLogicalGet",(ProError)status, 0);                               
	
	if (status != PRO_TK_NO_ERROR)
		return (status);

	ProArraySizeGet(*p_w_name, num_spool);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblSpoolFromLogicalCreate
  Purpose  : creates spools from logical
\*---------------------------------------------------------------------------*/
ProError ProCblSpoolFromLogicalCreate (
    ProAssembly assy,                /* In: assembly's hndle*/
    ProName    *p_w_name,            /* In: spool_names */
    int        spool_num,            /* In: spool num */
    ProCblSpool   **p_spools)        /* Out: ProArray of spools
                                        user must free memory by ProArrayFree*/
{
    ProError status;
  
    if (assy == NULL || p_w_name == NULL || p_spools == NULL)
        return (PRO_TK_BAD_INPUTS);
 
    status = ProSpoolsFromLogicalCreate(assy, p_w_name, (ProSpool**) p_spools);
    TEST_CALL_REPORT("procbl_create_spools_from_logical", 
                "ProCblSpoolFromLogicalCreate",(ProError)status, 0);

    if (status != PRO_TK_NO_ERROR)
	return (status);

    
    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
    Functions to route cables.
\*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*\
  Function : ProCblRoutingStart
  Purpose  : Initializes routing procedure
\*---------------------------------------------------------------------------*/
ProError ProCblRoutingStart (
    ProCblCable    *p_cables,       /* In: ProArray of cables to route */
    void      **p_cable_route_data) /* Out: The cable routing data required for
                          subsequent function calls to route cables
                          through locations. The function allocates
                          the space for this argument. */
{
    ProError error;
    int cable_num, i;
	ProCable * cables;

    if (p_cables == NULL || p_cable_route_data == NULL)
        return (PRO_TK_BAD_INPUTS);

    ProArraySizeGet ((ProArray)p_cables, &cable_num);

	ProArrayAlloc (0, sizeof (ProCable), 1, (ProArray*)&cables);
	for (i = 0; i < cable_num; i++)
	{
		ProModelitem item;
		error = ProModelitemInit(p_cables[i].p_owner->harness, p_cables[i].id, PRO_CABLE, &item);
		error = ProArrayObjectAdd((ProArray*)&cables, i , 1, &item);
	}
	
	error = ProCableRoutingStart(p_cables[0].p_owner->owner, cables, p_cable_route_data);
    TEST_CALL_REPORT("procbl_routing_start", 
             "ProCblRoutingStart",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblRouteThruLocation
  Purpose  : Finds or creates the location, and routes cables through it
\*---------------------------------------------------------------------------*/
ProError ProCblRouteThruLocation (
    void      *p_cable_route_data, /* In: routing data */
    ProCablelocationref ref,       /* In: referances */
    ProCablelocation* p_location,  /* Out: The location. */
    ProCablelocation* second_axis_location) /* Out: If routing through an axis, the route generates two locations. Otherwise,this is NULL. */
{
    ProError error;

    if (p_cable_route_data == NULL || ref == NULL || p_location == NULL)
        return (PRO_TK_BAD_INPUTS);

	error = ProCableThruLocationRoute(p_cable_route_data, ref, p_location, second_axis_location);
    TEST_CALL_REPORT("procbl_route_thru_location", 
              "ProCblRouteThruLocation",(ProError)error, 0);

	if (error != PRO_TK_NO_ERROR)
		return (error);

    return (PRO_TK_NO_ERROR);
}

/*---------------------------------------------------------------------------*\
  Function : ProCblRoutingEnd
  Purpose  : Ends the cable routing
\*---------------------------------------------------------------------------*/
ProError ProCblRoutingEnd (
    void      **p_cable_route_data)   /* In: The address of the cable 
                                         routing data */
{
	ProError error;

	if (p_cable_route_data == NULL)
		return (PRO_TK_BAD_INPUTS);

	error = ProCableRoutingEnd(p_cable_route_data);
	TEST_CALL_REPORT("procbl_routing_end", 
		"ProCblRoutingEnd",(ProError)error, 0);
	if (error != PRO_TK_NO_ERROR)
		return (error);
	return (PRO_TK_NO_ERROR);
}

