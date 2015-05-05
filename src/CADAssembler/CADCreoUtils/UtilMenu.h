
#ifndef UTILMENU_H
#define UTILMENU_H

#ifndef lint
static char UtilMenu_h[] = "@(#)UtilMenu.h	1.2 11/26/96";
#endif

/*--------------------------------------------------------------------*\
ProToolkit includes
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <stdlib.h>

/*--------------------------------------------------------------------*\
Application defines
\*--------------------------------------------------------------------*/
#define TEST_CALL_PRO_MENU_DELETE	1<<0
#define TEST_CALL_PRO_MENU_HOLD		1<<1
#define TEST_CALL_PRO_MENU_ACTIVATE	1<<2
#define TEST_CALL_PRO_MENU_DEACTIVATE	1<<3
#define TEST_CALL_PRO_MENU_HIGHLIGHT	1<<4
#define TEST_CALL_PRO_MENU_UNHIGHLIGHT	1<<5
#define TEST_CALL_PRO_MENUBUTTON_DELETE 1<<6

/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*\
Application data structures
\*--------------------------------------------------------------------*/
typedef struct util_menu_buttons 
{
/* Used for easy menu creation. First menu button for menu name,
   Last should be empty  */
    ProMenubuttonName button;
    int		      value;
    int		      special_flag;
} ProUtilMenuButtons;

/*--------------------------------------------------------------------*\
Prototypes
\*--------------------------------------------------------------------*/

int ProUtilAssign(ProAppData, int);
int ProUtilMenuKill(ProAppData, int);

/*=============================================================*\
  FUNCTION: ProUtilMenubuttonHighlight
  PURPOSE:  Highlight / Unhighlight button
\*=============================================================*/
ProError ProUtilMenubuttonHighlight(
    char    *menu,	    /* In : the menu name */
    char    *menubutton,    /* In : the menubutton name */
    int	    enable);	    /* In : 0 - unhigh, 1 - high */

/*=============================================================*\
  FUNCTION: ProUtilMenubuttonActivate
  PURPOSE:  Activate/Deactivate button
\*=============================================================*/
ProError ProUtilMenubuttonActivate(
    char    *menu,	    /* In : the menu name */
    char    *menubutton,    /* In : the menubutton name */
    int	    enable);	    /* In : 0 - deact, 1 - act */


/*=============================================================*\
  FUNCTION: ProUtilMenubuttonDeleteWithStatus
  PURPOSE:  Call DeleteWithStatus func
\*=============================================================*/
int ProUtilMenubuttonDeleteWithStatus(
    ProAppData appdata,
    int status);

/*============================================================================*\
 Function:	ProUtilMenuIntValueSelect
 Purpose:       Create menu/Select int value/Close menu
 Return:	PRO_TK_NO_ERROR if value selected, or PRO_TK_GENERAL_ERROR 
\*============================================================================*/
ProError ProUtilMenuIntValueSelect(
    ProUtilMenuButtons *buttons_array, /* In : The menu button array */
    int *output_value);

/*============================================================================*\
 Function:	ProUtilMenuStringsAlloc
 Purpose:       Alloc memory for array of wchar_t* used in ProMenu functions
\*============================================================================*/
ProError ProUtilMenuStringsAlloc(
    wchar_t ***w_ptr);

/*============================================================================*\
 Function:	ProUtilMenuStringsStrAdd
 Purpose:       Alloc memory and add string to MenuStrings array
\*============================================================================*/
ProError ProUtilMenuStringsStrAdd(
    wchar_t ***w_ptr,
    char *str);

/*============================================================================*\
 Function:	ProUtilMenuStringsWstrAdd
 Purpose:       Alloc memory and add string to MenuStrings array
\*============================================================================*/
ProError ProUtilMenuStringsWstrAdd(
    wchar_t ***w_ptr,
    wchar_t *w_str);


/*============================================================================*\
 Function:	ProUtilMenuStringsIntValueSelect
 Purpose:       Select a string from menu and return it's number
\*============================================================================*/
ProError ProUtilMenuStringsIntValueSelect(
    char    *menu_name,
    wchar_t **w_ptr,
    int	    *output_value);

/*============================================================================*\
 Function:	ProUtilMenuStringsFree
 Purpose:       Free memory allocated by ..Alloc, ..StrAdd, ..WstrAdd
\*============================================================================*/
ProError ProUtilMenuStringsFree(
    wchar_t ***w_ptr);

/*============================================================================*\
 Function:	ProUtilMenuStringManySelect
 Purpose:       Select many string from menu with submenu Select All/Unsell All
\*============================================================================*/
ProError ProUtilMenuStringManySelect(
    char    *menu_name,		   /* In : Menu name */
    wchar_t **w_menu_buttons,	   /* In : Menu buttons */
    int	    **output_array,	   /* Out: ProArray of numbers of selected 
					   strings, use ProArrayFree to free
					   memory */
    int *p_n_sel);		   /* Out: Number of selected components */

/*====================================================================*\
 FUNCTION :  ProUtilResolveFeatMenu()
 PURPOSE  :  Add button to resolve feat menu
\*====================================================================*/
ProError ProUtilResolveFeatMenu ();

#endif
