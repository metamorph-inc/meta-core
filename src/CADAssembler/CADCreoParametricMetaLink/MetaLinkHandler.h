#ifndef METALINK_HANDLER_H
#define METALINK_HANDLER_H

/**
This object is intended to be run in a thread where it
processes EditPointer objects from a deque.
Each object is extracted and the proper Creo methods are called.
*/

/**
For an example using this <b>handler</h> object see "meta-link-main.cpp".

*/

/** the header defining the EditPointer objects */
#include "gen/MetaLinkMsg.pb.h"
#include "BridgeClient.h"
#include <AssemblyEditingViaLink.h>


/* */
#include <CommonUtilities.h>
#include <ISISConstants.h>
#include <AssembleUtils.h>

#include "CADEnvironmentSettings.h"
#include "WindowsFunctions.h"

#include <InputArgumentsParser.h>
#include <AssemblyEditingViaLink.h>
#include <Test_MetaLink.h>
#include <sstream>
#include <time.h>
#include <queue>

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <boost/thread/thread.hpp>

#include <boost/atomic.hpp>
#include <boost/thread/mutex.hpp>
#include <log4cpp/Category.hh>

#include "gen/MetaLinkMsg.pb.h"
#include "EventLoopMonitor.h"
/* */

namespace isis {
extern const std::string CAD_ASSEMBLY_TOPIC;
extern const std::string CAD_COMPONENT_TOPIC;

class MetaLinkHandler {
public:

    MetaLinkHandler(isis::MetaLinkAssemblyEditor::Pointer assembler_ptr,
                    boost::asio::io_service& io_service,
                    std::string host,
                    std::string service,
                    const std::string &in_MajorMode,
                    const std::string &in_TargetID,
                    EventLoopMonitor &in_proToolkitMonitor);

    boost::atomic<bool> m_ready;
    boost::atomic<int> m_sequence;
    EventLoopMonitor &m_proToolkitMonitor;
    queue<isis::EditPointer> m_eventQueue;

    /**
    Express an interest in certain message sources.
    This will also cause an owner of the object to send a current version.
    */
    bool interest(isis::BridgeClient* client, const std::string &in_MajorMode, const std::string &in_TargetID);

    /**
    Process an edit.
    Remember an edit may have many raw payload elements.
    */
    bool process(isis::EditPointer edit_ptr);
    bool processEdit(isis::EditPointer edit_ptr);

    //void connect(boost::asio::io_service& io_service, std::string host, std::string service);
    void disconnect();

    void interrupt();

    void send(const EditPointer edit) {
        m_client.send(edit);
    }

    void change_m_operator(const std::string str) {
        m_operator = str;
    }

    typedef pb::RepeatedPtrField< meta::CADComponentType > ComponentRepeated;
    typedef pb::RepeatedPtrField< meta::ConstraintType > ConstraintRepeated;
    typedef pb::RepeatedPtrField< meta::ParametricParametersType > ParametricParametersRepeated;

    typedef ComponentRepeated::iterator ComponentIterator;

    bool send_LocateSelectedRequest( const isis::MetaLinkMode &in_MajorMode , const std::string &in_TargetID );

	// Debug method for command-line assembly creation
	void CreateAssembly(std::string xmlfile);

private:
    ::log4cpp::Category& m_logcat;

    isis::BridgeClient m_client;
    boost::thread m_network_thread;
    std::string m_operator;
    const std::string m_target_id;
    boost::asio::io_service& m_ioservice;

    isis::MetaLinkAssemblyEditor::Pointer m_assembler;

    bool MetaLinkHandler::process_PassivePost( isis::EditPointer edit);

    bool process_AssemblyDesignPost(isis::EditPointer edit_ptr);
    bool process_AssemblyDesignPost_select( isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action, std::string in_AssemblyTopic, std::string in_AssemblyInstanceId );
    bool process_AssemblyDesignPost_insert( isis::EditPointer edit, int actionIx, meta::Action *action, std::string assemblyTopic, std::string assemblyInstanceId );
    bool process_AssemblyDesignPost_clear( isis::EditPointer edit, int actionIx, meta::Action *action, std::string assemblyTopic, std::string assemblyInstanceId );
    bool process_AssemblyDesignPost_update( isis::EditPointer edit,  int actionIx, meta::Action *action, std::string assemblyTopic, std::string assemblyInstanceId);
    meta::Notice process_ComponentAdded( const std::string &in_AssemblyInstanceID, meta::CADComponentType &in_component) throw (isis::application_exception);
    meta::Notice process_ComponentModified( const std::string &in_AssemblyInstanceID, meta::CADComponentType &in_component) throw (isis::application_exception);
    meta::Notice process_ConstraintAdded( const std::string &in_AssemblyInstanceID, meta::ConstraintType &in_constraint ) throw (isis::application_exception);
    meta::Notice process_ParametersModified( const std::string &in_AssemblyInstanceID, meta::ParametricParametersType &in_parameter ) throw (isis::application_exception);
    meta::Notice process_CreateAssembly(const std::string &in_AssemblyXML) 	throw (isis::application_exception);

    bool process_AvmComponentPost(isis::EditPointer edit_ptr);
    bool process_AvmComponentPost_select( isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action, std::string in_MainTopic, std::string in_AvmComponentId);
    bool process_AvmComponentPost_insert( isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action, std::string in_MainTopic, std::string in_AvmComponentId );
    bool process_AvmComponentPost_clear( isis::EditPointer in_Edit, int in_ActionIx, meta::Action *in_Action, std::string in_MainTopic, std::string in_AvmComponentId );
    meta::Notice process_AvmComponentInitialize( const std::string &in_ComponentInstanceID, meta::CADComponentType &in_component) throw (isis::application_exception);

    // If in_SearchPaths.size() == 0 then no action taken.
    meta::Notice process_SearchPaths(const std::list<std::string> &in_SearchPaths)throw (isis::application_exception);

    // helper method (should probably make an implementatation class to carry these
    void upsertComponent(isis::EditPointer edit, meta::Payload& payload);

};

} // namespace isis


#endif
