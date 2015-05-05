#ifndef _BRIDGECLIENTST_H_
#define _BRIDGECLIENTST_H_

#include <string>
#include "gen/MetaLinkMsg.pb.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
namespace meta = edu::vanderbilt::isis::meta;

namespace isis
{
	typedef boost::shared_ptr<meta::Edit> EditPointer;

// Single-threaded bridge client

class BridgeClientST
{
public:
	class TimeoutException
	{
	};
public:
	class Receiver
	{
	public:
		Receiver(boost::asio::ip::tcp::socket *_socket) : socket(_socket), done(false) {};
		isis::EditPointer getResult()
		{
			return result;
		}
		boost::asio::ip::tcp::socket *socket;
		isis::EditPointer result;
	private:
		bool done;
	public:
		void operator()();
	};
private:
	const std::string m_host;
	std::string m_service;
	boost::asio::io_service io_service;
	boost::asio::ip::tcp::socket *socket;
public:
	BridgeClientST(const std::string host, const std::string service);
	boost::system::error_code connect();
	void send(isis::EditPointer value );
	isis::EditPointer receive();
	isis::EditPointer receive(int timeout) throw (TimeoutException);
	~BridgeClientST();
};

std::ostream& operator <<(std::ostream & out, const BridgeClientST::TimeoutException & status) ;

}

#endif