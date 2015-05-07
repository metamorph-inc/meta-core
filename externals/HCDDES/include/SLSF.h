#ifndef _ISIS_SLSF_PARADIGMS_H
#define _ISIS_SLSF_PARADIGMS_H


////////////////////////////////////////////////////////////////////////////////
// By default, PARADIGM will not be defined and the HCDDES project will 
// ignore non-ECSLDP paradigms.  However, non-ECSLDP versions of mdl2mga.exe  
// may be built by calling the MDL2MGA project's build with a specified 
// ${PARADIGM} target. 
//                  Chris.vanBuskirk@vanderbilt.edu
// 
//TODO: write an msbuild script with the supported 
//      non-ESMoL targets, which relies on the 
//      vanilla vStudio C++ projects that HCDDES uses.
//      In the meantime, build manually from vStudio
//      by  a) uncommenting the appropriate define
//      for PARADIGM below  &  
//      b) rebuild the MDL2MGA project using the gui.
//
//TODO: I can't test addtypes.exe with non-ecsldp paradigms
//      anymore since I upgraded to MATLAB 2007b.  Currently,
//      there are some hard-coded strings that are tied
//      to version 2007a.  Thus, I haven't checked in 
//      any SLSF mods for either AddTypes or the code generators.
//      (CPB 12-Oct-2007)
////////////////////////////////////////////////////////////////////////////////
#define ESMoL_PARADIGM   0
#define FCVMS_PARADIGM   1
#define SDR_PARADIGM     2
// CyPhyML Paradigm now obsolete
#define CyPhyML_PARADIGM 3
#define CyberComposition_PARADIGM 4

#ifdef USE_ESMoL
#define PARADIGM ESMoL_PARADIGM
#endif

#ifdef USE_FCVMS
#define PARADIGM FCVMS_PARADIGM
#undef FCVMS
#endif

#ifdef USE_SDR
#define PARADIGM SDR_PARADIGM
#endif

#ifdef USE_CyPhyML
#error "CyPhymL Paradigm no longer supported."
#endif

#ifdef USE_CyberComposition
#define PARADIGM CyberComposition_PARADIGM
#endif


#ifndef PARADIGM
#define PARADIGM ESMoL_PARADIGM
#endif

#if PARADIGM == ESMoL_PARADIGM

# include	<ESMoL.h>
  namespace SLSF				= ESMoL;
  namespace SLSF_ROOT			= ESMoL;
#define		SLSF_PARADIGM_NAME	"ESMoL"
#define		SLSF_MODEL_LEVEL	3

#elif PARADIGM == FCVMS_PARADIGM

# include	<FCSL.h>
  namespace SLSF				= FCSL::SLSF;
  namespace SLSF_ROOT			= FCSL;
# define	SLSF_PARADIGM_NAME	"FCSL"
# define    Dataflow_children	SLSF_Dataflow_children
# define    Stateflow_children	SLSF_Stateflow_children
# define    Types_children		SLSF_Types_children

#elif PARADIGM == SDR_PARADIGM

# include	<SCAML.h>
  namespace SLSF				= SCAML::SLSF;
  namespace SLSF_ROOT			= SCAML;
# define	SLSF_PARADIGM_NAME	"SCAML"
# define    Dataflow_children	SLSF_Dataflow_children
# define    Stateflow_children	SLSF_Stateflow_children
# define    Types_children		SLSF_Types_children

#elif PARADIGM == CyPhyML_PARADIGM

#elif PARADIGM == CyberComposition_PARADIGM

# include	<CyberComposition.h>
  namespace SLSF				= CyberComposition::Simulink;
  namespace SLSF_ROOT			= CyberComposition;
namespace CyberComposition { namespace Simulink {
	typedef SF_Junction Junction;
	typedef SF_Port Port;
	typedef SF_Parameter Parameter;
	typedef SF_TypeBase TypeBase;
	typedef SF_Matrix Matrix;
} }
#define		SLSF_PARADIGM_NAME	"CyberComposition"

#else

# include	<ESMoL.h>
  namespace SLSF				= ESMoL;
  namespace SLSF_ROOT			= ESMoL;
#define		SLSF_PARADIGM_NAME	"ESMoL"
#define		SLSF_MODEL_LEVEL	3

#endif


#endif
