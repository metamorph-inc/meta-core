

#ifndef UTILNAMES_H
#define UTILNAMES_H

/*--------------------------------------------------------------------*\
Pro/TOOLKIT includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProFeature.h>
// #include <TestError.h>

/*====================================================================*\
   FUNCTION :	ProUtilModelnameGet
   PURPOSE  :	Get the name of a model as C string name and type
   INPUT    :   model - the model
   OUTPUT   :	name, type
\*====================================================================*/
char *ProUtilModelnameGet(
    ProMdl *model,
    char *name,
    char *type);

#endif /* UTILNAMES_H */
