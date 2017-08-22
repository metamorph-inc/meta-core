/**
 * the-lift.cpp
 * Copyright (c) 2013 Fred Eisele (phreed at gmail dot com)
 *
 * Distributed under the Boost Software License, Version 1.0. (See accompanying
 * file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */

#include "BridgeClient.h"
#include "FramedEdit.h"

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <boost/cstdint.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <google/protobuf/text_format.h>
// #include <boost/log/trivial.hpp>

namespace isis {
	namespace asio = boost::asio;
	using asio::ip::tcp;
	using boost::uint8_t;

	namespace meta = edu::vanderbilt::isis::meta;
	namespace pb = google::protobuf;

	typedef boost::function<bool ()> OutboundEventHandler;

	/**
	 * connection - handles a connection with a single client.
	 * Create only through the BridgeConnection::create factory.
	 * <p>
	 * As messages are received they are placed in a queue which
	 * are then passed to the CREO api to update the model.
	 */
	class BridgeConnection : public boost::enable_shared_from_this<BridgeConnection> {
	public:

		BridgeConnection(asio::io_service& io_service, OutboundEventHandler out_edit, InboundHandler in_edit) :
		        //m_logcat( ::log4cpp::Category::getInstance(std::string("metalink.bridge.connection")) ),
				m_socket(io_service), m_outbound_handler_ref(out_edit), m_inbound_handler_ref(in_edit),
					m_framed_edit(boost::shared_ptr<meta::Edit>(new meta::Edit())), 
					m_ready(false) 
		{
			isis_LOG(lg, isis_FILE, isis_INFO) << "instantiate socket";
		}


		void start(boost::asio::ip::tcp::resolver::iterator it) {
			isis_LOG(lg, isis_FILE, isis_INFO) << "BridgeConnection::start "  << " host-name: " << it->host_name();
			m_socket.async_connect(*it, 
				boost::bind(&BridgeConnection::handle_connect,
							this, asio::placeholders::error));
			isis_LOG(lg, isis_FILE, isis_INFO) << "started";
		}

		void send(EditPointer editPtr) {
			if (! m_ready) {
				isis_LOG(lg, isis_FILE, isis_INFO) << "send not ready";
				return;
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "BridgeConnection::send";
			start_write_outbound(editPtr);
		}

		/** 
		* shut down any threads drop connections etc.
		* A dummy edit needs to be added to the out-bound queue to get it to unblock.
		*/
		void disconnect() {
			this->m_ready = false;
			m_socket.close();
		}

	private:
		//::log4cpp::Category& m_logcat;  

		tcp::socket m_socket;
		OutboundEventHandler m_outbound_handler_ref;
		InboundHandler m_inbound_handler_ref;
		FramedEdit m_framed_edit;
	    boost::atomic<bool> m_ready;

		
		void handle_connect(const boost::system::error_code& error) {
			if (error) {
				isis_LOG(lg, isis_FILE, isis_ERROR) << "BridgeConnection::handle_connect, error: " << error.message(); 
				return;
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "connection established";
			m_ready = true;
			start_read_inbound();
			isis_LOG(lg, isis_FILE, isis_INFO) << "BridgeConnection::handle_connect after : start_read";
			m_outbound_handler_ref();
		}

		void start_read_inbound() {
			isis_LOG(lg, isis_FILE, isis_INFO) << "BridgeConnection::start_read_inbound";
			m_framed_edit.resize_input_buffer_for_header();
			asio::async_read(m_socket, m_framed_edit.get_input_buffer_for_header(),
					boost::bind(&BridgeConnection::handle_read_header,
							this, asio::placeholders::error));
		}

		void handle_read_header(const boost::system::error_code& error) {
			if (error) {
				if (m_ready == true) {
					isis_LOG(lg, isis_FILE, isis_ERROR) << "BridgeConnection::handle_read_header " << error.message();
				}
				else {
					// shutting down
				}
				return;
			}
			isis_LOG(lg, isis_FILE, isis_DEBUG) << "Got header!" << '\n' << m_framed_edit.show_input_buffer();
			unsigned payload_length = m_framed_edit.decode_header();
			if (payload_length < 1) {
				start_read_inbound();
				return;
			}
			isis_LOG(lg, isis_FILE, isis_DEBUG) << payload_length << " bytes";
			start_read_body();
		}

		/**
		 * called once a header has been successfully read.
		 * m_readbuf already contains the header in the first HEADER_SIZE bytes after start.
		 * Expand it to fit in the body as well, and start async read into the body.
		 * The final checksum is not included in the payload length.
		 */
		void start_read_body() {
			m_framed_edit.resize_input_buffer_for_load();
		
			asio::async_read(m_socket, m_framed_edit.get_input_buffer_for_payload(),
					boost::bind(&BridgeConnection::handle_read_body,
							this, asio::placeholders::error));
		}

		void handle_read_body(const boost::system::error_code& error) {	
			if (error) {
				isis_LOG(lg, isis_FILE, isis_ERROR) << "BridgeConnection::handle_read_body " << error.message();
				return;
			}
			isis_LOG(lg, isis_FILE, isis_DEBUG) << "Got body!";
			// R.O. Leave out buffer    isis_LOG(lg, isis_FILE, isis_DEBUG) << "Got body!" << '\n' << m_framed_edit.show_input_buffer();
			handle_request();
			start_read_inbound();
		}

		/**
		 * Called when enough data was read into m_readbuf for a complete request message.
		 * TODO: This is where the call to CREO is made.
		 */
		void handle_request() {
			bool success = m_framed_edit.unpack();
			if (!success) {
				isis_LOG(lg, isis_FILE, isis_WARN) << "handle request could not unpack ";
				return;
			}
			EditPointer edit = m_framed_edit.get_load();
			isis_LOG(lg, isis_FILE, isis_INFO) << "edit " << edit->DebugString();
			/** This is where the edit gets passed off to the incoming edit queue */
			m_inbound_handler_ref(edit);
			m_framed_edit.get_load()->Clear();
		}

		bool start_write_outbound(EditPointer editPtr) {
			FramedEdit framed_edit(editPtr);
			isis_LOG(lg, isis_FILE, isis_INFO) << "request framed";
			isis::data_buffer backing_buff;
			framed_edit.pack(backing_buff);
			isis_LOG(lg, isis_FILE, isis_INFO) << "request packed " << backing_buff.size();
			boost::asio::mutable_buffers_1 sendable = boost::asio::buffer(backing_buff);
			
			boost::asio::async_write(m_socket, sendable, 
				boost::bind(&BridgeConnection::handle_write_outbound_complete, 
				             this, 
				             asio::placeholders::error, asio::placeholders::bytes_transferred)); 

			isis_LOG(lg, isis_FILE, isis_INFO) << "requested outbound " << editPtr->DebugString();
			return true;
		}

		void handle_write_outbound_complete( 
			const boost::system::error_code& error, 
			std::size_t bytes_transferred)
		{   
			if (error) {
			    isis_LOG(lg, isis_FILE, isis_ERROR) << "BridgeConnection::handle_write_outbound_complete " << error.message();
			    return;
			}
			isis_LOG(lg, isis_FILE, isis_INFO) << "write complete " << " bytes " << bytes_transferred;
		}


	};

	struct BridgeClient::BridgeClientImpl { 
		//::log4cpp::Category& m_logcat;  
		tcp::resolver m_resolver;
		
		std::string m_host;
		std::string m_service;

		BridgeConnection m_connection;
		

		BridgeClientImpl(asio::io_service& io_service, 
			std::string host, std::string service, 
			OutboundEventHandler outbound_handler, InboundHandler inbound_handler) :  
		//m_logcat( ::log4cpp::Category::getInstance(std::string("metalink.bridge.client.impl")) ),
		m_resolver(io_service), m_host(host), m_service(service), 
			m_connection(io_service, outbound_handler, inbound_handler)
		{
			isis_LOG(lg, isis_FILE, isis_INFO)  << "BridgeClientImpl::ctor " << m_host << ":" << service;
		}

		void start_resolve() {
			isis_LOG(lg, isis_FILE, isis_INFO)  << "BridgeClientImpl::start_resolve";
			tcp::resolver::query query(m_host, m_service);
			isis_LOG(lg, isis_FILE, isis_INFO)  << "resolver query " << query.host_name() << ":" << query.service_name();
			m_resolver.async_resolve(query, 
				boost::bind(&BridgeClientImpl::handle_resolution, this, 
					asio::placeholders::error,  boost::asio::placeholders::iterator));
		}

		/**
		 * Called with connection iterator for each interface (should be just one).
		 */
		void handle_resolution(const boost::system::error_code &error, tcp::resolver::iterator it) {
		  if (error)  {
			  isis_LOG(lg, isis_FILE, isis_WARN) << "could not resolve host: " << m_host 
				  << " and service: " << m_service
				  << " with error: " << error.message();
			  return;
		  }
		  isis_LOG(lg, isis_FILE, isis_INFO) << "BridgeClient::handle_resolution ";
		  m_connection.start(it);
		}

		void send(EditPointer edit_ptr) {
			m_connection.send(edit_ptr);
		}

		void disconnect() {
			m_connection.disconnect();
		}

	};


	BridgeClient::BridgeClient(boost::asio::io_service& io_service, 
			const std::string host, const std::string service,
			OutboundHandler outbound_handler,
			InboundHandler inbound_handler) :
		//m_logcat( ::log4cpp::Category::getInstance(std::string("metalink.bridge.client")) ),
			impl(new BridgeClientImpl(io_service, host, service, boost::bind(outbound_handler, this), inbound_handler)) 
	{
		impl->start_resolve();
	}

	BridgeClient::~BridgeClient() {
	}

	void BridgeClient::disconnect() {
		impl->disconnect();
	}

	void BridgeClient::send(EditPointer edit_ptr) {
		impl->send(edit_ptr);
	}

} // isis namespace