/*! \file isis_include_ptc_headers.h
    \brief .h file that includes the necessary Creo headers.

	In addition to including the necessary Creo Toolkit headers, 
	this file wraps the includes in extern "C".  This is necessary to use the includes 
	with C++.
*/
#ifndef ISIS_INCLUDE_PTC_HEADERS_H
#define ISIS_INCLUDE_PTC_HEADERS_H

extern "C"
{
#include <ProToolkit.h>
#include <user_tk_error.h>

#include <ProCore.h>
#include <ProMdl.h>
#include <ProMenu.h>
#include <ProModelitem.h>
#include <ProObjects.h>
#include <ProSizeConst.h>
#include <ProUtil.h>
#include <ProTKRunTime.h>
#include <ProMechanica.h>
#include <ProMechGeomref.h>

#include <ProFeature.h>
#include <ProFeatType.h>
#include <PTApplsUnicodeUtils.h>
#include <ProAsmcomp.h>
#include <ProSolid.h>
#include <ProMdlUnits.h>
#include <ProParameter.h>
#include <ProCsys.h>
#include <ProAxis.h>
#include <ProWstring.h>
#include <ProMaterial.h>
#include <ProFemMesh.h>
#include <ProWindows.h>
#include <ProDtmPln.h>
#include <ProDtmPnt.h>
#include <ProDtmAxis.h>
#include <ProDtmCsys.h>
#include <UtilString.h>
#include <ProIntfimport.h>
#include <ProSimprep.h>
#include <ProFit.h>
#include <ProIntf3Dexport.h>
#include <ProSurface.h>
#include <ProMdlUnits.h>
#include <ProRelSet.h>
//#include <TestError.h>
}

#endif