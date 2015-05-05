
#ifndef UTILTYPES_H
#define UTILTYPES_H

#include <ProMfg.h>
#include <ProScope.h>
#include <ProGtol.h>
#include <ProSurfacedata.h>

PRO_BEGIN_C_DECLS

extern int ProUtilObjtypeStr( ProType type, char *type_name);

extern int ProUtilFeattypeStr( ProFeattype type, char *type_name);

extern int ProUtilSrftypeStr( ProSrftype type, char *type_name);

extern int ProUtilEnttypeStr( ProEnttype type, char *type_name);

extern int ProUtilMfgtypeStr( ProMfgType type, char *type_name);

extern int ProUtilExtRefScopeStr(ProExtRefScope type, char *type_name);

extern int ProUtilGtolTypeStr( ProGtolType type, char *type_name);

extern int ProUtilGtolMaterialCondStr(ProGtolMaterialCond type, char *type_name);

extern int ProUtilLeaderTypeStr(ProLeaderType type, char *type_name);

extern int ProUtilGtolPlacementTypeStr(ProGtolPlacementType type, char *type_name);

extern int ProUtilGtolRefItemTypeStr(ProGtolRefItemType type, char *type_name);

extern int ProUtilGtolReferenceTypeStr(ProGtolReferenceType type, char *type_name);

extern int ProUtilGtolProjzoneStr(ProGtolProjzone type, char *type_name);

PRO_END_C_DECLS

#endif /* ifdef UTILTYPES_H */

