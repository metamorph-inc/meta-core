

#ifndef BRIDGE_CLIENT_H
#define BRIDGE_CLIENT_H

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/function.hpp>

#include <boost/thread/thread.hpp>
#include <boost/lockfree/spsc_queue.hpp>
#include <iostream>

#include "cc_LoggerBoost.h"

#include "gen/MetaLinkMsg.pb.h"

/**
 * Bridge server connection.
 *
 */
namespace meta = edu::vanderbilt::isis::meta;
namespace pb = google::protobuf;

namespace isis {
	class BridgeClient;

	typedef boost::shared_ptr<meta::Edit> EditPointer;
	typedef boost::lockfree::spsc_queue<EditPointer, boost::lockfree::capacity<1024> > EditQueue;
	typedef boost::shared_ptr<EditQueue> EditQueuePointer;
	typedef boost::function<bool (BridgeClient*)> OutboundHandler;
	typedef boost::function<bool (EditPointer)> InboundHandler;

	class BridgeClient {
	public:
		
		BridgeClient(boost::asio::io_service& io_service, 
			const std::string host, const std::string service,
			OutboundHandler outbound_handler,
			InboundHandler inbound_handler);

		~BridgeClient();

		void send(EditPointer edit_ptr);
		void disconnect();

	private:
		//::log4cpp::Category& m_logcat;  

		struct BridgeClientImpl;
		boost::scoped_ptr<BridgeClientImpl> impl;

	};
} // isis namespace

#endif // BRIDGE_CLIENT_H
