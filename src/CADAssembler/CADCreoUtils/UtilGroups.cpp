

/*--------------------------------------------------------------------*\
Pro/Toolkit includes -- include this first
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"
#include "TestError.h"
#include "ProMenu.h"
#include "ProElemId.h"

/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestElem.h"
#include "UtilGroups.h"
#include "UtilString.h"


/*====================================================================*\
FUNCTION : ProUtilIdToStr
PURPOSE  : Given the ProElemId, return the corresponding string
\*====================================================================*/
ProError ProUtilIdToStr(ProElemId elem_id, char *string_id)
{
    int table_size = sizeof(id_string_table2)/sizeof(ProTestIdToString);
    int i;
    ProBool exists = PRO_B_FALSE;

    ProUtilstrcpy(string_id, "not exist");
    for (i=0; i<table_size; i++)
     {
        if (id_string_table2[i].name_id == elem_id)
          {
            ProUtilstrcpy(string_id, 
                          (const char *)id_string_table2[i].elem_name);
            exists = PRO_B_TRUE;
            break;
          }
     }
    if (exists == PRO_B_TRUE)
       return PRO_TK_NO_ERROR;
    else
       return PRO_TK_E_NOT_FOUND;
}


/*====================================================================*\
FUNCTION : ProUtilStrToId
PURPOSE  : Given the string, return the corresponding element id
\*====================================================================*/
ProError ProUtilStrToId(char *string_id, ProElemId *p_elem_id)
{
    int table_size = sizeof(id_string_table2)/sizeof(ProTestIdToString);
    int i;
    ProBool exists = PRO_B_TRUE;

    for (i=0; i<table_size; i++)
     {
        if (strcmp(id_string_table2[i].elem_name, string_id) == 0)
          {
            *p_elem_id = id_string_table2[i].name_id;
            exists = PRO_B_TRUE;
            break;
          }
     }
    if (exists == PRO_B_TRUE)
       return PRO_TK_NO_ERROR;
    else
       return PRO_TK_E_NOT_FOUND;
}


