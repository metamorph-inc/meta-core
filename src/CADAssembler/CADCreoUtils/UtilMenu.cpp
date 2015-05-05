
#ifndef lint 
    static char UtilMenu_c [] = "@(#)UtilMenu.c	1.2 11/26/96";
#endif 

/*--------------------------------------------------------------------*\
Pro/Toolkit includes -- include this first
\*--------------------------------------------------------------------*/
#include <ProToolkit.h>
#include <ProArray.h>
#include <ProMenu.h>
#include <ProUtil.h>
/*--------------------------------------------------------------------*\
Application macros
\*--------------------------------------------------------------------*/
#include "TestError.h"
#include "UtilMenu.h"
#include "UtilString.h"

/*====================================================================*\
FUNCTION : ProUtilAssign
PURPOSE  : assigns option to a member (being dereferenced & casted to int)
	   Can be used in promenu_on_button. The menu will diappear after 
	   the chosen menu button is pushed.
\*====================================================================*/
int ProUtilAssign(ProAppData p_member, int option)
{
    ProError status;

    *((int *)p_member) = option;
    status = ProMenuDelete();
    TEST_CALL_REPORT("ProMenuDelete","ProUtilAssign", status,
    				status != PRO_TK_NO_ERROR);

    return (0);
}

/*====================================================================*\
FUNCTION : ProUtilMenuKill
PURPOSE  : Deletes the number of menus given.
\*====================================================================*/
int ProUtilMenuKill(ProAppData p_member, int n_menus)
{
    ProError status;
    int      i;

    for (i = 0; i < n_menus; i++)
    {
    status = ProMenuDelete();
    TEST_CALL_REPORT("ProMenuDelete","ProUtilMenuKill", status,
    				status != PRO_TK_NO_ERROR);
    }

    return (0);
}


/*=============================================================*\
  FUNCTION: ProUtilMenubuttonActivate
  PURPOSE:  Activate/Deactivate button
\*=============================================================*/
ProError ProUtilMenubuttonActivate(
    char    *menu,	    /* In : the menu name */
    char    *menubutton,    /* In : the menubutton name */
    int	    enable)	    /* In : 0 - dis, 1 - en */
{
    ProError err;

    if (enable)
    {
	err = ProMenubuttonActivate(menu, menubutton);
	TEST_CALL_REPORT("ProMenubuttonActivate()",
		 "ProUtilMenubuttonActivate()", err, err != PRO_TK_NO_ERROR);
    }
    else
    {
	err = ProMenubuttonDeactivate(menu, menubutton);
	TEST_CALL_REPORT("ProMenubuttonDeactivate()",
		 "ProUtilMenubuttonActivate()", err, err != PRO_TK_NO_ERROR);
    }
    return (err);
} 

/*=============================================================*\
  FUNCTION: ProUtilMenubuttonHighlight
  PURPOSE:  Highlight / Unhighlight button
\*=============================================================*/
ProError ProUtilMenubuttonHighlight(
    char    *menu,	    /* In : the menu name */
    char    *menubutton,    /* In : the menubutton name */
    int	    enable)	    /* In : 0 - dis, 1 - en */
{
    ProError err;

    if (enable)
    {
	err = ProMenubuttonHighlight(menu, menubutton);
	TEST_CALL_REPORT("ProMenubuttonHighlight()",
		 "ProUtilMenubuttonActivate()", err, err != PRO_TK_NO_ERROR);
    }
    else
    {
	err = ProMenubuttonUnhighlight(menu, menubutton);
	TEST_CALL_REPORT("ProMenubuttonUnhighlight()",
		 "ProUtilMenubuttonActivate()", err, err != PRO_TK_NO_ERROR);
    }
    return (err);
} 

/*=============================================================*\
  FUNCTION: ProUtilMenubuttonDeleteWithStatus
  PURPOSE:  Call DeleteWithStatus func
\*=============================================================*/
int ProUtilMenubuttonDeleteWithStatus(
    ProAppData appdata,
    int status)
{
    ProError err;

    err = ProMenuDeleteWithStatus(status);
    TEST_CALL_REPORT("ProMenuDeleteWithStatus()",
	"ProUtilMenubuttonDeleteWithStatus()", err, err != PRO_TK_NO_ERROR);
    return (0);
}

/*============================================================================*\
 Function:	ProUtilMenuIntValueSelect
 Purpose:       Create menu/Select int value/Close menu
 Return:	PRO_TK_NO_ERROR if value selected, or PRO_TK_GENERAL_ERROR 
\*============================================================================*/
ProError ProUtilMenuIntValueSelect(
    ProUtilMenuButtons *buttons_array, /* In : The menu button array */
    int *output_value)
{
    ProError err;
    int i, n_buttons, menu_id;
    wchar_t **w_menu_buttons;
    
    for (n_buttons = 0; buttons_array[n_buttons].button[0]!='\0'; n_buttons++);

    if (n_buttons <= 1)
	return PRO_TK_NO_ERROR;
	
    ProUtilMenuStringsAlloc(&w_menu_buttons);
    for (i=1; i<n_buttons; i++)
	ProUtilMenuStringsStrAdd(&w_menu_buttons, buttons_array[i].button);
    ProMenuFromStringsRegister ( buttons_array[0].button, NULL, w_menu_buttons,
	NULL, NULL, &menu_id);
    ProUtilMenuStringsFree(&w_menu_buttons);
    for (i=0; i<n_buttons; i++)
    {
	if (buttons_array[i].special_flag & 
	    (TEST_CALL_PRO_MENU_DELETE | TEST_CALL_PRO_MENU_HOLD))
	    ProMenubuttonActionSet(buttons_array[0].button, 
		buttons_array[i].button, (ProMenubuttonAction)
		(buttons_array[i].special_flag & TEST_CALL_PRO_MENU_DELETE ?
		ProMenuDelete : ProMenuHold), NULL, 0);
	else
	    ProMenubuttonActionSet(buttons_array[0].button, 
		buttons_array[i].button, (ProMenubuttonAction)
		ProUtilMenubuttonDeleteWithStatus, NULL, 
		buttons_array[i].value);
    }
    for (i=1; i<n_buttons; i++)
    {
	if (buttons_array[i].special_flag & TEST_CALL_PRO_MENUBUTTON_DELETE)
	    ProMenubuttonDelete(buttons_array[0].button, 
		buttons_array[i].button, &menu_id); 
    }
    err = ProMenuCreate( PROMENUTYPE_MAIN, buttons_array[0].button, &menu_id );
    for (i=1; i<n_buttons; i++)
    {
	if (buttons_array[i].special_flag &
	    (TEST_CALL_PRO_MENU_ACTIVATE | TEST_CALL_PRO_MENU_DEACTIVATE))
	    ProUtilMenubuttonActivate(buttons_array[0].button, 
		buttons_array[i].button, 
		buttons_array[i].special_flag & TEST_CALL_PRO_MENU_ACTIVATE);
	if (buttons_array[i].special_flag &
	    (TEST_CALL_PRO_MENU_HIGHLIGHT | TEST_CALL_PRO_MENU_UNHIGHLIGHT))
	    ProUtilMenubuttonActivate(buttons_array[0].button, 
		buttons_array[i].button, 
		buttons_array[i].special_flag & TEST_CALL_PRO_MENU_HIGHLIGHT);
    }
    err = ProMenuProcess(buttons_array[0].button, output_value);
    return (err);
}

/*============================================================================*\
 Function:	ProUtilMenuStringsAlloc
 Purpose:       Alloc memory for array of wchar_t* used in ProMenu functions
\*============================================================================*/
ProError ProUtilMenuStringsAlloc(
    wchar_t ***w_ptr)
{
    ProError err;
    static wchar_t  w_epmty[] = {'\0'};
    wchar_t *w_p = w_epmty;

    err = ProArrayAlloc(0, sizeof(wchar_t*), 1, (ProArray*)w_ptr);
    if (err == PRO_TK_NO_ERROR)
	err = ProArrayObjectAdd((ProArray*)w_ptr, 0, 1, &w_p);
    return (err);
}

/*============================================================================*\
 Function:	ProUtilMenuStringsStrAdd
 Purpose:       Alloc memory and add string to MenuStrings array
\*============================================================================*/
ProError ProUtilMenuStringsStrAdd(
    wchar_t ***w_ptr,
    char *str)
{
    ProError err;
    wchar_t *w_p;
    int n_str;

    int len = static_cast<int>(strlen(str));
    w_p = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
    if (w_p == NULL)
	return (PRO_TK_GENERAL_ERROR);
    ProStringToWstring(w_p, str); 

    err = ProArraySizeGet((ProArray)*w_ptr, &n_str);
    err = ProArrayObjectAdd((ProArray*)w_ptr, n_str-1, 1, &w_p);
    return (err);
}

/*============================================================================*\
 Function:	ProUtilMenuStringsWstrAdd
 Purpose:       Alloc memory and add string to MenuStrings array
\*============================================================================*/
ProError ProUtilMenuStringsWstrAdd(
    wchar_t ***w_ptr,
    wchar_t *w_str)
{
    ProError err;
    wchar_t *w_p;
    int len, n_str;

    len = ProUtilWstrLen(w_str);
    w_p = (wchar_t*)calloc(len + 1, sizeof(wchar_t));
    if (w_p == NULL)
	return (PRO_TK_GENERAL_ERROR);
    ProUtilWstrcpy(w_p, w_str); 

    err = ProArraySizeGet((ProArray)*w_ptr, &n_str);
    err = ProArrayObjectAdd((ProArray*)w_ptr, n_str-1, 1, &w_p);
    return (err);
}

/*============================================================================*\
 Function:	ProUtilMenuStringsIntValueSelect
 Purpose:       Select a string from menu and return it's number
\*============================================================================*/
ProError ProUtilMenuStringsIntValueSelect(
    char    *menu_name,
    wchar_t **w_ptr,
    int	    *output_value)
{
    ProUtilMenuButtons *buttons_array;
    int i, n_buttons;
    ProError err;

    for(n_buttons=1; w_ptr[n_buttons-1][0] != '\0'; n_buttons++);

    buttons_array = (ProUtilMenuButtons *)calloc(n_buttons+1, 
	sizeof(ProUtilMenuButtons));
    
    ProUtilstrcpy(buttons_array[0].button, (const char *) menu_name);
    buttons_array[0].special_flag = TEST_CALL_PRO_MENU_DELETE;

    for(i=1; i<=n_buttons; i++)
    {
	ProWstringToString(buttons_array[i].button, w_ptr[i-1]);
	buttons_array[i].value = i - 1;
    }
    err = ProUtilMenuIntValueSelect(buttons_array, output_value);
    free(buttons_array);
    return (err);
}

/*============================================================================*\
 Function:	ProUtilMenuStringsFree
 Purpose:       Free memory allocated by ..Alloc, ..StrAdd, ..WstrAdd
\*============================================================================*/
ProError ProUtilMenuStringsFree(
    wchar_t ***w_ptr)
{
    ProError err;
    int i, n_str;
    wchar_t **w_p = *w_ptr;

    err = ProArraySizeGet((ProArray)w_p, &n_str);
    for (i=n_str-2; i>0; i--)
	free(w_p[i]);
    err = ProArrayFree((ProArray*)w_ptr);
    return (err);
}

typedef struct menu_toggle_info
{
    char *menu_name;
    wchar_t **w_menu_buttons;
    int *button_status;
    int n_buttons;
} MenuToggleInfo;

#define SELECT_ALL  0
#define UNSEL_ALL   1
#define DONE_SEL    2
#define QUIT_SEL    3

/*============================================================================*\
 Function:	ProUtilMenuToggleOne
 Purpose:       Used from ProUtilMenuStringManySelect
\*============================================================================*/
int ProUtilMenuToggleOne(
    MenuToggleInfo *p_toggle_info,
    int n_str)
{
    p_toggle_info->button_status[n_str] = !p_toggle_info->button_status[n_str];
    return (0);
}

/*============================================================================*\
 Function:	ProUtilMenuToggleDone
 Purpose:       Used from ProUtilMenuStringManySelect
\*============================================================================*/
int ProUtilMenuToggleDone(
    MenuToggleInfo *p_toggle_info,
    int action)
{
    int i, sel;
    ProError err;
    ProCharName name;

    switch (action)
    {
    case SELECT_ALL:
    case UNSEL_ALL:
	sel = (action == SELECT_ALL);
	for (i=0; i<p_toggle_info->n_buttons; i++)
	    if (p_toggle_info->button_status[i] != sel)
	    {
		ProWstringToString(name, p_toggle_info->w_menu_buttons[i]);
		err = ProUtilMenubuttonHighlight(p_toggle_info->menu_name,
		    name,  sel);
		p_toggle_info->button_status[i] = sel;
	    }
	break;
    case DONE_SEL:
    case QUIT_SEL:
	err = ProMenuDeleteWithStatus(action);
	err = ProMenuDeleteWithStatus(action);
	break;
    }
    return (0);
}


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
    int *p_n_sel)		   /* Out: Number of selected components */
{
    int i, n_but, n_sel, menu_id, action;
    ProError err, ret_status = PRO_TK_USER_ABORT;
    int *toggle_array;
    MenuToggleInfo toggle_info;
    ProCharName name;
    wchar_t **w_menus;

    for(n_but=0; w_menu_buttons[n_but][0] != '\0'; n_but++);
    err = ProArrayAlloc(n_but, sizeof(int), 1, (ProArray*)&toggle_array);
    
    toggle_info.w_menu_buttons = w_menu_buttons;
    toggle_info.button_status = toggle_array;
    toggle_info.n_buttons = n_but;
    toggle_info.menu_name = menu_name;

    err = ProMenuFromStringsRegister ( menu_name, NULL, 
	w_menu_buttons, NULL, NULL, &menu_id);
    for (i=0; i<n_but; i++)
    {
	toggle_array[i] = 0;
	ProWstringToString(name, w_menu_buttons[i]);
	err = ProMenubuttonActionSet(menu_name, name,
	    (ProMenubuttonAction)ProUtilMenuToggleOne, &toggle_info, i);
    }   
    err = ProMenubuttonActionSet(menu_name, menu_name,
	(ProMenubuttonAction)ProMenuDelete, NULL, 0);
    err = ProMenuModeSet(menu_name, PROMENUMODE_DATA);
    err = ProMenuDatamodeSet(menu_name, PRO_B_TRUE );
    err = ProMenuCreate(PROMENUTYPE_MAIN, menu_name, &menu_id);

    err = ProUtilMenuStringsAlloc(&w_menus);
    err = ProUtilMenuStringsStrAdd(&w_menus, (char *)"Select All");
    err = ProUtilMenuStringsStrAdd(&w_menus, (char *)"Unsel All");
    err = ProUtilMenuStringsStrAdd(&w_menus, (char *)"Done Sel");
    err = ProUtilMenuStringsStrAdd(&w_menus, (char *)"Quit Sel");
    
    ProMenuFromStringsRegister ( (char *)"TkMenuUtilSelect", NULL, w_menus,
	NULL, NULL, &menu_id);
    err = ProMenubuttonActionSet((char *)"TkMenuUtilSelect", (char *)"Select All",
	(ProMenubuttonAction)ProUtilMenuToggleDone, &toggle_info, SELECT_ALL);
    err = ProMenubuttonActionSet((char *)"TkMenuUtilSelect", (char *)"Unsel All", 
	(ProMenubuttonAction)ProUtilMenuToggleDone, &toggle_info, UNSEL_ALL);
    err = ProMenubuttonActionSet((char *)"TkMenuUtilSelect", (char *)"Done Sel", 
	(ProMenubuttonAction)ProUtilMenuToggleDone, &toggle_info, DONE_SEL);
    err = ProMenubuttonActionSet((char *)"TkMenuUtilSelect", (char *)"Quit Sel",
	(ProMenubuttonAction)ProUtilMenuToggleDone, &toggle_info, QUIT_SEL);
    err = ProMenubuttonActionSet((char *)"TkMenuUtilSelect", (char *)"TkMenuUtilSelect", 
	(ProMenubuttonAction)ProUtilMenuToggleDone, &toggle_info, QUIT_SEL);

    err = ProMenuCreate(PROMENUTYPE_SUB, (char *)"TkMenuUtilSelect", &menu_id);
    err = ProMenuProcess((char *)"TkMenuUtilSelect", &action);
    if(err == PRO_TK_NO_ERROR && action == DONE_SEL)
    {
        err = ProArrayAlloc(0, sizeof(int), 1, (ProArray*)output_array);
	n_sel = 0;
	for (i=0; i<n_but; i++)
	    if (toggle_array[i])
	    {
		ProArrayObjectAdd((ProArray*)output_array, PRO_VALUE_UNUSED,
		    1, &i);
		n_sel++;
	    }
	if (n_sel>0)
	{
	    *p_n_sel = n_sel;
	    ret_status = PRO_TK_NO_ERROR;
	}
	else
	    err = ProArrayFree((ProArray*)output_array);
    }
    err = ProArrayFree((ProArray*)&toggle_array);
    return (ret_status);
}

#undef SELECT_ALL
#undef UNSEL_ALL
#undef DONE_SEL
#undef QUIT_SEL

