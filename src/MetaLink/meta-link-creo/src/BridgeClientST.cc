#include "BridgeClientST.h"
#include <string>
#include <iostream>
#include "FramedEdit.h"

using namespace std;



namespace isis
{

void receive(isis::BridgeClientST::Receiver *r)
{
	try{
	FramedEdit framedEdit(boost::shared_ptr<meta::Edit>(new meta::Edit()));
	framedEdit.resize_input_buffer_for_header();
	size_t s = r->socket->read_some(framedEdit.get_input_buffer_for_header());
	unsigned payload_length = framedEdit.decode_header();
	cout << "Payload length: " << payload_length;
	framedEdit.resize_input_buffer_for_load();
	s = r->socket->read_some(framedEdit.get_input_buffer_for_payload());
	cout << "read bytes:" << s << endl;
	framedEdit.unpack();
	r->result=framedEdit.get_load();
	std::cout << "edit " << r->result->DebugString() << std::endl;
	std::cout << "Finish" << std::endl;
	} catch (boost::system::system_error e){
		std::cout << "Exceptiuon" << std::endl;
	}
}

BridgeClientST::BridgeClientST(const string host, const string service) : m_host(host), m_service(service)
{
}

boost::system::error_code BridgeClientST::connect()
{
	socket = new boost::asio::ip::tcp::socket(io_service);
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 15150);
	boost::system::error_code ec;
	socket->connect(endpoint, ec);
	return ec;
}

void BridgeClientST::send(isis::EditPointer value)
{
	FramedEdit framed_edit(value);
	isis::data_buffer backing_buff;
	framed_edit.pack(backing_buff);

	socket->send(boost::asio::buffer(backing_buff));
}

isis::EditPointer BridgeClientST::receive()
{

	FramedEdit framedEdit(boost::shared_ptr<meta::Edit>(new meta::Edit()));
	framedEdit.resize_input_buffer_for_header();
	size_t s = socket->read_some(framedEdit.get_input_buffer_for_header());
	unsigned payload_length = framedEdit.decode_header();
	cout << "Payload length: " << payload_length;
	framedEdit.resize_input_buffer_for_load();
	s = socket->read_some(framedEdit.get_input_buffer_for_payload());
	cout << "read bytes:" << s << endl;
	framedEdit.unpack();
	EditPointer edit = framedEdit.get_load();
	std::cout << "edit " << edit->DebugString() << std::endl;
	return framedEdit.get_load();

}

isis::EditPointer BridgeClientST::receive(int timeout) throw (TimeoutException)
{
	Receiver *r = new Receiver(socket);
	boost::thread t(isis::receive,r);
	if (t.try_join_for(boost::chrono::milliseconds(timeout)))
	{
		return r->getResult();
	} else {
		t.interrupt();
		throw TimeoutException();
	}

}

BridgeClientST::~BridgeClientST()
{
	socket->close();
	delete socket;
}

std::ostream& operator <<(std::ostream & out, const BridgeClientST::TimeoutException & ex) {
	out << "BridgeClientST::TimeoutException";
	return out;
}


}
