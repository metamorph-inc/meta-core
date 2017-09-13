#include "stdafx.h"

/*--------------------------------------------------------------------*\
Pro/Toolkit includes
\*--------------------------------------------------------------------*/
#include <ProUICmd.h>

/*--------------------------------------------------------------------*\
Meta includes
\*--------------------------------------------------------------------*/
//#include "CommonDefinitions.h"
#include "GlobalModelData.h"
#include "TestError.h"
#include "CreoPlugins.h"
#include "CreoPluginFunctions.h"

/*--------------------------------------------------------------------*\
Application global/external data
\*--------------------------------------------------------------------*/
static wchar_t *MSGFIL = L"msg_user.txt";
static char revcode[PRO_LINE_SIZE];

ProMdl getSelectedMdl();  // defined in CreoExport.cpp

static uiCmdAccessState TestAccessAlways(uiCmdAccessMode access_mode)
{
    return ACCESS_AVAILABLE;
}

static bool CheckMode(isis::MetaLinkMode mode)
{
    if(isis::GlobalModelData::Instance.mode != mode)
    {
        ostringstream msg;
        msg << "This functionality is only available in "
            << ((mode == isis::DESIGNEDIT) ? "design" : "component")
            << " editing mode";
        ErrorDialogStr(msg.str());
        return false;
    }
    return true;
}


/**
Select (highlight) an existing component instance in Creo (Creo Select)
causing the corresponding element to hightlight in CyPhy.

Pre-conditions:
 * MetaLink is running and CyPhy and CAD-assebler are connected
 * Creo is opened in assembly design editing mode.
   - CyPhy switches Creo to assembly design editing mode.
   - CyPhy starts CAD-assembler in assembly design editing mode.
 * There is an assembly design loaded in Creo.
Action:
 * Right mouse button select the component instance you want to "highlight".
   - choose "Highlight"
Post-condition:
 * The selected item is highlighted in the CyPhy component.
*/
ProError DoSelectComponent()
{
    isis::GlobalModelData::Instance.metalink_handler_ptr->
    send_LocateSelectedRequest(
        isis::GlobalModelData::Instance.instanceId,
        isis::GlobalModelData::Instance.designId);
    return PRO_TK_NO_ERROR;
}

/**
Select (highlight) an existing datum feature in Creo (Creo Select)
causing the corresponding element to hightlight in CyPhy.

Pre-conditions:
 * MetaLink is running and CyPhy and CAD-assebler are connected
 * Creo is opened in AVM component editing mode.
   - CyPhy switches Creo to AVM component editing mode.
   - CyPhy starts CAD-assembler in component editing mode.
 * There is an AVM component model (assembly or part) loaded in Creo.
Action:
 * Right mouse button select the assembly or part you want to "highlight".
   - choose "Highlight"
Post-condition:
 * The selected item is highlighted in the CyPhy component.
*/
ProError DoSelectDatum()
{
    return PRO_TK_NOT_IMPLEMENTED;
}

ProError DoSelect()
{
    
    isis_LOG(lg, isis_FILE, isis_WARN)
            << "selection in mode: " << isis::GlobalModelData::Instance.mode;

    switch(isis::GlobalModelData::Instance.mode)
    {
    case isis::UNDEFINEDMODE:
         isis_LOG(lg, isis_FILE, isis_WARN) << "select/locate is undefined in current mode";
        return PRO_TK_NOT_IMPLEMENTED;
    case isis::COMPONENTEDIT:
        return DoSelectDatum();
    case isis::DESIGNEDIT:
        return DoSelectComponent();
    default:
        isis_LOG(lg, isis_FILE, isis_WARN)
                << "select/locate is undefined in current mode "
                << isis::GlobalModelData::Instance.mode;
        return PRO_TK_NOT_IMPLEMENTED;
    }
}

/*====================================================================*\
FUNCTION : user_initialize()
PURPOSE  : Pro/DEVELOP standard initialize
   Define an action and menu button for selecting a component.
\*====================================================================*/
ProError SetupCreoSelectPlugin()
{
    
    ProError status;
    uiCmdCmdId	cmd_id_select;

    // Add new button to the menu bar
    status = ProCmdActionAdd("IsisSelectComponent",(uiCmdCmdActFn)DoSelect, uiProe2ndImmediate, TestAccessAlways,	PRO_B_TRUE, PRO_B_TRUE, &cmd_id_select);

    switch(status = ProCmdDesignate(cmd_id_select, "-ISIS Select Cyphy Component", "-ISIS Select Cyphy Component Label", "-ISIS Select Cyphy Component Description", MSGFIL))
    {
    case PRO_TK_NO_ERROR:
        break;
    case PRO_TK_BAD_INPUTS:
        isis_LOG(lg, isis_FILE, isis_WARN) << "One or more input arguments was invalid.";
        return PRO_TK_BAD_INPUTS;
    case PRO_TK_E_NOT_FOUND:
        isis_LOG(lg, isis_FILE, isis_WARN) << "The message file was not found.";
        return PRO_TK_E_NOT_FOUND;
    case PRO_TK_MSG_NOT_FOUND:
        isis_LOG(lg, isis_FILE, isis_WARN) << "One or more messages was not found in the message file.";
        return PRO_TK_MSG_NOT_FOUND;
    }
    switch(status = ProCmdIconSet(cmd_id_select, "isis_select.png"))
    {
    case  PRO_TK_NO_ERROR:
        break;
    case PRO_TK_BAD_INPUTS:
         isis_LOG(lg, isis_FILE, isis_WARN) << "The input arguments were invalid.";
        return PRO_TK_BAD_INPUTS;
    case PRO_TK_E_NOT_FOUND:
         isis_LOG(lg, isis_FILE, isis_WARN) << "The icon file was not found.";
        return PRO_TK_E_NOT_FOUND;
    case PRO_TK_INVALID_FILE:
         isis_LOG(lg, isis_FILE, isis_WARN) << "The file specified was not a Creo Parametric .BIF file or a custom .GIF file.";
        return PRO_TK_INVALID_FILE;
    default:
         isis_LOG(lg, isis_FILE, isis_WARN) << "The file specified was not of required format.";
        return status;
    }


    return PRO_TK_NO_ERROR;
}
