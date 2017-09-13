#include "stdafx.h"
#define	PRO_USE_VAR_ARGS 1
#include "AssembleUtils.h"
#include "CreoPluginFunctions.h"
#include <ProParamval.h>
#include <ProFeature.h>
#include <ProUIMessage.h>
#include <ProAssembly.h>
#include <ProFeatType.h>
#include <ProParameter.h>
#include <ProMdl.h>
#include <ProAsmcomp.h>
#include <ProUIDialog.h>
#include <ProUITextarea.h>
#include <ProUIPushbutton.h>
#include <ProUILabel.h>
#include <ProUIList.h>
#include <ProUtil.h>
#include <ProUITree.h>
#include <ProSolid.h>
#include <ProSelbuffer.h>
#include <ProModelitem.h>
#include <ProUIInputpanel.h>
#include <ProUIOptionmenu.h>
#include <cstdio>
#include <fstream>
#include "CreoModelToCyphyXML.h"
#include "DatumRefResolver.h"
#include "UIFunctions.h"
using namespace std;

#include "CreoMetaDataExporter.h"

// Boost-related includes
#include <boost/uuid/uuid.hpp>            // uuid class
#include <boost/uuid/uuid_generators.hpp> // generators
#include <boost/uuid/uuid_io.hpp>

// Meta includes
#include "gen/MetaLinkMsg.pb.h"
#include "BridgeClient.h"
#include "cc_CommonDefinitions.h"
#include "cc_MultiFormatString.h"
namespace meta = edu::vanderbilt::isis::meta;
namespace pb = google::protobuf;
namespace asio = boost::asio;
using asio::ip::tcp;
using boost::uint8_t;

// Windows API
#include <windows.h>

struct DatumInfo
{
    string datumName;
    string cyphyName;
    int alignment; // 0 - ALIGN, 1 - MATE
};

class ComponentItem
{
public:
    ComponentItem() : isFolder(false), parentIdx(-1)
    {
    }
    bool isFolder; // If this is a folder, true
    int parentIdx; // Index of parent (which is a folder). -1 otherwise
    string guid;
    string name;
    string parentGuid;
    // Returns parent index based on guid
    static const int findGuid(vector<ComponentItem>& items, string guid)
    {
        for(vector<ComponentItem>::const_iterator it = items.begin(); it != items.end(); ++it)
        {
            if(it->guid==guid)
                return it-items.begin();
        }
        return -1;
    }
};

// Messages to display on UI
static wchar_t *connectionfailed = L"Unable to connect to bridge. Please check if it is running.";
static char *origin = "CAD";
static char *bridgeIP = "127.0.0.1";
static char* bridgePort = "15150";
// Static list, holding the retrieved component list from the Bridge
static vector<ComponentItem> componentTree;
static char selectedGuid[128];

// Check which mode Creo is operating
bool CheckMode(isis::MetaLinkMode mode)
{
    if(isis::GlobalModelData::Instance.mode != mode)
    {
        ErrorDialogStr(string("This functionality is only available in ") + string(((mode==isis::DESIGNEDIT)?"design":"component")) + string(" editing mode"));
        return false;
    }
    return true;
}

// Get the currently selected model (the first one)
ProMdl getSelectedMdl()
{
    ProMdl mdl = 0;
    ProSelection *selection = 0;
    ProModelitem p_mdl_item;
    int selsize;
    ProSelbufferSelectionsGet(&selection);
    ProArraySizeGet(selection,&selsize);
    if(selsize > 0)
    {
        ProSelectionModelitemGet(*selection, &p_mdl_item);
        ProModelitemMdlGet(&p_mdl_item,&mdl);
        ProSelectionarrayFree(selection);
        return mdl;
    }
    else
    {
        return NULL;
    }
}

// Get the list of selected models
int getSelectedMdls(vector<ProMdl> &models)
{
    ProMdl mdl = 0;
    ProSelection *selection = 0;
    ProModelitem p_mdl_item;
    int selsize;
    ProSelbufferSelectionsGet(&selection);
    ProArraySizeGet(selection,&selsize);
    for(int i = 0; i < selsize; i++)
    {
        ProSelectionModelitemGet(selection[i], &p_mdl_item);
        ProModelitemMdlGet(&p_mdl_item,&mdl);
        models.push_back(mdl);
    }
    ProSelectionarrayFree(selection);
    return selsize;
}

// Converts a wide string to string
string getStrFromWstr(const wchar_t* ws)
{
    wstring wstr(ws);
    return string(wstr.begin(),wstr.end());
}

static void StdCloseAction(char* dialog, char* component, ProAppData data)
{
    ProUIDialogExit(dialog, 1);
}

string GetCyphyName(ProFeature* p_feature)
{
    ProError error;
    // Get CYPHY_NAME as datumname, if set
    ProParameter datumNameParam;
    ProParamvalue datumNameValue;
    error = ProParameterInit(p_feature, const_cast<wchar_t*>(wstring(isis::CYPHY_NAME.begin(),isis::CYPHY_NAME.end()).c_str()), &datumNameParam);
    if(error == PRO_TK_NO_ERROR)
    {
        error = ProParameterValueGet(&datumNameParam, &datumNameValue);
        if(error == PRO_TK_NO_ERROR)
        {
            if(datumNameValue.type == PRO_PARAM_STRING)
            {
                wstring wstr(datumNameValue.value.s_val);
                return string(wstr.begin(),wstr.end());
            }
        }
    }
    return "";
}

void ErrorDialogStr(const string msg)
{
    wstring wmsg(msg.begin(),msg.end());
    ErrorDialog(wmsg.c_str());
}

void ErrorDialog(const wchar_t* msg)
{
    ProUIMessageButton* buttons;
    ProUIMessageButton user_choice;
    ProArrayAlloc(1, sizeof(ProUIMessageButton), 1, (ProArray*)&buttons);
    buttons [0] = PRO_UI_MESSAGE_OK;
    ProUIMessageDialogDisplay(PROUIMESSAGE_ERROR, L"Error", (wchar_t*)msg, buttons, PRO_UI_MESSAGE_OK, &user_choice);
}

string getExtension(ProMdlType p_type)
{
    switch(p_type)
    {
    case PRO_MDL_ASSEMBLY:
        return ".ASM";
    case PRO_MDL_PART:
        return ".PRT";
    default:
        return "";
    }
}
// Populates the tree control with the list of components (add component)
void populateProUITree(char *dialog, char *component, const vector<ComponentItem> &items)
{
    ProUITreeNodeType data;
    ProUITreeNodeTypeAlloc("testnode", &data);
    char* root = "RootNode";
    ProUITreeNodeAdd(dialog, component, root, L"root", NULL, "testnode");
    for(vector<ComponentItem>::const_iterator it = items.begin(); it != items.end(); ++it)
    {
        char *parent;
        if(it->parentIdx==-1)
        {
            parent = root;
        }
        else
        {
            parent = const_cast<char*>(it->parentGuid.c_str());
        }
        size_t origsize = it->name.size() + 1;
        const size_t newsize = 100;
        size_t convertedChars = 0;
        wchar_t wcstring[newsize];
        mbstowcs_s(&convertedChars, wcstring, origsize, it->name.c_str(), _TRUNCATE);
        ProUITreeNodeAdd(dialog, component, const_cast<char*>(it->guid.c_str()), wcstring, parent, "PROUI_MIXED");
    }
    ProUITreeNodeExpand(dialog, component, root, PRO_B_TRUE);
}

// Requests the component list from Cyphy, and populates the tree control with the elements (add component)
int RequestComponentList()
{
    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    boost::uuids::uuid guid = boost::uuids::random_generator()();
    edit->set_guid(boost::uuids::to_string(guid));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);

    edit->add_origin(origin);

	meta::Action *action = edit->add_actions();
	action->set_actionmode(meta::Action_ActionMode_REQUEST_COMPONENT_LIST);


	isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    return true;
}

static void RefreshComponentListAction(char* dialog, char* component, ProAppData data)
{
    RequestComponentList();
}


// Processes the response received with the list of components (add component)
bool processResponse_ComponentList(isis::EditPointer edit_ptr)
{
    pb::RepeatedPtrField<meta::Action> prl = edit_ptr->actions();

    for(int kx=0; kx < prl.size(); ++kx)
    {
        meta::Action action = prl.Get(kx);

        if(action.actionmode() == meta::Action::REQUEST_COMPONENT_LIST)
        {
            componentTree.clear();
            for(int i = 0; i < action.manifest_size(); i++)
            {
                ComponentItem it;
                it.name = action.manifest(i).name();
                it.guid = action.manifest(i).guid();
                if(action.manifest(i).nodemode() == meta::ComponentManifestNode_NodeMode_FOLDER)
                {
                    it.isFolder = true;
                }
                else
                {
                    it.parentIdx = ComponentItem::findGuid(componentTree, action.manifest(i).cyphyparentid());
                    it.parentGuid = action.manifest(i).cyphyparentid();
                }
                componentTree.push_back(it);
            }
        }
    }
	    int dialog_status;
    ProError status;
    int reqresult;
    char *dialog = "pt_install_dialog";

    status = ProUIDialogCreate(dialog, dialog);

    status = ProUIDialogCloseActionSet(dialog, StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "CancelBtn",	StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "CreateBtn",	RefreshComponentListAction, NULL);
    status = ProUITreeSelectActionSet(dialog, "CompTree", CompTreeSelectAction, NULL);

    populateProUITree("pt_install_dialog", "CompTree", componentTree);

    status = ProUIDialogActivate(dialog, &dialog_status);
    if(status != PRO_TK_NO_ERROR) ErrorDialog(L"Dialog create error");

    status = ProUIDialogDestroy(dialog);



    return false;
}

// Sends an add component message to Cyphy
int SendAddComponent(char *dialog, char* guid, const string &name, const string& designId)
{
    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(guid);

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);

    meta::Action* action = edit->add_actions();
    action->set_actionmode(meta::Action_ActionMode_ADD_COMPONENT_TO_CYPHY_DESIGN);
	action->set_subjectid(designId);
    meta::CADComponentType* comp = action->mutable_payload()->add_components();
    comp->set_avmcomponentid(guid);
    comp->set_name(name);

    edit->add_origin(origin);
    edit->set_sequence(1);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    return 0;
}


static void CompTreeSelectAction(char* dialog, char* component, ProAppData data)
{
    ProError status = PRO_TK_NO_ERROR;
    int count;
    char **values;
    status = ProUITreeSelectednamesGet(dialog, component, &count, &values);
    if(count < 1) return;
    strcpy(selectedGuid,values[0]);
    int idx = ComponentItem::findGuid(componentTree, selectedGuid);
    if(componentTree[idx].isFolder) return;
    ProStringarrayFree(values,count);

    CompAddDialogCreate();
    return;
}

static void CreateDlgCancelAction(char* dialog, char* component, ProAppData data)
{
    ProUIDialogExit(dialog, 1);
}

static void CreateDlgOkAction(char* dialog, char* component, ProAppData data)
{
    ProError status;
    wchar_t *cname;
    size_t convertedChars = 0;
    ProMdl mdl;
    ProModelitem p_model_item;
    wchar_t* cid;
    ProUIInputpanelValueGet(dialog, "Message", &cname);

    string name = getStrFromWstr(cname);

    int n = name.find("\n");
    while(n != string::npos)
    {
        name.erase(n);
        n = name.find("\n");
    }
    n = name.find("\r");
    while(n != string::npos)
    {
        name.erase(n);
        n = name.find("\r");
    }

    if(name.size()==0)
    {
        wchar_t empty[1];
        *empty=0;
        ProUIInputpanelValueSet(dialog, component, empty);
        ErrorDialog(L"Invalid component name");
        return;
    }

    ProMdlCurrentGet(&mdl);
    ProMdlToModelitem(mdl, &p_model_item);

    string designId = isis::GlobalModelData::Instance.designId;
    if(designId.find('|')!=string::npos)
    {
        designId = designId.substr(0, designId.find('|'));
    }

    if(designId.size()>0)
    {
        SendAddComponent(dialog, selectedGuid, getStrFromWstr(cname), designId);
    }
    else
    {
        ErrorDialog(L"No CYPHY_DESIGN_ID or CYPHY_COMPONENT_INSTANCE_ID found. You might be in component editing mode and can't add components in this mode.");
        return;
    }

    ProWstringFree(cname);

    status = ProUIDialogExit(dialog, 0);
}

// Shows the "Add Component" dialog
ProError CompAddDialogCreate()
{
    int 	dialog_status;
    char *	dialog = "comp_add_dialog";
    ProError 	status;

    status = ProUIDialogCreate(dialog, dialog);
    status = ProUIDialogCloseActionSet(dialog, StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "CancelBtn",	CreateDlgCancelAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "OkBtn", CreateDlgOkAction, NULL);
    status = ProUIInputpanelActivateActionSet(dialog, "Message", CreateDlgOkAction, NULL);

    // Set name to component name as default
    int idx = ComponentItem::findGuid(componentTree, selectedGuid);
    isis::MultiFormatString compname(componentTree[idx].name);
    ProUIInputpanelValueSet(dialog, "Message", const_cast<wchar_t*>((const wchar_t*)compname));

    status = ProUIDialogActivate(dialog, &dialog_status);

    status = ProUIDialogDestroy(dialog);

    if(dialog_status==0)
    {
        status = ProUIDialogExit("pt_install_dialog", PRO_TK_NO_ERROR);
    }

    return status;

}


// Retrieves component information and sends an update message to Cyphy
int UpdateComponent(ProMdl mdl)
{
    ProError err;
    if(!mdl) return 0;
    isis::Component comp;
    isis::FillComponentStructure(mdl, comp);
    if(comp.numResources()>0)
    {
        if(isis::GlobalModelData::Instance.mode == isis::COMPONENTEDIT)
        {
            comp.getResource(0).setId(isis::GlobalModelData::Instance.ComponentEdit.resourceId);
        }
        else
        {
            comp.getResource(0).setId("123344");
        }
        comp.getModel().setMainResourceID(comp.getResource(0).getId());
    }

    ProModelitem p_model_item;
    ProMdlToModelitem(mdl, &p_model_item);

    ostringstream str;
    //ofstream str("test.xml");

    comp.write(str);

    isis::EditPointer edit(new meta::Edit());
    edit->add_origin(origin);
    edit->set_editmode(meta::Edit_EditMode_POST);

    boost::uuids::uuid guid = boost::uuids::random_generator()();
    edit->set_guid(boost::uuids::to_string(guid));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);
    string avmid;

    if(isis::GlobalModelData::Instance.mode == isis::COMPONENTEDIT)
    {
        avmid = isis::GlobalModelData::Instance.ComponentEdit.avmId;
    }
    else
    {
        int selsize;
        ProSelection *selection = 0;
        ProSelbufferSelectionsGet(&selection);
        ProArraySizeGet(selection,&selsize);
        if(selsize==0)
        {
            ProSelectionarrayFree(selection);
            ErrorDialog(L"Please select component(s).");
            return 0;
        }
        ProMdl mainMdl;
        ProMdlCurrentGet(&mainMdl);
        ProAsmcomppath p_cmp_path;
        ProIdTable comp_id_table;
        comp_id_table[0] = -1;
        ProAsmcomppathInit((ProAssembly)mainMdl, comp_id_table, 0, &p_cmp_path);
        ProSelectionAsmcomppathGet(selection[0], &p_cmp_path);
        const isis::CADComponentData *cdata = isis::GlobalModelData::Instance.GetComponentDataFromModel(p_cmp_path);
        avmid = cdata->avmComponentId;
    }
	meta::Action *action = edit->add_actions();
    meta::Alien *alien = new meta::Alien();
    alien->set_encoded(str.str());
    alien->set_encodingmode(meta::Alien_EncodingMode_XML);
	action->set_actionmode(meta::Action_ActionMode_UPDATE_CYPHY_COMPONENT);
    action->set_allocated_alien(alien);
	action->set_subjectid(avmid);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    err = ProMdlSave(mdl);

    if(err!=PRO_TK_NO_ERROR)
    {
        ErrorDialog(L"Unable to save model");
        return 0;
    }

    return 1;
}

// Clears everything in Creo and re-assembles the design
int Resync(ProMdl mdl)
{
    isis::EditPointer edit(new meta::Edit());
    edit->add_origin(origin);
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	meta::Action *action = edit->add_actions();
	action->set_actionmode(meta::Action_ActionMode_RESYNC);

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);
    if(isis::GlobalModelData::Instance.mode == isis::COMPONENTEDIT)
    {
//		edit->add_topic(isis::GlobalModelData::Instance.ComponentEdit.avmId);
		action->set_subjectid(isis::GlobalModelData::Instance.ComponentEdit.avmId);
    }
    else if(isis::GlobalModelData::Instance.mode == isis::DESIGNEDIT)
    {
//		edit->add_topic(isis::GlobalModelData::Instance.designId);
        action->set_subjectid(isis::GlobalModelData::Instance.designId);
    }

    isis::GlobalModelData::Instance.metalinkAssemblyEditorPtr->Clear();

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);
    return 1;
}


static void CreateNewComponentAction(char* dialog, char* component, ProAppData data)
{
    wchar_t *cname;

    isis::Component *comp = (isis::Component*)data;
    ProError err=PRO_TK_NO_ERROR;

    ProUIInputpanelValueGet(dialog, "Message", &cname);

    if(!wcslen(cname))
    {
        ErrorDialog(L"Invalid name");
        return;
    }

    comp->setName(getStrFromWstr(cname));

    string avmid = boost::uuids::to_string(boost::uuids::random_generator()());

    comp->setAVMId(avmid);

    std::ostringstream str;
    comp->write(str);

    isis::EditPointer edit(new meta::Edit());
    edit->add_origin(origin);
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);
    if(isis::GlobalModelData::Instance.mode == isis::UNDEFINEDMODE)
    {
//        edit->add_topic(isis::GlobalModelData::Instance.designId);
    }
    meta::Action *action = edit->add_actions();
    meta::Alien *alien = new meta::Alien();
    alien->set_encoded(str.str());
    alien->set_encodingmode(meta::Alien_EncodingMode_XML);
	action->set_actionmode(meta::Action_ActionMode_CREATE_CYPHY_COMPONENT);
    action->set_allocated_alien(alien);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

	isis::GlobalModelData::Instance.mode = isis::COMPONENTEDIT;

    ProWstringFree(cname);
    ProUIDialogExit(dialog, PRO_TK_NO_ERROR);
}

// Create a new component from the selected part/assembly
int CreateNewComponent(ProMdl mdl)
{
    ProError err;
    if(!mdl)
    {
        ErrorDialogStr("Please select an Assembly or Part in the tree. Your Cyphy component is going to be created from the selected entity.");
        return 0;
    }
    isis::Component comp;
    isis::FillComponentStructure(mdl, comp);
    if(comp.numResources()>0)
    {
        comp.getModel().setMainResourceID(comp.getResource(0).getId());
    }

    err = ProMdlSave(mdl);

    if(err!=PRO_TK_NO_ERROR)
    {
        ErrorDialog(L"Unable to save model");
    }

    char *dialog = "comp_create_dialog";
    err = ProUIDialogCreate(dialog, dialog);

    err = ProUIDialogCloseActionSet(dialog, StdCloseAction, NULL);
    err = ProUIPushbuttonActivateActionSet(dialog, "CancelBtn", StdCloseAction, NULL);
    err = ProUIPushbuttonActivateActionSet(dialog, "OkBtn", CreateNewComponentAction, (ProAppData)&comp);
    err = ProUIInputpanelActivateActionSet(dialog, "Message", CreateNewComponentAction, (ProAppData)&comp);

    int dlg_status;
    err = ProUIDialogActivate(dialog, &dlg_status);

    if(dlg_status=PRO_TK_NO_ERROR)
    {
        err = ProMdlSave(mdl);
    }

    err = ProUIDialogDestroy(dialog);

    return 1;
}

// Executes the remove component functionality
ProError DoRemoveComponent()
{
    if(!CheckMode(isis::DESIGNEDIT)) return PRO_TK_NO_ERROR;
    int selsize;
    ProSelection *selection = 0;
    ProSelbufferSelectionsGet(&selection);
    ProArraySizeGet(selection,&selsize);
    if(selsize==0)
    {
        ProSelectionarrayFree(selection);
        ErrorDialog(L"Please select component(s).");
    }

    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);

    meta::Action* action = edit->add_actions();
    action->set_actionmode(meta::Action_ActionMode_REMOVE_CYPHY_DESIGN_COMPONENT);
	action->set_subjectid(isis::GlobalModelData::Instance.designId);
    ProMdl mainMdl;
    ProMdlCurrentGet(&mainMdl);
    for(int i = 0; i < selsize; i++)
    {
        ProModelitem p_mdl_item;
        ProMdl mdl = 0;
        // Left component
        ProSelectionModelitemGet(selection[i], &p_mdl_item);
        if(p_mdl_item.type != PRO_ASSEMBLY && p_mdl_item.type != PRO_PART)
        {
            ErrorDialogStr("Selected items must be part/assembly.");
            ProSelectionarrayFree(selection);
            return PRO_TK_NO_ERROR;
        }
        ProModelitemMdlGet(&p_mdl_item,&mdl);
        ProAsmcomppath p_cmp_path;
        ProIdTable comp_id_table;
        comp_id_table[0] = -1;
        ProAsmcomppathInit((ProAssembly)mainMdl, comp_id_table, 0, &p_cmp_path);
        ProSelectionAsmcomppathGet(selection[i], &p_cmp_path);
        const isis::CADComponentData *cdata = isis::GlobalModelData::Instance.GetComponentDataFromModel(p_cmp_path);
        action->mutable_payload()->add_components()->set_componentid(cdata->componentID);
    }


    edit->add_origin(origin);
    edit->set_sequence(1);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    ProSelectionarrayFree(selection);

    return PRO_TK_NO_ERROR;
}

// Sends a "connect" message to Cyphy (connecting components)
int SendConnectComponent(const string &guid, const string &connector1, const string &connector2)
{
    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);

    meta::Action* action = edit->add_actions();
    action->set_actionmode(meta::Action_ActionMode_CONNECT_COMPONENTS_CYPHY);
    action->mutable_payload()->add_components()->set_componentid(guid);
    action->mutable_payload()->add_components()->set_componentid(connector1);
    action->mutable_payload()->add_components()->set_componentid(connector2);

    edit->add_origin(origin);
    edit->set_sequence(1);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    return 0;
}

// The user pressed the "Connect" button
static void DoConnect(char* dialog, char* component, ProAppData data)
{
    int n_sel = 0;
    char **selection = 0;
    ProUIListSelectednamesGet(dialog, "LeftDatumList", &n_sel, &selection);
    if(n_sel!=1)
    {
        ErrorDialogStr("Please select 1 connector from each component");
        return;
    }
    string connector1(selection[0]);
    ProStringarrayFree(selection, n_sel);

    ProUIListSelectednamesGet(dialog, "RightDatumList", &n_sel, &selection);
    if(n_sel!=1)
    {
        ErrorDialogStr("Please select 1 connector from each component");
        return;
    }
    string connector2(selection[0]);
    ProStringarrayFree(selection, n_sel);

    SendConnectComponent(isis::GlobalModelData::Instance.designId, connector1, connector2);

    ProUIDialogExit(dialog, 0);

    return;
}


/**
Connect 2 components

Pre-conditions:
 * MetaLink is running and CyPhy and CAD-assebler are connected
 * Creo is opened in design editing mode.
Action:
 * Offers selection of connectors in a dialog
 * Sends a message to Cyph to connect the selected parts
 Post-condition:
 * The updates are reflected in the CyPhy component.
   - Cyphy will send a refresh message so they are reflected in Creo as well
*/
ProError DoConnectComponents()
{
    if(!CheckMode(isis::DESIGNEDIT)) return PRO_TK_NO_ERROR;
    char *dialog = "connect_dialog";
    ProMdl mainMdl;
    ProMdlCurrentGet(&mainMdl);
    int dialog_status;
    ProError status = ProUIDialogCreate(dialog, dialog);
    ProSelection *selection = 0;
    int selsize;
    ProSelbufferSelectionsGet(&selection);
    ProArraySizeGet(selection,&selsize);
    if(selsize!=2)
    {
        ErrorDialogStr("Please select exactly 2 components to connect.");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    ProModelitem p_mdl_item;
    ProMdl mdl = 0;
    // Left component
    ProSelectionModelitemGet(selection[0], &p_mdl_item);
    if(p_mdl_item.type != PRO_ASSEMBLY && p_mdl_item.type != PRO_PART)
    {
        ErrorDialogStr("Both selected items must be a part/assembly.");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    ProModelitemMdlGet(&p_mdl_item,&mdl);
    ProAsmcomppath p_cmp_path;
    ProIdTable comp_id_table;
    comp_id_table[0] = -1;
    status = ProAsmcomppathInit((ProAssembly)mainMdl, comp_id_table, 0, &p_cmp_path);
    ProSelectionAsmcomppathGet(selection[0], &p_cmp_path);
    const isis::CADComponentData *cdataleft = isis::GlobalModelData::Instance.GetComponentDataFromModel(p_cmp_path);
    if(cdataleft==0)
    {
        ErrorDialogStr("Internal error. Can't find component data for selected component");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    if(cdataleft->connectors.size()==0)
    {
        ErrorDialogStr(cdataleft->displayName + " doesn't have any connectors defined. Plese check the cyphy model.");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    // Right component
    ProSelectionModelitemGet(selection[1], &p_mdl_item);
    if(p_mdl_item.type != PRO_ASSEMBLY && p_mdl_item.type != PRO_PART)
    {
        ErrorDialogStr("Both selected items must be a part/assembly.");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    ProModelitemMdlGet(&p_mdl_item,&mdl);
    ProIdTable comp_id_table2;
    comp_id_table2[0] = -1;
    status = ProAsmcomppathInit((ProAssembly)mainMdl, comp_id_table2, 0, &p_cmp_path);
    ProSelectionAsmcomppathGet(selection[1], &p_cmp_path);
    const isis::CADComponentData *cdataright = isis::GlobalModelData::Instance.GetComponentDataFromModel(p_cmp_path);
    if(cdataright==0)
    {
        ErrorDialogStr("Internal error. Can't find component data for selected component");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    if(cdataright->connectors.size()==0)
    {
        ErrorDialogStr(cdataright->displayName + " doesn't have any connectors defined. Plese check the cyphy model.");
        ProSelectionarrayFree(selection);
        return PRO_TK_NO_ERROR;
    }
    ProSelectionarrayFree(selection);
    isis::MultiFormatString mstr(cdataleft->displayName);
    ProUILabelTextSet(dialog, "LeftLabel", const_cast<wchar_t*>((const wchar_t*)mstr));
    mstr = isis::MultiFormatString(cdataright->displayName);
    ProUILabelTextSet(dialog, "RightLabel", const_cast<wchar_t*>((const wchar_t*)mstr));
    char** idsleft;
    wchar_t** namesleft;
    idsleft = new char*[cdataleft->connectors.size()];
    namesleft = new wchar_t*[cdataleft->connectors.size()];
    int i = 0;
    for(vector<isis::CADComponentConnector>::const_iterator it = cdataleft->connectors.begin(); it != cdataleft->connectors.end(); ++it)
    {
        string id = it->id+"_"+it->parentid;
        idsleft[i] = new char[id.size()+1];
        strcpy(idsleft[i], id.c_str());
        isis::MultiFormatString mstr(it->displayName);
        namesleft[i] = new wchar_t[mstr.size()+1];
        wcscpy(namesleft[i], (const wchar_t*)mstr);
        i++;
    }
    ProUIListNamesSet(dialog, "LeftDatumList", cdataleft->connectors.size(), idsleft);
    ProUIListLabelsSet(dialog, "LeftDatumList", cdataleft->connectors.size(), namesleft);
    char** idsright;
    wchar_t** namesright;
    idsright = new char*[cdataright->connectors.size()];
    i=0;
    namesright = new wchar_t*[cdataright->connectors.size()];
    for(vector<isis::CADComponentConnector>::const_iterator it = cdataright->connectors.begin(); it != cdataright->connectors.end(); ++it)
    {
        string id = it->id+"_"+it->parentid;
        idsright[i] = new char[id.size()+1];
        strcpy(idsright[i], id.c_str());
        isis::MultiFormatString mstr(it->displayName);
        namesright[i] = new wchar_t[mstr.size()+1];
        wcscpy(namesright[i], (const wchar_t*)mstr);
        i++;
    }

    ProUIListNamesSet(dialog, "RightDatumList", cdataright->connectors.size(), idsright);
    ProUIListLabelsSet(dialog, "RightDatumList", cdataright->connectors.size(), namesright);

    ProUIListSelectionpolicySet(dialog, "RightDatumList", PROUISELPOLICY_SINGLE);
    ProUIListSelectionpolicySet(dialog, "LeftDatumList", PROUISELPOLICY_SINGLE);

    status = ProUIDialogCloseActionSet(dialog, StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "ConnectBtn", DoConnect, NULL);
    status = ProUIDialogActivate(dialog, &dialog_status);

    for(int i = 0; i < cdataleft->connectors.size(); i++)
    {
        delete idsleft[i];
        delete namesleft[i];
    }
    delete[] idsleft;
    delete[] namesleft;
    for(int i = 0; i < cdataright->connectors.size(); i++)
    {
        delete idsright[i];
        delete namesright[i];
    }
    delete[] idsright;
    delete[] namesright;

    ProSelectionarrayFree(selection);

    status = ProUIDialogDestroy(dialog);
    return PRO_TK_NO_ERROR;
}


/**
Add a component to a design in Creo (Creo Add).

Pre-conditions:
 * MetaLink is running and CyPhy and CAD-assebler are connected
 * Creo is opened in assembly design editing mode.
   - CyPhy switches Creo to assembly design editing mode.
   - CyPhy starts CAD-assembler in assembly design editing mode.
Action:
 * Press the Add CyPhy Component button.
 * Select the AVM component to be added by name.
 * Enter the component instance name.
Post-condition:
 * The new component instance appears.
    - in CyPhy - ready to be constrained.
	- in Creo - unconstrained at the base location.

*/
ProError DoAddComponent()
{
    if(!CheckMode(isis::DESIGNEDIT)) return PRO_TK_NO_ERROR;
    

    RequestComponentList();

	return PRO_TK_NO_ERROR;

}

static int SendCreateConnectorMessage(const string &avmId, const vector<DatumInfo> &datumList, const string &connectorName, const string &connectorId)
{
    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);
//    edit->add_topic(avmId);

    meta::Action* action = edit->add_actions();
	action->set_actionmode(meta::Action_ActionMode_ADD_CONNECTOR_TO_COMPONENT);
	action->set_subjectid(avmId);
    meta::ConnectorType *connector = action->mutable_payload()->add_connectors();
    connector->set_id(connectorId);
    connector->set_displayname(connectorName);
    for(vector<DatumInfo>::const_iterator it = datumList.begin(); it != datumList.end(); ++it)
    {
        meta::ConnectorDatumType *datum = connector->add_datums();
        datum->set_id(it->datumName);
        datum->set_displayname(it->cyphyName);
        datum->set_alignment((it->alignment)?meta::ConnectorDatumType_AlignType_MATE:meta::ConnectorDatumType_AlignType_ALIGN);
    }

    edit->add_origin(origin);
    edit->set_sequence(1);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    return 0;
}

static void GetSelectedDatumNames(vector<DatumInfo> &names, bool (*filter)(ProFeature*))
{

    int selsize = 0;
    ProSelection *datumSelection = 0;
    ProSelbufferSelectionsGet(&datumSelection);
    ProArraySizeGet(datumSelection,&selsize);
    ProModelitem p_mdl_item;
    for(int i = 0; i < selsize; i++)
    {
        ProSelectionModelitemGet(datumSelection[i], &p_mdl_item);
        if(p_mdl_item.type != PRO_FEATURE)
            continue;
        ProFeattype type;
        ProFeatureTypeGet((ProFeature*)&p_mdl_item, &type);
        if(isis::IsDatumType(type))
        {
            if(filter && !filter((ProFeature*)&p_mdl_item))
            {
                continue;
            }
            ProElempath path;
            ProValue value;
            ProValueData  value_data;
            ProElempathItem constr_type[3];

            constr_type[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
            constr_type[0].path_item.elem_id = PRO_E_STD_FEATURE_NAME;

            ProElempathAlloc(&path);
            ProElempathDataSet(path, constr_type, 1);
            ProError error = ProFeatureElemValueGet((ProFeature*)&p_mdl_item, path, &value);
            wstring wname;
            ostringstream ostream;
            if(error == PRO_TK_NO_ERROR)
            {
                ProValueDataGet(value, &value_data);
                wname = wstring(value_data.v.w);
                if(wname==L"no_name") continue;
            }
            else
            {
                continue;
            }
            DatumInfo info;
            info.cyphyName = GetCyphyName((ProFeature*)&p_mdl_item);
            info.datumName = string(wname.begin(),wname.end());
            names.push_back(info);
        }
    }
    ProSelectionarrayFree(datumSelection);
}

static void ShowDatumSetupDialog(vector<DatumInfo> &datumNames)
{
    int dialog_status;
    ProError status;
    int reqresult;
    char *dialog = "connector_datums_dialog";
    status = ProUIDialogCreate(dialog, dialog);

    ProUIGridopts opts;
    PRO_UI_GRIDOPTS_DEFAULT(opts);
    opts.column = 0;

    //ProUIDialogLayoutAdd(dialog, "Layout", &opts);

    char **optnames = new char*[2];
    optnames[0] = "ALIGN";
    optnames[1] = "MATE";

    wchar_t **optlabels = new wchar_t*[2];
    optlabels[0] = L"ALIGN";
    optlabels[1] = L"MATE";

    for(vector<DatumInfo>::const_iterator it = datumNames.begin(); it != datumNames.end(); ++it)
    {
        //ProUIDialogLayoutAdd(dialog, "Layout", &opts);
        ProUIDialogSeparatorAdd(dialog, "x", &opts);
        string n = string("opt_")+it->datumName;
        ProUIDialogOptionmenuAdd(dialog, const_cast<char*>(n.c_str()), &opts);
        ProUIOptionmenuNamesSet(dialog, const_cast<char*>(n.c_str()), 2, optnames);
        ProUIOptionmenuLabelsSet(dialog, const_cast<char*>(n.c_str()), 2, optlabels);
        n = string("text_")+it->datumName;
        ProUIDialogInputpanelAdd(dialog, const_cast<char*>(n.c_str()), &opts);
        ProUIInputpanelInputtypeSet(dialog, const_cast<char*>(n.c_str()), PROUIINPUTTYPE_STRING);
        if(it->cyphyName!="")
        {
            ProUIInputpanelStringSet(dialog, const_cast<char*>(n.c_str()), const_cast<char*>((it->cyphyName).c_str()));
        }
        else
        {
            ProUIInputpanelStringSet(dialog, const_cast<char*>(n.c_str()), const_cast<char*>((it->datumName).c_str()));
        }
    }

    ProUIDialogLabelAdd(dialog, "Title", &opts);
    ProUILabelTextSet(dialog, "Title", L"Enter the names for the connections to be created:");

    status = ProUIPushbuttonActivateActionSet(dialog, "OkBtn", StdCloseAction, NULL);

    delete[] optlabels;

    delete[] optnames;

    ProUIDialogActivate(dialog, &dialog_status);

    for(vector<DatumInfo>::iterator it = datumNames.begin(); it != datumNames.end(); ++it)
    {
        string n = string("text_")+it->datumName;
        char* value;
        status = ProUIInputpanelStringGet(dialog, const_cast<char*>(n.c_str()), &value);
        if(status == PRO_TK_NO_ERROR)
        {
            it->cyphyName = string(value);
            ProStringFree(value);
        }
        n = string("opt_")+it->datumName;
        int n_sel;
        char **selections;
        status = ProUIOptionmenuSelectednamesGet(dialog, const_cast<char*>(n.c_str()), &n_sel, &selections);
        if(status == PRO_TK_NO_ERROR)
        {
            if(n_sel>0)
            {
                if(strstr(selections[0],"ALIGN"))
                {
                    it->alignment = 0;
                }
                else
                {
                    it->alignment = 1;
                }
            }
        }
        else
        {
            it->alignment = 0;
        }
    }

    status = ProUIDialogDestroy(dialog);
}

static void AddToExistingConnectorAction(char* dialog, char* component, ProAppData data)
{
    int n_sel = 0;
    char **selection = 0;
    ProUIListSelectednamesGet(dialog, "ConnectorList", &n_sel, &selection);
    if(n_sel!=1)
    {
        ErrorDialogStr("Please select a connector.");
        return;
    }
    string connectorName(selection[0]);
    ProStringarrayFree(selection, n_sel);

    vector<DatumInfo> origDatumNames;
    GetSelectedDatumNames(origDatumNames, 0);

    ShowDatumSetupDialog(origDatumNames);

    SendCreateConnectorMessage(isis::GlobalModelData::Instance.ComponentEdit.avmId, origDatumNames, "", connectorName);

    ProUIDialogExit(dialog, 0);
}


static void CreateNewConnectorAction(char* dialog, char* component, ProAppData data)
{
    vector<DatumInfo> origDatumNames;
    GetSelectedDatumNames(origDatumNames, 0);
    wchar_t* lines;
    ProError status = ProUITextareaValueGet(dialog, "NewConnectorName", &lines);

    isis::MultiFormatString str(lines);

    if(str.size()==0)
    {
        ErrorDialog(L"Invalid name.");
        return;
    }

    ShowDatumSetupDialog(origDatumNames);

    SendCreateConnectorMessage(isis::GlobalModelData::Instance.ComponentEdit.avmId, origDatumNames, str, "");

    ProWstringFree(lines);

    ProUIDialogExit(dialog,0);
}

static void UseExistingConnectorAction(char* dialog, char* component, ProAppData data)
{
    int dialog_status;
    ProError status;
    int reqresult;
    char *seldialog = "connector_select_dialog";

    if(isis::GlobalModelData::Instance.ComponentEdit.connectors.size()==0)
    {
        ErrorDialog(L"Component contains no connectors.");
        return;
    }

    status = ProUIDialogCreate(seldialog, seldialog);
    if(status != PRO_TK_NO_ERROR)
        return;
    status = ProUIDialogCloseActionSet(seldialog, StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(seldialog, "OkBtn", AddToExistingConnectorAction, NULL);

    char** ids;
    wchar_t** names;
    ids = new char*[isis::GlobalModelData::Instance.ComponentEdit.connectors.size()];
    int i=0;
    names = new wchar_t*[isis::GlobalModelData::Instance.ComponentEdit.connectors.size()];
    for(vector<isis::CADComponentConnector>::const_iterator it2 = isis::GlobalModelData::Instance.ComponentEdit.connectors.begin(); it2 != isis::GlobalModelData::Instance.ComponentEdit.connectors.end(); ++it2)
    {
        string id = it2->id;
        ids[i] = new char[id.size()+1];
        strcpy(ids[i], id.c_str());
        isis::MultiFormatString mstr(it2->displayName);
        names[i] = new wchar_t[mstr.size()+1];
        wcscpy(names[i], (const wchar_t*)mstr);
        i++;
    }

    ProUIListSelectionpolicySet(seldialog, "ConnectorList", PROUISELPOLICY_SINGLE);

    status = ProUIListNamesSet(seldialog, "ConnectorList", isis::GlobalModelData::Instance.ComponentEdit.connectors.size(), ids);
    status = ProUIListLabelsSet(seldialog, "ConnectorList", isis::GlobalModelData::Instance.ComponentEdit.connectors.size(), names);

    status = ProUIDialogActivate(seldialog, &dialog_status);

    for(int i = 0; i < isis::GlobalModelData::Instance.ComponentEdit.connectors.size(); i++)
    {
        delete ids[i];
        delete names[i];
    }
    delete[] ids;
    delete[] names;

    status = ProUIDialogDestroy(seldialog);

    if(dialog_status != 1)
        ProUIDialogExit(dialog, 0);

    return;
}

ProError DoCreateConnector()
{
    if(!CheckMode(isis::COMPONENTEDIT)) return PRO_TK_NO_ERROR;
    int dialog_status;
    ProError status;
    int reqresult;
    char *dialog = "connector_add_dialog";

    vector<DatumInfo> datumNames;
    GetSelectedDatumNames(datumNames, 0);

    if(datumNames.size()==0)
    {
        ErrorDialog(L"Please select at least one datum.");
        return PRO_TK_NO_ERROR;
    }

    status = ProUIDialogCreate(dialog, dialog);
    status = ProUIDialogCloseActionSet(dialog, StdCloseAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "CreateNewBtn", CreateNewConnectorAction, NULL);
    status = ProUIPushbuttonActivateActionSet(dialog, "UseExistingBtn", UseExistingConnectorAction, NULL);

    status = ProUIDialogActivate(dialog, &dialog_status);

    status = ProUIDialogDestroy(dialog);

    return status;
}


static int SendAddAnalysisPointsMessage(const string &avmId, const vector<DatumInfo> &datumList)
{
    isis::EditPointer edit(new meta::Edit());
    edit->set_editmode(meta::Edit_EditMode_POST);

    edit->set_guid(boost::uuids::to_string(boost::uuids::random_generator()()));

	edit->add_topic(isis::GlobalModelData::Instance.instanceId);
//    edit->add_topic(avmId);

    meta::Action* action = edit->add_actions();
    action->set_actionmode(meta::Action_ActionMode_CREATE_ANALYSIS_POINTS);
	action->set_subjectid(avmId);
    for(vector<DatumInfo>::const_iterator it = datumList.begin(); it != datumList.end(); ++it)
    {
        meta::DatumType *datum = action->mutable_payload()->add_datums();
        datum->set_name(it->datumName);
    }

    edit->add_origin(origin);
    edit->set_sequence(1);

    isis::GlobalModelData::Instance.metalink_handler_ptr->send(edit);

    return 0;
}

bool DatumPointFilter(ProFeature *feature)
{
    ProFeattype type;
    ProFeatureTypeGet(feature, &type);
    return type == PRO_FEAT_DATUM_POINT;
}

ProError DoCreateAnalysisPoints()
{
    if(!CheckMode(isis::COMPONENTEDIT)) return PRO_TK_NO_ERROR;
    vector<DatumInfo> datumNames;
    GetSelectedDatumNames(datumNames, DatumPointFilter);
    if(datumNames.size()==0)
    {
        ErrorDialog(L"Please select at least one point datum.");
        return PRO_TK_NO_ERROR;
    }

    SendAddAnalysisPointsMessage(isis::GlobalModelData::Instance.ComponentEdit.avmId, datumNames);

    return PRO_TK_NO_ERROR;
}

static ProError FeatVisitElemtreeDump(ProFeature* p_feature, ProError status, ProAppData app_data)
{
    ProAsmcomppath *assem_path = (ProAsmcomppath*)app_data;
    map<string, ProMatrix> *matrixmap = (map<string, ProMatrix>*)app_data;
    ProFeattype type;
    ProFeatureTypeGet(p_feature, &type);
    if(type==PRO_FEAT_COMPONENT)
    {
        ProMdl mdl;
        ProError err;
        err = ProAsmcompMdlGet(p_feature, &mdl);
        if(err == PRO_TK_NO_ERROR)
        {
            ProMdlType p_type;
            ProMdlTypeGet(mdl, &p_type);
            if(p_type==PRO_MDL_ASSEMBLY || p_type==PRO_MDL_PART)
            {
                ProElempath path;
                ProValue value;
                ProValueData  value_data;
                ProElempathItem constr_type[3];

                constr_type[0].type = PRO_ELEM_PATH_ITEM_TYPE_ID;
                constr_type[0].path_item.elem_id = PRO_E_STD_FEATURE_NAME;

                ProElempathAlloc(&path);
                ProElempathDataSet(path, constr_type, 1);
                ProError error = ProFeatureElemValueGet(p_feature, path, &value);

                ProElement elemTree;
                ProFeatureElemtreeExtract(p_feature, assem_path, PRO_FEAT_EXTRACT_NO_OPTS, &elemTree);
                wostringstream ostr;
                ostr << p_feature->id;
                ostr << L"elemtree.xml";
                ProPath filepath;
                wcscpy(filepath, ostr.str().c_str());
                //filepath[len]=0;
                ProElemtreeWrite(elemTree, PRO_ELEMTREE_XML, filepath);

                //ProSolidFeatVisit(ProMdlToSolid(mdl), FeatVisitElemtreeDump, NULL, matrixmap);
            }
        }
    }
    return PRO_TK_NO_ERROR;
}




ProError DoDumpPosition()
{
    ProMdl mainMdl;
    ProMdlCurrentGet(&mainMdl);
    CreoMetaDataExporter e(mainMdl);
    e.Export("constraintdata.xml");

    ProAsmcomppath assem_path;

    ProIdTable c_id_table;
    c_id_table [0] = -1;
    isis::isis_ProAsmcomppathInit((ProSolid)mainMdl, c_id_table, 0, &assem_path);

    ProSolidFeatVisit(ProMdlToSolid(mainMdl), FeatVisitElemtreeDump, NULL, &assem_path);

    return PRO_TK_NO_ERROR;
}

ProError DoDatumResolve()
{
    int selsize;
    ProSelection *datumSelection = 0;
    ProSelbufferSelectionsGet(&datumSelection);
    ProArraySizeGet(datumSelection,&selsize);
    ProModelitem p_mdl_item;
    ProAsmcomppath p_cmp_path;
    if(selsize==0) return PRO_TK_NO_ERROR;

    isis::DatumRefResolver resolver(datumSelection[0]);

    try
    {
        vector<ProSelection> result;
        resolver.Resolve(result);
        for(vector<ProSelection>::const_iterator it = result.begin(); it != result.end(); ++it)
        {
            ProModelitem item;
            ProSelectionModelitemGet(*it, &item);
            ProName name;
            ProMdlNameGet(item.owner, name);
            isis::CreoMessageDialog(name);
        }
    }
    catch(isis::application_exception &e)
    {
        isis::CreoMessageDialog(e.what());
    }

    return PRO_TK_NO_ERROR;
}