

#define PRO_USE_VAR_ARGS

/*---------------------------------------------------------------------------*\
    Headers
\*---------------------------------------------------------------------------*/
#include "ProToolkit.h"
#include "ProEdge.h"
#include "ProTool.h"

#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <math.h>

#include "ProMenu.h"
#include "TestError.h"
#include "ProMessage.h"
#include "ProUtil.h"
#include "UtilCollect.h"
#include "TestCollect.h"
#include "PTApplsUnicodeUtils.h"



/*---------------------------------------------------------------------------*\
    Macros
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
    Data types
\*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*\
    Global variables
\*---------------------------------------------------------------------------*/
static wchar_t		w_msg_file[ PRO_PATH_SIZE ];
static FILE		*fp_out = NULL;
static struct object_type_name
    { char *p_name; /*ProType*/int type; } object_type_name[] = 
{
    { (char *)"PRO_TYPE_UNUSED", 	 PRO_TYPE_UNUSED     },
    { (char *)"PRO_ASSEMBLY", 		 PRO_ASSEMBLY	     },
    { (char *)"PRO_PART", 		 PRO_PART            },
    { (char *)"PRO_FEATURE", 		 PRO_FEATURE         },
    { (char *)"PRO_DRAWING", 		 PRO_DRAWING         },
    { (char *)"PRO_SURFACE", 		 PRO_SURFACE         },
    { (char *)"PRO_EDGE", 		 PRO_EDGE            },
    { (char *)"PRO_3DSECTION", 		 PRO_3DSECTION       },
    { (char *)"PRO_DIMENSION", 		 PRO_DIMENSION       },
    { (char *)"PRO_2DSECTION", 		 PRO_2DSECTION       },
    { (char *)"PRO_LAYOUT", 		 PRO_LAYOUT          },
    { (char *)"PRO_AXIS",		 PRO_AXIS            },
    { (char *)"PRO_CSYS", 		 PRO_CSYS            },
    { (char *)"PRO_REF_DIMENSION", 	 PRO_REF_DIMENSION   },
    { (char *)"PRO_GTOL", 		 PRO_GTOL            },
    { (char *)"PRO_DWGFORM", 		 PRO_DWGFORM         },
    { (char *)"PRO_SUB_ASSEMBLY", 	 PRO_SUB_ASSEMBLY    },
    { (char *)"PRO_MFG", 		 PRO_MFG             },
    { (char *)"PRO_QUILT", 		 PRO_QUILT           },
    { (char *)"PRO_CURVE", 		 PRO_CURVE	     },
    { (char *)"PRO_POINT", 		 PRO_POINT	     },
    { (char *)"PRO_NOTE", 		 PRO_NOTE	     },
    { (char *)"PRO_IPAR_NOTE", 		 PRO_IPAR_NOTE       },
    { (char *)"PRO_EDGE_START", 	 PRO_EDGE_START      },
    { (char *)"PRO_EDGE_END", 		 PRO_EDGE_END        },
    { (char *)"PRO_CRV_START", 		 PRO_CRV_START       },
    { (char *)"PRO_CRV_END", 		 PRO_CRV_END         },
    { (char *)"PRO_SYMBOL_INSTANCE",	PRO_SYMBOL_INSTANCE  },
    { (char *)"PRO_DRAFT_ENTITY", 	 PRO_DRAFT_ENTITY    },
    { (char *)"PRO_REPORT", 		 PRO_REPORT          },
    { (char *)"PRO_MARKUP", 		 PRO_MARKUP          },
    { (char *)"PRO_SKETCH_ENTITY", 	 PRO_SKETCH_ENTITY   },
    { (char *)"PRO_COMP_CRV", 		 PRO_COMP_CRV        },
    { (char *)"PRO_PARAMETER", 		 PRO_PARAMETER       },
    { (char *)"PRO_DIAGRAM_WIRE", 	 PRO_DIAGRAM_WIRE    },
    { (char *)"PRO_SIMP_REP", 		 PRO_SIMP_REP        },
    { (char *)"PRO_EXTOBJ", 		 PRO_EXTOBJ          },
    { (char *)"PRO_CONTOUR", 		 PRO_CONTOUR	     },
    { (char *)"PRO_GROUP",		 PRO_GROUP	     },
    { (char *)"PRO_FAMILY_TABLE", 	 PRO_FAMILY_TABLE    },
    { NULL,			 0		     }
};


static struct surface_type_name
    { char *p_name; /*ProSrftype*/int type; } surface_type_name[] = 
{
    { (char *)"PRO_SRF_NONE",	PRO_SRF_NONE  	},
    { (char *)"PRO_SRF_PLANE",	PRO_SRF_PLANE 	},
    { (char *)"PRO_SRF_CYL",	PRO_SRF_CYL   	},
    { (char *)"PRO_SRF_CONE",	PRO_SRF_CONE  	},
    { (char *)"PRO_SRF_TORUS",	PRO_SRF_TORUS 	},
    { (char *)"PRO_SRF_COONS",	PRO_SRF_COONS 	},
    { (char *)"PRO_SRF_SPL",	PRO_SRF_SPL   	},
    { (char *)"PRO_SRF_FIL",	PRO_SRF_FIL   	},
    { (char *)"PRO_SRF_RUL",	PRO_SRF_RUL   	},
    { (char *)"PRO_SRF_REV",	PRO_SRF_REV   	},
    { (char *)"PRO_SRF_TABCYL",	PRO_SRF_TABCYL 	},
    { (char *)"PRO_SRF_B_SPL",	PRO_SRF_B_SPL 	},
    { (char *)"PRO_SRF_FOREIGN",PRO_SRF_FOREIGN },
    { (char *)"PRO_SRF_CYL_SPL",PRO_SRF_CYL_SPL	},
    { NULL,		0		}
};


static struct feature_status_name
    { char *p_name; /*ProFeatStatus*/int type; } feature_status_name[] = 
{
    { (char *)"PRO_FEAT_INVALID",	      PRO_FEAT_INVALID	           },
    { (char *)"PRO_FEAT_ACTIVE",	      PRO_FEAT_ACTIVE              },
    { (char *)"PRO_FEAT_INACTIVE",	      PRO_FEAT_INACTIVE            },
    { (char *)"PRO_FEAT_FAMTAB_SUPPRESSED",   PRO_FEAT_FAMTAB_SUPPRESSED   },
    { (char *)"PRO_FEAT_SIMP_REP_SUPPRESSED", PRO_FEAT_SIMP_REP_SUPPRESSED },
    { (char *)"PRO_FEAT_PROG_SUPPRESSED",     PRO_FEAT_PROG_SUPPRESSED     },
    { (char *)"PRO_FEAT_SUPPRESSED",	      PRO_FEAT_SUPPRESSED          },
    { (char *)"PRO_FEAT_UNREGENERATED",	      PRO_FEAT_UNREGENERATED       },
    { NULL,			      0			           }
};


static struct edge_type_name
    { char *p_name; /*ProEnttype*/int type; } edge_type_name[] = 
{
    { (char *)"PRO_ENT_NONE",	  PRO_ENT_NONE	   },
    { (char *)"PRO_ENT_POINT",	  PRO_ENT_POINT	   },
    { (char *)"PRO_ENT_LINE",	  PRO_ENT_LINE	   },
    { (char *)"PRO_ENT_ARC",	  PRO_ENT_ARC	   },
    { (char *)"PRO_ENT_TXT",	  PRO_ENT_TXT      },
    { (char *)"PRO_ENT_ARROW",	  PRO_ENT_ARROW    },
    { (char *)"PRO_ENT_CIRCLE",	  PRO_ENT_CIRCLE   },
    { (char *)"PRO_ENT_SPLINE",	  PRO_ENT_SPLINE   },
    { (char *)"PRO_ENT_B_SPLINE", PRO_ENT_B_SPLINE },
    { (char *)"PRO_ENT_ELLIPSE",  PRO_ENT_ELLIPSE  },
    { (char *)"PRO_ENT_POLYGON",  PRO_ENT_POLYGON  },
    { (char *)"PRO_ENT_CMP_CRV",  PRO_ENT_CMP_CRV  },
    { NULL,		  0		   }
};


static struct tool_type_name
    { char *p_name; /*ProToolType*/int type; } tool_type_name[] = 
{
    { (char *)"PRO_TOOL_NONE" , PRO_TOOL_NONE },
    { (char *)"PRO_TOOL_DRILL",  PRO_TOOL_DRILL },
    { (char *)"PRO_TOOL_TURN", PRO_TOOL_TURN },
    { (char *)"PRO_TOOL_SAW" , PRO_TOOL_SAW },
    { (char *)"PRO_TOOL_MILL" , PRO_TOOL_MILL },
    { (char *)"PRO_TOOL_GROOVE" , PRO_TOOL_GROOVE },
    { (char *)"PRO_TOOL_NIBBLE" , PRO_TOOL_NIBBLE },
    { (char *)"PRO_TOOL_PUNCH", PRO_TOOL_PUNCH },
    { (char *)"PRO_TOOL_AUXILIARY" , PRO_TOOL_AUXILIARY },
    { (char *)"PRO_TOOL_FORM" , PRO_TOOL_FORM },
    { (char *)"PRO_TOOL_PUNPNT" , PRO_TOOL_PUNPNT },
    { (char *)"PRO_TOOL_AREA_NIB"  , PRO_TOOL_AREA_NIB },
    { (char *)"PRO_TOOL_MFG_REFINE" , PRO_TOOL_MFG_REFINE },
    { (char *)"PRO_TOOL_BORE" , PRO_TOOL_BORE },
    { (char *)"PRO_TOOL_TAP" , PRO_TOOL_TAP },
    { (char *)"PRO_TOOL_CSINK" , PRO_TOOL_CSINK },
    { (char *)"PRO_TOOL_REAM", PRO_TOOL_REAM },
    { (char *)"PRO_TOOL_CENTER_DRL" , PRO_TOOL_CENTER_DRL },
    { (char *)"PRO_TOOL_TURN_GRV" , PRO_TOOL_TURN_GRV },
    { (char *)"PRO_TOOL_MILL_THREAD", PRO_TOOL_MILL_THREAD },
    { (char *)"PRO_TOOL_TURN_THREAD" , PRO_TOOL_TURN_THREAD },
    { (char *)"PRO_TOOL_SHEAR" , PRO_TOOL_SHEAR },
    { (char *)"PRO_TOOL_SIDE_MILL" , PRO_TOOL_SIDE_MILL },
    { (char *)"PRO_TOOL_CMM_FIXED" , PRO_TOOL_CMM_FIXED },
    { (char *)"PRO_TOOL_CMM_ROTATED" , PRO_TOOL_CMM_ROTATED  },
    { (char *)"PRO_TOOL_BACK", PRO_TOOL_BACK },
    { NULL,	0 }
};

static struct dim_type_name
    { char *p_name; /*ProDimensiontype*/int type; } dim_type_name[] = 
{
    { (char *)"PRO_DIM_LIN" , PRODIMTYPE_LINEAR },
    { (char *)"PRO_DIM_RAD" , PRODIMTYPE_RADIUS },
    { (char *)"PRO_DIM_DIA" , PRODIMTYPE_DIAMETER },
    { (char *)"PRO_DIM_ANG" , PRODIMTYPE_ANGLE },
    { NULL,	PRODIMTYPE_UNKNOWN }
};

/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollection()
    Purpose:    On button function. 
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollection( 
    ProAppData p_appdata,
    int int_dummy)
{
    ProError	status;
    int		dummy;


    ProStringToWstring( w_msg_file, (char *)"gen_msg.txt" );

    fp_out = PTApplsUnicodeFopen( DAT_FILE_NAME, "a" );
    if( fp_out == NULL )
    	fp_out = stderr;

    status = ProMenuFileRegister( (char *)"COLLECTION", (char *)"collection.mnu", &dummy );
    if( status != PRO_TK_NO_ERROR )
	return -1;

    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Animation", 
	(ProMenubuttonAction)ProTestCollectAnimation, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Solid Csys", 
	(ProMenubuttonAction)ProTestCollectSolidCsys, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Solid Axis", 
	(ProMenubuttonAction)ProTestCollectSolidAxis, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Solid Quilts", 
	(ProMenubuttonAction)ProTestCollectSolidQuilts, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Solid Surfaces", 
	(ProMenubuttonAction)ProTestCollectSolidSurfaces, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Simp Reps", 
	(ProMenubuttonAction)ProTestCollectSolidSimpreps, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Solid Features", 
	(ProMenubuttonAction)ProTestCollectSolidFeatures, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Model Notes", 
	(ProMenubuttonAction)ProTestCollectModelNotes, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Mfg Tools", 
	(ProMenubuttonAction)ProTestCollectMfgTools, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Feature Geomitems", 
	(ProMenubuttonAction)ProTestCollectFeatureGeomitems, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Elements", 
	(ProMenubuttonAction)ProTestCollectElemtreeElements, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Curve Comps", 
	(ProMenubuttonAction)ProTestCollectCurveComponents, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Contours", 
	(ProMenubuttonAction)ProTestCollectContours, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Extobj", 
	(ProMenubuttonAction)ProTestCollectExtobj, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Find By Name", 
	(ProMenubuttonAction)ProTestFindByName, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Axis By Plane", 
	(ProMenubuttonAction)ProTestCollectSolidAxisByPlane, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Dimensions", 
	(ProMenubuttonAction)ProTestCollectDimensions, p_appdata, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"Done Collect", 
	(ProMenubuttonAction)ProMenuDelete, NULL, 0 );
    ProMenubuttonActionSet( (char *)"COLLECTION", (char *)"COLLECTION", 
	(ProMenubuttonAction)ProMenuDelete, NULL, 0 );

    ProMenuCreate( PROMENUTYPE_MAIN, (char *)"COLLECTION", &dummy );
    ProMenuProcess( (char *)"COLLECTION", &dummy );

    fclose( fp_out );

    return (0);
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectAnimation()
    Purpose:    On button function. 
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectAnimation( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMatrix	    m_frame_view;
    ProMatrix	    m_transform;
    ProAnimFrame	anim_frame;
    ProAnimObj	anim_object;
    ProSelection   *p_selection;
    int		n_selected = 0;
    ProMdl	model;
    ProModelitem	model_item;
    int		i_frame;
    int		i_obj;
    double	add_angle, angle;
    ProAnimMovie	animation_movie;
    ProAnimFrame	*p_frames;
    int		n_frames;
    ProAnimObj	*p_objs;
    int		n_objs;
    int i, j;

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 4; j++)
        {
            if (i == j)
            {
                m_frame_view[i][j] = 1.0;
                m_transform[i][j] = 1.0;
            }
            else
            {
                m_frame_view[i][j] = 0.0;
                m_transform[i][j] = 0.0;
            }
        }
    }
    /* Print the header */
    ProTKFprintf( fp_out, "\n# Animation frames & objects\n" );

    /* Select objects to animate */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select objects to animate" );
    status = ProSelect( (char *)"part", -1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    if( (status != PRO_TK_NO_ERROR) || (n_selected < 1) )
	return -1;

    /* Get the model item from a selection object. */
    status = ProSelectionModelitemGet( p_selection[0], &model_item );
    TEST_CALL_REPORT( "ProSelectionModelitemGet()", "ProTestCollectAnimation()",
	status, status != PRO_TK_NO_ERROR);
    if( status == PRO_TK_NO_ERROR )
    {
	/* Retrieve the model the owns the specified item. */
	status = ProModelitemMdlGet( &model_item, &model );
	TEST_CALL_REPORT( "ProModelitemMdlGet()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
    }

    if( status != PRO_TK_NO_ERROR )
	return -1;

    /* Create the anim movie */
    status = ProAnimmovieCreate( model, &animation_movie );
    TEST_CALL_REPORT( "ProAnimmovieCreate()", "ProTestCollectAnimation()",
	status, status != PRO_TK_NO_ERROR);
    if( status != PRO_TK_NO_ERROR )
	return -1;

    /* Init rotate angle */
    angle = 0.0;
    add_angle = 360.0;
    add_angle /= (double)FRAME_NUM;

    /* Create animation objects and add it to the frames, add frames to movie*/
    for( i_frame=0; i_frame<FRAME_NUM; i_frame++ )
    {
	/* Create an animation frame */
	status = ProAnimframeCreate( m_frame_view, &anim_frame );
    	TEST_CALL_REPORT( "ProAnimframeCreate()", "ProTestCollectAnimation()",
                                status, status != PRO_TK_NO_ERROR);
	if( status != PRO_TK_NO_ERROR )
	    break;

	/* Add rotation to the transformation matrix */
	angle += add_angle;
	ProUtilXRot( angle, m_transform );

	for( i_obj=0; i_obj<n_selected; i_obj++ )
	{
	    /* Create an animation object... */
	    status = ProAnimobjectCreate( p_selection[i_obj], m_transform, 
		&anim_object );
    	    TEST_CALL_REPORT( "ProAnimobjectCreate()", 
		"ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);

	    if( status == PRO_TK_NO_ERROR )
	    {
		/* ...and add it to the frame */
		status = ProAnimframeObjAdd( anim_frame, anim_object );
    	    	TEST_CALL_REPORT( "ProAnimframeObjAdd()", 
			"ProTestCollectAnimation()",
			status, status != PRO_TK_NO_ERROR);
	    }
	}

	status = ProAnimmovieFrameAdd( animation_movie, anim_frame );
	TEST_CALL_REPORT( "ProAnimmovieFrameAdd()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
    }

    /* Start a batch animation process */
    status = ProBatchAnimationStart( animation_movie, NULL, NULL );
    TEST_CALL_REPORT( "ProBatchAnimationStart()", "ProTestCollectAnimation()",
	status, status != PRO_TK_NO_ERROR);


    n_frames = 0;
    p_frames = NULL;
    status = ProUtilCollectAnimFrames( animation_movie, &p_frames );
    if( status == PRO_TK_NO_ERROR )
    {
	status = ProArraySizeGet( p_frames, &n_frames );
    	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
    }

    ProTKFprintf( fp_out, "Number of anim frames: %d\n", n_frames );
    ProTKFprintf( fp_out, "Frame #\tNumber of objs in frame\n" );

    /* Delete frames */
    for( i_frame=0; i_frame<n_frames; i_frame++ )
    {
	n_objs = 0;
	p_objs = NULL;
	status = ProUtilCollectAnimObjects( p_frames[i_frame], &p_objs );
	if( status == PRO_TK_NO_ERROR )
	{
	    status = ProArraySizeGet( p_objs, &n_objs );
    	    TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
	}
	
	/* Print the num of objs per frame */
	ProTKFprintf( fp_out, "%d\t%d\n", i_frame, n_objs );

	for( i_obj=0; i_obj<n_objs; i_obj++ )
	{
	    status = ProAnimframeObjRemove( p_frames[i_frame], p_objs[i_obj] );
    	    TEST_CALL_REPORT( "ProAnimframeObjRemove()", 
		"ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);

	    status = ProAnimobjectDelete( p_objs[i_obj] );
    	    TEST_CALL_REPORT( "ProAnimobjectDelete()", 
		"ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
	}
	if( p_objs != NULL )
	{
	    status = ProArrayFree( (ProArray*)&p_objs );
    	    TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
	}
	
	/* Remove frame from the movie */
	status = ProAnimmovieFrameRemove( animation_movie, p_frames[i_frame] );
	TEST_CALL_REPORT( "ProAnimmovieFrameRemove()", 
		"ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);

	/* Delete frame */
	status = ProAnimframeDelete( p_frames[i_frame] );
	TEST_CALL_REPORT( "ProAnimframeDelete()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
    }

    /* Delete movie */
    status = ProAnimmovieDelete( animation_movie );
    TEST_CALL_REPORT( "ProAnimmovieDelete()", "ProTestCollectAnimation()",
	status, status != PRO_TK_NO_ERROR);

    if( p_frames != NULL )
    {
	status = ProArrayFree( (ProArray*)&p_frames );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectAnimation()",
		status, status != PRO_TK_NO_ERROR);
    }


    return (0);
}



/*=========================================================================*\
    Function:	ProUtilSelectedSurfaceGet()
    Purpose:	Retrieves a handle of the selected surface from 
		the selection object.
    Returns:	PRO_TK_NO_ERROR - The function successfully get 
				    the surface.
		PRO_TK_GENERAL_ERROR - Error.
    Note:	
\*=========================================================================*/
ProError ProUtilSelectedSurfaceGet( 
    ProSelection    selection,	    /* In:  Selection object */
    ProSurface	    *p_surface )    /* In:  Selected surface */
{
    ProModelitem    model_item;
    ProError	    status;


    /* Check out the input parameters */
    if( p_surface == NULL )
	return PRO_TK_GENERAL_ERROR;

    /* Get the model item from a selection object. */
    status = ProSelectionModelitemGet( selection, &model_item );
    TEST_CALL_REPORT( "ProSelectionModelitemGet()", 
	"ProUtilSelectedSurfaceGet()",
	status, status != PRO_TK_NO_ERROR);
    if( status == PRO_TK_NO_ERROR )
    {
	if( model_item.type == PRO_SURFACE )
	{
	    /* Retrieve the surface. */
	    status = ProGeomitemToSurface( &model_item, p_surface );
	    TEST_CALL_REPORT( "ProGeomitemToSurface()", 
	    	"ProUtilSelectedSurfaceGeton()",
		status, status != PRO_TK_NO_ERROR);
	    if( status == PRO_TK_NO_ERROR )
		return PRO_TK_NO_ERROR;
	}
    }


    return (PRO_TK_GENERAL_ERROR);
}



/*=========================================================================*\
    Function:	ProUtilSelectedCurveGet()
    Purpose:	Retrieves a handle of the selected surface from 
		the selection object.
    Returns:	PRO_TK_NO_ERROR - The function successfully get 
				    the surface.
		PRO_TK_GENERAL_ERROR - Error.
    Note:	
\*=========================================================================*/
ProError ProUtilSelectedCurveGet( 
    ProSelection    selection,	    /* In:  Selection object */
    ProCurve	    *p_curve	    /* In:  Selected surface */
)
{
    ProModelitem    model_item;
    ProError	    status;


    /* Check out the input parameters */
    if( p_curve == NULL )
	return PRO_TK_GENERAL_ERROR;

    /* Get the model item from a selection object. */
    status = ProSelectionModelitemGet( selection, &model_item );
    TEST_CALL_REPORT( "ProSelectionModelitemGet()", "ProUtilSelectedCurveGet()",
	status, status != PRO_TK_NO_ERROR);
    if( status == PRO_TK_NO_ERROR )
    {
	if( model_item.type == PRO_CURVE )
	{
	    /* Retrieve the curve. */
	    status = ProGeomitemToCurve( &model_item, p_curve );
	    TEST_CALL_REPORT( "ProGeomitemToCurve()", 
	    	"ProUtilSelectedCurveGet()",
		status, status != PRO_TK_NO_ERROR);
	    if( status == PRO_TK_NO_ERROR )
		return PRO_TK_NO_ERROR;
	}
    }


    return (PRO_TK_GENERAL_ERROR);
}


/*=========================================================================*\
    Function:	ProUtilXRot()
    Purpose:	Add x rotation values to the transformation matrix.
    Returns:	None
    Note:	Only the rotation components of the matrix are modified.
\*=========================================================================*/
void ProUtilXRot( 
    double angle,	/* (In)	    The rotation angle */
    ProMatrix mx )	/* (Out)    The transformation matrix */
{
    mx[0][0] = 1.0;
    mx[0][1] = mx[0][2] = mx[1][0] = mx[2][0] = 0.0;
    mx[1][1] = mx[2][2] = cos( angle * PI / 180.0);
    mx[1][2] = sin( angle * PI / 180.0);
    mx[2][1] = - mx[1][2];
}



/*~~=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidCsys()
    Purpose:    On button function. Test csys collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidCsys( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProCsys	    *p_csys;
    ProGeomitem	    geom_item;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid CSYS\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid csys */
    status = ProUtilCollectSolidCsys( (ProSolid)p_model, &p_csys );

    /* Print out obtained csys */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_csys, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectSolidCsys()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the csys array */
	ProTKFprintf( fp_out, "Number of solid csys:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the csys name */
	    status = ProCsysToGeomitem( (ProSolid)p_model, p_csys[i], &geom_item );
	    TEST_CALL_REPORT( "ProCsysToGeomitem()", 
		"ProTestCollectSolidCsys()",
		status, status != PRO_TK_NO_ERROR);

	    status = ProModelitemNameGet( &geom_item, w_name );
	    TEST_CALL_REPORT( "ProModelitemNameGet()", 
		"ProTestCollectSolidCsys()",
		status, status != PRO_TK_NO_ERROR);

	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\n", name );
	}

	status = ProArrayFree( (ProArray*)&p_csys );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidCsys()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidAxis()
    Purpose:    On button function. Test axis collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidAxis( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProAxis	    *p_axis;
    ProGeomitem	    geom_item;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid axis\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid axis */
    status = ProUtilCollectSolidAxis( (ProSolid)p_model, &p_axis );

    /* Print out obtained axis */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_axis, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectSolidAxis()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the axis array */
	ProTKFprintf( fp_out, "Number of solid axis:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the axis name */
	    ProAxisToGeomitem( (ProSolid)p_model, p_axis[i], &geom_item );
	    ProModelitemNameGet( &geom_item, w_name );
	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\n", name );
	}

	status = ProArrayFree( (ProArray*)&p_axis );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidAxis()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidQuilts()
    Purpose:    On button function. Test axis collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidQuilts( 
    ProAppData p_appdata,
    int int_dummy )
{
#ifndef PT_PRODUCTS_BUILD
    ProError	    status;
    ProMdl	    p_model;
    ProQuilt	    *p_quilts;
    ProGeomitem	    geom_item;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid quilts\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid quilts */
    status = ProUtilCollectSolidQuilts( (ProSolid)p_model, &p_quilts );

    /* Print out obtained quilts */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_quilts, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectSolidQuilts()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the quilts array */
	ProTKFprintf( fp_out, "Number of solid quilts:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the quilt name */
	    status = ProQuiltToGeomitem( (ProSolid)p_model, p_quilts[i], &geom_item );
	    TEST_CALL_REPORT( "ProQuiltToGeomitem()", 
		"ProTestCollectSolidQuilts()",
		status, status != PRO_TK_NO_ERROR);

	    status = ProModelitemNameGet( &geom_item, w_name );
	    TEST_CALL_REPORT( "ProModelitemNameGet()", 
		"ProTestCollectSolidQuilts()",
		status, status != PRO_TK_NO_ERROR);

	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\n", name );
	}

	status = ProArrayFree( (ProArray*)&p_quilts );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidQuilts()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
#else
    return PRO_TK_E_NOT_FOUND;
#endif
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidSurfaces()
    Purpose:    On button function. Test surfaces collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidSurfaces( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProSurface	    *p_surfaces;
    int		    n;
    int		    i;
    ProSrftype	    surf_type;
    int		    surf_id;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid surfaces\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid surfaces */
    status = ProUtilCollectSolidSurfaces( (ProSolid)p_model, &p_surfaces );

    /* Print out obtained surfaces */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_surfaces, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectSolidSurfaces()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the surface array */
	ProTKFprintf( fp_out, "Number of solid surfaces:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the surface ID */
	    surf_id = -1;
	    status = ProSurfaceIdGet( p_surfaces[i], &surf_id );
	    TEST_CALL_REPORT( "ProSurfaceIdGet()", 
		"ProTestCollectSolidSurfaces()",
		status, status != PRO_TK_NO_ERROR);

	    /* Print out surf ID */
	    ProTKFprintf( fp_out, "ID %d:\t", surf_id );

	    /* Get the surface type */
	    status = ProSurfaceTypeGet( p_surfaces[i], &surf_type );
	    TEST_CALL_REPORT( "ProSurfaceTypeGet()", 
		"ProTestCollectSolidSurfaces()",
		status, status != PRO_TK_NO_ERROR);

	    /* Print out surf type */
	    ProTKFprintf( fp_out, "%s\n", ProUtilSurfaceTypeToString( surf_type ) );
	}

	status = ProArrayFree( (ProArray*)&p_surfaces );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidSurfaces()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilSurfaceTypeToString()
    Purpose:    Return the string with surface type
    Returns:	Surface type name
\*=========================================================================*/
char *ProUtilSurfaceTypeToString( 
    ProSrftype surf_type    /* In:  Surface type to convert to string */
)
{
    int		i;


    for( i=0; surface_type_name[i].p_name != NULL; i++ )
	if( surface_type_name[i].type == surf_type )
	    return surface_type_name[i].p_name;


    return ((char *)"unknown");
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidSimpreps()
    Purpose:    On button function. Test simpreps collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidSimpreps( 
    ProAppData p_appdata,
    int int_dummy )
{
#ifndef PT_PRODUCTS_BUILD
    ProError	    status;
    ProMdl	    p_model;
    ProSimprep	    *p_simpreps;
    ProName	    w_name;
    ProCharName	    name;
    ProSimprepdata  *p_simprep_data;
    int		    n;
    int		    i;

    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid simprep\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid simp reps */
    status = ProUtilCollectSolidSimpreps( (ProSolid)p_model, &p_simpreps );

    /* Print out obtained simp reps */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_simpreps, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectSolidSimpreps()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the simp reps array */
	ProTKFprintf( fp_out, "Number of simp reps:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the simp rep name */
	    status = ProSimprepdataGet( p_simpreps+i, &p_simprep_data );
	    TEST_CALL_REPORT( "ProSimprepdataGet()", 
		"ProTestCollectSolidSimpreps()",
		status, status != PRO_TK_NO_ERROR);

	    status = ProSimprepdataNameGet( p_simprep_data, w_name );
	    TEST_CALL_REPORT( "ProSimprepdataNameGet()", 
		"ProTestCollectSolidSimpreps()",
		status, status != PRO_TK_NO_ERROR);

	    ProWstringToString( name, w_name );

	    /* Print the simp rep name */
	    ProTKFprintf( fp_out, "%s\n", name );

	    /* Free the memory allocated by ProSimprepdataGet() */
	    status = ProSimprepdataFree( &p_simprep_data );
	    TEST_CALL_REPORT( "ProSimprepdataFree()", 
		"ProTestCollectSolidSimpreps()",
		status, status != PRO_TK_NO_ERROR);
	}

	status = ProArrayFree( (ProArray*)&p_simpreps );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidSimpreps()",
		status, status != PRO_TK_NO_ERROR);
    }

    return status;
#else
    return (PRO_TK_E_NOT_FOUND);
#endif
}


/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestFeatureArrayPrint
    Purpose:    write feature information to a file 
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestFeatureArrayPrint(
    ProFeature *p_features,	    /* In : list of features, allocated by 
					    ProArrayAlloc */
    FILE *fp)			    /* In : file to output */
{
    ProError	status;
    int		i, n;
    ProFeattype	feat_type;
    ProFeatStatus	feat_status;


    /* Get the array size */
    n = 0;
    status = ProArraySizeGet( p_features, &n );
    TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestFeatureArrayPrint()",
	status, status != PRO_TK_NO_ERROR);

    /* Print out the features array */
    ProTKFprintf( fp, "Number of features:\t%d\n", n );
    for( i=0; i<n; i++ )
    {
	status = ProFeatureTypeGet( p_features + i, &feat_type );
	TEST_CALL_REPORT( "ProFeatureTypeGet()", "ProTestFeatureArrayPrint()",
		status, status != PRO_TK_NO_ERROR);

	status = ProFeatureStatusGet( p_features + i, &feat_status );
	TEST_CALL_REPORT( "ProFeatureStatusGet()", "ProTestFeatureArrayPrint()",
		status, status != PRO_TK_NO_ERROR);

	ProTKFprintf( fp, "Id %d,\ttype %d,\tstatus: %s\n", 
	    p_features[i].id,
	    feat_type,
	    ProUtilFeatureStatusToString( feat_status ) );
    }
    return (0);
}

/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidFeatures()
    Purpose:    On button function. Test solid features collection. Test
		visible features collection. Test asm comps collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidFeatures( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProMdlType	    mdltype;
    ProFeature	    *p_features;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid features\n" );

    p_model = *((ProMdl*)p_appdata);

    /* Get solid features */
    status = ProUtilCollectSolidFeatures( (ProSolid)p_model, &p_features );

    /* Print out obtained features */
    if( status == PRO_TK_NO_ERROR )
    {
	ProTestFeatureArrayPrint(p_features, fp_out);
	status = ProArrayFree( (ProArray*)&p_features );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidFeatures()",
		status, status != PRO_TK_NO_ERROR);
    }

    /* Get visible features */
    status = ProUtilCollectSolidFeatvis( (ProSolid)p_model, &p_features );

    /* Print out obtained features */
    if( status == PRO_TK_NO_ERROR )
    {
	ProTestFeatureArrayPrint(p_features, fp_out);
	status = ProArrayFree( (ProArray*)&p_features );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidFeatures()",
		status, status != PRO_TK_NO_ERROR);
    }

    status = ProMdlTypeGet(p_model, &mdltype);
    if (mdltype == PRO_MDL_ASSEMBLY)
    {
	/* Get asm comp features */
	status = ProUtilCollectAsmcomp((ProAssembly) p_model,
	    (ProAsmcomp**)&p_features );

	/* Print out obtained features */
	if( status == PRO_TK_NO_ERROR )
	{
	    ProTestFeatureArrayPrint(p_features, fp_out);
	    status = ProArrayFree( (ProArray*)&p_features );
	    TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectSolidFeatures()",
		status, status != PRO_TK_NO_ERROR);
	}
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilFeatureStatusToString()
    Purpose:    Return the string with feature status
    Returns:	Surface type name
\*=========================================================================*/
char *ProUtilFeatureStatusToString( 
    ProFeatStatus feat_status    /* In:  Surface type to convert to string */
)
{
    int		i;

    for( i=0; feature_status_name[i].p_name != NULL; i++ )
	if( feature_status_name[i].type == feat_status )
	    return feature_status_name[i].p_name;


    return ((char *)"unknown");
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectModelNotes()
    Purpose:    On button function. Test solid notes collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectModelNotes( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProModelitem    *p_notes;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Model notes\n" );

    p_model = *((ProMdl*)p_appdata);
    if( p_model == NULL )
    	return -1;
    	
    /* Get solid notes */
    status = ProUtilCollectModelNotes( p_model, &p_notes );

    /* Print out obtained notes */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_notes, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectModelNotes()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the notes array */
	ProTKFprintf( fp_out, "Number of notes:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    status = ProModelitemNameGet( p_notes+i, w_name );
	    TEST_CALL_REPORT( "ProModelitemNameGet()", 
	    	"ProTestCollectModelNotes()",
		status, status != PRO_TK_NO_ERROR);
	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\n", name );
	}

	status = ProArrayFree( (ProArray*)&p_notes );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectModelNotes()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectMfgTools()
    Purpose:    On button function. Test manufacturing tools collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectMfgTools( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status = PRO_TK_NO_ERROR;
    ProMdl	    p_model;
    ProTool	    *p_tools;
    ProToolType	    tool_type;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Manufacturing tools\n" );

    p_model = *((ProMdl*)p_appdata);
    if( p_model == NULL )
    	return -1;

    /* Get solid tools */
    /*status =*/ ProUtilCollectMfgTools( (ProMfg)p_model, &p_tools );

    /* Print out obtained tools */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_tools, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectMfgTools()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the tools array */
	ProTKFprintf( fp_out, "Number of mfg tools:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get the tool type */
	    tool_type = PRO_TOOL_NONE;
	    status = ProToolTypeGet( p_tools+i, &tool_type );
	    TEST_CALL_REPORT( "ProToolTypeGet()", "ProTestCollectMfgTools()",
		status, status != PRO_TK_NO_ERROR);

	    ProTKFprintf( fp_out, "%s\n", ProUtilToolTypeToString( tool_type ) );
	}

	status = ProArrayFree( (ProArray*)&p_tools );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectMfgTools()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilToolTypeToString()
    Purpose:    Return the string with feature status
    Returns:	Surface type name
\*=========================================================================*/
char *ProUtilToolTypeToString( 
    ProToolType	tool_type		/* In:  Tool type */
)
{
    int		i;


    for( i=0; tool_type_name[i].p_name != NULL; i++ )
	if( tool_type_name[i].type == tool_type )
	    return tool_type_name[i].p_name;


    return ((char *)"unknown");
}


/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilDimTypeToString()
    Purpose:    Return the string with feature status
    Returns:	Surface type name
\*=========================================================================*/
char *ProUtilDimTypeToString( 
    ProDimensiontype	dim_type		/* In:  Dimension type */
)
{
    int		i;


    for( i=0; dim_type_name[i].p_name != NULL; i++ )
	if( dim_type_name[i].type == dim_type )
	    return dim_type_name[i].p_name;


    return ((char *)"unknown");
}


/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectFeatureGeomitems()
    Purpose:    On button function. Test feature geometry items collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectFeatureGeomitems( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProFeature	    feature;
    ProGeomitem	    *p_geomitems;
    ProGeomitemdata *p_geomitem_data;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Feature geomitems\n" );

    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the feature" );
    status = ProSelect( (char *)"feature", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", "ProTestCollectFeatureGeomitems()",
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    /* Get selected feature */
    if( ProSelectionModelitemGet( p_selection[0], &feature ) != 
	PRO_TK_NO_ERROR )
	return -1;

    /* Get solid geomitems */
    status = ProUtilCollectFeatureGeomitems( &feature, PRO_TYPE_UNUSED, 
	&p_geomitems );

    /* Print out obtained geomitems */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_geomitems, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", 
		"ProTestCollectFeatureGeomitems()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the geomitems array */
	ProTKFprintf( fp_out, "Number of geomitems:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    status = ProGeomitemdataGet( p_geomitems+i, &p_geomitem_data );
	    TEST_CALL_REPORT( "ProGeomitemdataGet()", 
	    	"ProTestCollectFeatureGeomitems()",
		status, status != PRO_TK_NO_ERROR);

	    status = ProModelitemNameGet( p_geomitems+i, w_name );
	    TEST_CALL_REPORT( "ProModelitemNameGet()", 
	    	"ProTestCollectFeatureGeomitems()",
		status, status != PRO_TK_NO_ERROR);

	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\t%s\n", name, 
		ProUtilGeomitemTypeToString( p_geomitem_data->obj_type ) );

	    status = ProGeomitemdataFree( &p_geomitem_data );
	    TEST_CALL_REPORT( "ProGeomitemdataFree()", 
	    	"ProTestCollectFeatureGeomitems()",
		status, status != PRO_TK_NO_ERROR);
	}

	status = ProArrayFree( (ProArray*)&p_geomitems );
	TEST_CALL_REPORT( "ProArrayFree()", 
		"ProTestCollectFeatureGeomitems()", 
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilGeomitemTypeToString()
    Purpose:    Return the string with geometry item type
    Returns:	Geometry item type name
\*=========================================================================*/
char *ProUtilGeomitemTypeToString( ProType item_type )
{
    int		i;


    for( i=0; object_type_name[i].p_name != NULL; i++ )
	if( object_type_name[i].type == item_type )
	    return object_type_name[i].p_name;


    return ((char *)"unknown");
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectElemtreeElements()
    Purpose:    On button function. Test element tree elements 
		collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectElemtreeElements( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProFeature	    feature;
    ProElement	    p_elem_tree;
    ElemtreeElement *p_elements;
    ProElemId	    element_id;
    ProValue        p_value;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Elements\n" );

    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the feature" );
    status = ProSelect( (char *)"feature", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", "ProTestCollectElemtreeElements()",
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    /* Get selected feature */
    if( ProSelectionModelitemGet( p_selection[0], &feature ) != 
	PRO_TK_NO_ERROR )
	return -1;

    status = ProFeatureElemtreeExtract( &feature, NULL, PRO_FEAT_EXTRACT_NO_OPTS, &p_elem_tree );
    TEST_CALL_REPORT( "ProFeatureElemtreeExtract()", 
    	"ProTestCollectElemtreeElements()",
	status, status != PRO_TK_NO_ERROR);
    if( status == PRO_TK_NO_ERROR )
	/* Get feature elements */
	status = ProUtilCollectElemtreeElements( p_elem_tree, NULL, 
	    &p_elements );

    /* Print out obtained elements */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_elements, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", 
		"ProTestCollectElemtreeElements()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the elements array */
	ProTKFprintf( fp_out, "Number of elements:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    /* Get element ID */
	    status = ProElementIdGet( p_elements[i].p_element, &element_id );
	    TEST_CALL_REPORT( "ProElementIdGet()", 
		"ProTestCollectElemtreeElements()",
		status, status != PRO_TK_NO_ERROR);

	    /* Get element value */
	    status = ProElementValueGet( p_elements[i].p_element, &p_value );
	    TEST_CALL_REPORT( "ProElementValueGet()", 
		"ProTestCollectElemtreeElements()",
		status, status != PRO_TK_NO_ERROR);

	    /* Print out element ID and value */
	    ProTKFprintf( fp_out, "ID: %d\t- %s\n", element_id, 
		ProUtilValueToString( p_value ) );
	}

	status = ProUtilElemtreeElementArrayFree( &p_elements );
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilValueToString()
    Purpose:    Convert ProValue to string
    Returns:	String with value. It is redefined on subsequent 
		calls to this function.
\*=========================================================================*/
char *ProUtilValueToString( ProValue p_value )
{
    ProError		status;
    ProValueData	value_data;
    static ProCharName	value_data_str;


    status = ProValueDataGet( p_value, &value_data );
    TEST_CALL_REPORT( "ProValueDataGet)", "ProUtilValueToString()",
	status, status != PRO_TK_NO_ERROR);

    switch( value_data.type )
    {
      case PRO_VALUE_TYPE_INT:
	ProTKSprintf( value_data_str, "%d", value_data.v.i );
	break;

      case PRO_VALUE_TYPE_DOUBLE:
	ProTKSprintf( value_data_str, "%f", value_data.v.d );
	break;

      case PRO_VALUE_TYPE_POINTER:
	ProTKSprintf( value_data_str, "pointer" );
	break;

      case PRO_VALUE_TYPE_STRING:
	ProTKSprintf( value_data_str, "%s", value_data.v.s );
	break;

      case PRO_VALUE_TYPE_WSTRING:
	ProWstringToString( value_data_str, value_data.v.w );
	break;

      case PRO_VALUE_TYPE_SELECTION:
	ProTKSprintf( value_data_str, "Selection" );
	break;

      default:
	ProTKSprintf( value_data_str, "unknown" );
    }


    return (value_data_str);
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectCurveComponents()
    Purpose:    On button function. Test curve components collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectCurveComponents( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProCurve	    p_curve;
    ProEnttype	    curve_type;
    CurveComponent  *p_components;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Curve components\n" );

    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the composite curve" );
    status = ProSelect( (char *)"curve", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", "ProTestCollectCurveComponents()",
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    /* Get selected model */
    if( ProUtilSelectedCurveGet( p_selection[0], &p_curve ) != PRO_TK_NO_ERROR )
	return -1;

    /* Selected curve must be composite */
    curve_type = (ProEnttype)-1;
    status = ProCurveTypeGet( p_curve, &curve_type );
    TEST_CALL_REPORT( "ProCurveTypeGet()", "ProTestCollectCurveComponents()",
	status, status != PRO_TK_NO_ERROR);
    if( curve_type != PRO_ENT_CMP_CRV )
	return -1;

    /* Get curve components */
    status = ProUtilCollectCurveComponents( p_curve, &p_components );

    /* Print out obtained components */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_components, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectCurveComponents()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the components array */
	ProTKFprintf( fp_out, "Number of curve components:\t%d\n", n );
	for( i=0; i<n; i++ )
	{
	    ProTKFprintf( fp_out, "Index %d%s\n", 
		p_components[i].index, 
		(p_components[i].flip == PRO_B_TRUE ? ", flipped" : "") );
	}

	status = ProArrayFree( (ProArray*)&p_components );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectCurveComponents()",
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectContours()
    Purpose:    On button function. Test surface contours collection
		and contour edges collection
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectContours( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProSurface	    p_surface;
    ProContour	    *p_contours;
    ProEdge	    *p_edges;
    char	    *p_trav_str;
    ProContourTraversal	traversal;
    int		    edge_id;
    double	    edge_len;
    ProEnttype	    edge_type;
    int		    n_contours;
    int		    n_edges;
    int		    i;
    int		    j;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Contours\n" );

    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the surface" );
    status = ProSelect( (char *)"surface", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", "ProTestCollectContours()",
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    /* Get selected model */
    if( ProUtilSelectedSurfaceGet( p_selection[0], &p_surface ) 
	!= PRO_TK_NO_ERROR )
	return -1;

    /* Get surface contours */
    status = ProUtilCollectSurfaceContours( p_surface, &p_contours );
    if( status != PRO_TK_NO_ERROR )
	return -1;

    /* Print out contours */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the contours array size */
	n_contours = 0;
	status = ProArraySizeGet( p_contours, &n_contours );
	TEST_CALL_REPORT( "ProArraySizeGet()", 
		"ProTestCollectContours()",
		status, status != PRO_TK_NO_ERROR);

	/* Print out the contours array */
	ProTKFprintf( fp_out, "Number of contours:\t%d\n", n_contours );
	for( i=0; i<n_contours; i++ )
	{
	    /* Internal or external contour */
	    status = ProContourTraversalGet( p_contours[i], &traversal );
	    TEST_CALL_REPORT( "ProContourTraversalGet()", 
		"ProTestCollectContours()",
		status, status != PRO_TK_NO_ERROR);
	    switch( traversal )
	    {
	      case PRO_CONTOUR_TRAV_INTERNAL: 
		p_trav_str = (char *)"Internal"; 
		break;

	      case PRO_CONTOUR_TRAV_NONE: 
		p_trav_str = (char *)"Erroneous"; 
		break;

	      case PRO_CONTOUR_TRAV_EXTERNAL: 
		p_trav_str = (char *)"External"; 
		break;

	      default: 
		p_trav_str = (char *)"Undefined"; 
		break;
	    }

	    /* Print out contour data */
	    ProTKFprintf( fp_out, "Contour: %s\n", p_trav_str );

	    /* Get the contour edges */
	    status = ProUtilCollectContourEdges( p_surface, p_contours[i], 
		&p_edges );

	    /* Print out obtained edges */
	    if( status == PRO_TK_NO_ERROR )
	    {
		/* Get the edges array size */
		n_edges = 0;
		status = ProArraySizeGet( p_edges, &n_edges );
		TEST_CALL_REPORT( "ProArraySizeGet()", 
			"ProTestCollectContours()",
			status, status != PRO_TK_NO_ERROR);

		/* Print out the edges array */
		ProTKFprintf( fp_out, "\t%d edges: \n", n_edges );
		for( j=0; j<n_edges; j++ )
		{
		    /* Get edge ID */
		    edge_id = -1;
		    ProEdgeIdGet( p_edges[j], &edge_id );

		    /* Get edge length */
		    edge_len = 0.0;
		    status = ProEdgeLengthEval( p_edges[j], &edge_len );
		    TEST_CALL_REPORT( "ProEdgeLengthEval()", 
			"ProTestCollectContours()",
			status, status != PRO_TK_NO_ERROR);

		    /* Get edge type */
		    edge_type = (ProEnttype)-1;
		    status = ProEdgeTypeGet( p_edges[j], &edge_type );
		    TEST_CALL_REPORT( "ProEdgeTypeGet()", 
			"ProTestCollectContours()",
			status, status != PRO_TK_NO_ERROR);

		    /* Print out edge data */
		    ProTKFprintf( fp_out, "\tID: %d,\tType: %s, \tLen: %f\n", 
			edge_id, 
			ProUtilEdgeTypeToString( edge_type ),
			edge_len );
		}

		status = ProArrayFree( (ProArray*)&p_edges );
		TEST_CALL_REPORT( "ProArrayFree()", 
			"ProTestCollectContours()",
			status, status != PRO_TK_NO_ERROR);
	    }
	}

	status = ProArrayFree( (ProArray*)&p_contours );
	TEST_CALL_REPORT( "ProArrayFree()", 
		"ProTestCollectContours()", 
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProUtilEdgeTypeToString()
    Purpose:    Return the string with edge type
    Returns:	Edge type name
\*=========================================================================*/
char *ProUtilEdgeTypeToString( 
    ProEnttype edge_type    /* In:  Edge type to convert to string */
)
{
    int		i;


    for( i=0; edge_type_name[i].p_name != NULL; i++ )
	if( edge_type_name[i].type == edge_type )
	    return edge_type_name[i].p_name;


    return ((char *)"unknown");
}



/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectExtobj()
    Purpose:    On button function. Test extobj collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectExtobj( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProExtobj	    *p_extobjs;
    ExtobjReference *p_extobj_refs;
    ProExtobjClass  extobj_class;
    int		    extobj_ref_type;
    int		    n_extobjs;
    int		    n_extobj_refs;
    int		    i;
    int		    j;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# External objects\n" );

    p_model = *((ProMdl*)p_appdata);
    if( p_model == NULL )
    	return -1;

    /* Init external object class */
    ProStringToWstring( extobj_class.name, (char *)"Mechanica" );
    extobj_class.type = 0;

    /* Get extobj */
    status = ProUtilCollectExtobj( p_model, &extobj_class,  &p_extobjs );

    /* Print out obtained extobjs */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n_extobjs = 0;
	status = ProArraySizeGet( p_extobjs, &n_extobjs );
	TEST_CALL_REPORT( "ProArraySizeGet()", 
		"ProTestCollectExtobj()", 
		status, status != PRO_TK_NO_ERROR);

	/* Print out the extobj array */
	ProTKFprintf( fp_out, "Number of external objects:\t%d\n", n_extobjs );
	for( i=0; i<n_extobjs; i++ )
	{
	    status = ProUtilCollectExtobjReferences( p_extobjs+i, 
		&p_extobj_refs );
	    if( status == PRO_TK_NO_ERROR )
	    {
		/* Get the array size */
		n_extobj_refs = 0;
		status = ProArraySizeGet( p_extobj_refs, &n_extobj_refs );
		TEST_CALL_REPORT( "ProArraySizeGet()", 
			"ProTestCollectExtobj()", 
			status, status != PRO_TK_NO_ERROR);

		/* Print out the extobj refs array */
		ProTKFprintf( fp_out, "\t%d external object references:\n", 
		    n_extobj_refs );
		for( j=0; j<n_extobj_refs; j++ )
		{
		    if( ProExtobjReftypeGet( p_extobj_refs[j].p_extobj_ref,
			&extobj_ref_type ) == PRO_TK_NO_ERROR )
		    ProTKFprintf( fp_out, "\tref type: %d\n", extobj_ref_type );
		}

		status = ProArrayFree( (ProArray*)&p_extobj_refs );
		TEST_CALL_REPORT( "ProArrayFree()", 
			"ProTestCollectExtobj()", 
			status, status != PRO_TK_NO_ERROR);
	    }
	}

	status = ProArrayFree( (ProArray*)&p_extobjs );
	TEST_CALL_REPORT( "ProArrayFree()", 
		"ProTestCollectExtobj()", 
		status, status != PRO_TK_NO_ERROR);
    }


    return status;
}


/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestFindByName()
    Purpose:    On button function. Test extobj collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestFindByName( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProMdl	    p_model;
    ProName	    name;
    ProModelitem    feature, geomitem;
    char	    c_name[PRO_NAME_SIZE];


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Features by name\n" );

    p_model = *((ProMdl*)p_appdata);
    if( p_model == NULL )
    	return -1;

    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Enter feature name" );
    if (ProMessageStringRead(PRO_NAME_SIZE, name)!= PRO_TK_NO_ERROR)
	return (-1);

    /* Find the feature */
    status = ProUtilFindFeatvisByName((ProSolid)p_model, name, &feature);

    if (status == PRO_TK_NO_ERROR)
    {
	status = ProModelitemNameGet(&feature, name);
	TEST_CALL_REPORT( "ProModelitemNameGet()", 
		"ProTestFindByName()", 
		status, status != PRO_TK_NO_ERROR);
	ProWstringToString(c_name, name);
	ProTKFprintf(fp_out, "Visible feature found:\t%s\n", c_name);
    }
    else 
        ProTKFprintf(fp_out, "Feature is not found\n");
    
    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the feature" );
    status = ProSelect( (char *)"feature", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", 
	"ProTestFindByName()", 
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Enter edge name" );
    if (ProMessageStringRead(PRO_NAME_SIZE, name)!= PRO_TK_NO_ERROR)
	return (-1);

    /* Get selected feature */
    status = ProSelectionModelitemGet(p_selection[0], &feature);
    TEST_CALL_REPORT( "ProSelectionModelitemGet()", 
	"ProTestFindByName()", 
	status, status != PRO_TK_NO_ERROR);

    /* Find the edge */
    status = ProUtilFindFeatureGeomitemByName(&feature,PRO_EDGE,name,&geomitem);

    if (status == PRO_TK_NO_ERROR)
    {
	status = ProModelitemNameGet(&geomitem, name);
	TEST_CALL_REPORT( "ProModelitemNameGet()", 
		"ProTestFindByName()", 
		status, status != PRO_TK_NO_ERROR);
	ProWstringToString(c_name, name);
	ProTKFprintf(fp_out, "Edge found:\t%s\n", c_name);
    }
    else 
        ProTKFprintf(fp_out, "Edge is not found\n");


    return status;
}


/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectDimensions()
    Purpose:    On button function. Test extobj collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectDimensions( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProMdl	    p_model;
    ProDimension    *p_dims;
    int             n_dims=0;
    int		    i;

    /* Print the header */
    ProTKFprintf( fp_out, "\n# Dimensions\n" );

    p_model = *((ProMdl*)p_appdata);
    if( p_model == NULL )
    	return -1;
    
    /* arg PRO_B_FALSE indicates that we collect standard dimensions */
    status = ProUtilCollectDimension(p_model, PRO_B_FALSE, &p_dims);
    if (status == PRO_TK_NO_ERROR)
    {
	/* Get the array size */
	status = ProArraySizeGet( p_dims, &n_dims );
	TEST_CALL_REPORT( "ProArraySizeGet()", "ProTestCollectDimensions()", 
		status, status != PRO_TK_NO_ERROR);

	/* Print out the dimension array */
	ProTKFprintf( fp_out, "Number of dimensions:\t%d\n", n_dims );
	for( i=0; i<n_dims; i++ )
	{
          ProName  name;
          ProDimensiontype type;
          double dim_value;

	  status = ProDimensionSymbolGet(&p_dims[i], name);
          TEST_CALL_REPORT( "ProDimensionSymbolGet()", "ProTestCollectDimensions()", 
		status, status != PRO_TK_NO_ERROR);
          status = ProDimensionTypeGet(&p_dims[i], &type);
          TEST_CALL_REPORT( "ProDimensionTypeGet()", "ProTestCollectDimensions()", 
		status, status != PRO_TK_NO_ERROR);
          status = ProDimensionValueGet(&p_dims[i], &dim_value);
          TEST_CALL_REPORT( "ProDimensionValueGet()", "ProTestCollectDimensions()", 
		status, status != PRO_TK_NO_ERROR);

	  ProTKFprintf(fp_out, "Dim %10s %20s %8.2f\n", name, ProUtilDimTypeToString(type), 
                       dim_value);
	}

	status = ProArrayFree( (ProArray*)&p_dims );
	TEST_CALL_REPORT( "ProArrayFree()", "ProTestCollectDimensions()", 
		status, status != PRO_TK_NO_ERROR);
    }
    return (PRO_TK_NO_ERROR);
}

/*=========================================================================*\
    File:	TestCollect.c
    Function:   ProTestCollectSolidAxisByPlane()
    Purpose:    On button function. Test axis collection.
    Returns:	0 if successful, non-zero otherwise.
\*=========================================================================*/
int ProTestCollectSolidAxisByPlane( 
    ProAppData p_appdata,
    int int_dummy )
{
    ProError	    status;
    ProSelection    *p_selection;
    int		    n_selected = 0;
    ProAxis	    *p_axis;
    ProSolid	    p_model;
    ProSurface	    surface;
    ProGeomitem	    geom_item, modelitem;
    ProName	    w_name;
    ProCharName	    name;
    int		    n;
    int		    i;


    /* Print the header */
    ProTKFprintf( fp_out, "\n# Solid axis by plane\n" );

    /* Select model */
    ProMessageDisplay( w_msg_file, (char *)"TEST %0s", (char *)"Select the plane" );
    status = ProSelect( (char *)"surface", 1, NULL, NULL, NULL, NULL, 
	&p_selection, &n_selected );
    TEST_CALL_REPORT( "ProSelect()", "ProTestCollectSolidAxisByPlane()", 
	status, status != PRO_TK_NO_ERROR);
    if( (status != PRO_TK_NO_ERROR) || (n_selected != 1) )
	return -1;

    /* Get selected surface */
    status = ProSelectionModelitemGet(p_selection[0], &modelitem);
    TEST_CALL_REPORT( "ProSelectionModelitemGet()", 
    	"ProTestCollectSolidAxisByPlane()", 
	status, status != PRO_TK_NO_ERROR );

    status = ProGeomitemToSurface(&modelitem, &surface);
    TEST_CALL_REPORT( "ProGeomitemToSurface()", 
    	"ProTestCollectSolidAxisByPlane()", 
	status, status != PRO_TK_NO_ERROR );


    /* Get solid axis */
    p_model = (ProSolid)modelitem.owner;
    status = ProUtilCollectSolidAxisByPlane(p_model, surface, 
	USER_PARALLEL,  &p_axis);

    /* Print out obtained axis */
    if( status == PRO_TK_NO_ERROR )
    {
	/* Get the array size */
	n = 0;
	status = ProArraySizeGet( p_axis, &n );
	TEST_CALL_REPORT( "ProArraySizeGet()", 
    		"ProTestCollectSolidAxisByPlane()", 
		status, status != PRO_TK_NO_ERROR );

	/* Print out the axis array */
	ProTKFprintf( fp_out, "Number solid axis parallel to selected plane:\t%d\n", 
		n );
	for( i=0; i<n; i++ )
	{
	    /* Get the axis name */
	    ProAxisToGeomitem( p_model, p_axis[i], &geom_item );
	    ProModelitemNameGet( &geom_item, w_name );
	    ProWstringToString( name, w_name );

	    ProTKFprintf( fp_out, "%s\n", name );
	}

	status = ProArrayFree( (ProArray*)&p_axis );
	TEST_CALL_REPORT( "ProArrayFree()", 
    		"ProTestCollectSolidAxisByPlane()", 
		status, status != PRO_TK_NO_ERROR );
    }


    return status;
}
