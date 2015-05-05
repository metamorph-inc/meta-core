
 
/*--------------------------------------------------------------------*\
Pro/Toolkit includes -- include this first
\*--------------------------------------------------------------------*/
#include "ProToolkit.h"
 
/*--------------------------------------------------------------------*\
Application includes
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilString.h"
#include <ProTKRunTime.h>


/*====================================================================*\
FUNCTION : ProUtilErrPrint
PURPOSE  : converts standard toolkit error to a string
\*====================================================================*/
ProError ProUtilErrPrint(
    ProError error, 
    char *err_str)
{
      switch (error)
        {
          case PRO_TK_NO_ERROR:
                  ProUtilstrcpy(err_str, "NO_ERROR");
                  break;
 
          case PRO_TK_GENERAL_ERROR:
                  ProUtilstrcpy(err_str, "GENERAL_ERROR");
                  break;
 
          case PRO_TK_BAD_INPUTS:
                  ProUtilstrcpy(err_str, "BAD_INPUTS");
                  break;
 
          case PRO_TK_USER_ABORT:
                  ProUtilstrcpy(err_str, "USER_ABORT");
                  break;
 
          case PRO_TK_E_NOT_FOUND:
                  ProUtilstrcpy(err_str, "NOT_FOUND");
                  break;
 
          case PRO_TK_E_FOUND:
                  ProUtilstrcpy(err_str, "FOUND");
                  break;
 
          case PRO_TK_BAD_CONTEXT:
                  ProUtilstrcpy(err_str, "BAD_CONTEXT");
                  break;
 
          case PRO_TK_NOT_IMPLEMENTED:
                  ProUtilstrcpy(err_str, "NOT_IMPLEMENTED");
                  break;
 
          case PRO_TK_OUT_OF_MEMORY:
                  ProUtilstrcpy(err_str, "OUT_OF_MEMORY");
                  break;
 
          case PRO_TK_COMM_ERROR:
                  ProUtilstrcpy(err_str, "COMM_ERROR");
                  break;
 
          case PRO_TK_INVALID_TYPE:
                  ProUtilstrcpy(err_str, "INVALID_TYPE");
                  break;
 
          case PRO_TK_INVALID_PTR:
                  ProUtilstrcpy(err_str, "INVALID_PTR");
                  break;
 
          case PRO_TK_UNAV_SEC:
                  ProUtilstrcpy(err_str, "UNAV_SEC");
                  break;
 
          case PRO_TK_INVALID_NAME:
                  ProUtilstrcpy(err_str, "INVALID_NAME");
                  break;
 
          case PRO_TK_NOT_EXIST:
                  ProUtilstrcpy(err_str, "NOT_EXIST");
                  break;
 
          case PRO_TK_NOT_VALID:
                  ProUtilstrcpy(err_str, "NOT_VALID");
                  break;
 
          case PRO_TK_INVALID_ITEM:
                  ProUtilstrcpy(err_str, "INVALID_ITEM");
                  break;
 
          case PRO_TK_CANT_ACCESS:
                  ProUtilstrcpy(err_str, "CANT_ACCESS");
                  break;
 
          default:
                  ProTKSprintf(err_str,"%d", error);
        }
    return (PRO_TK_NO_ERROR);
}
