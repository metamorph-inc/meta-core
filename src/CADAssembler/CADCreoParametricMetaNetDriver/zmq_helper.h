#pragma once

#include <string>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "zmq.hpp"

//  Convert C++ STL string to 0MQ frame 
static zmq::message_t to_zmessage(const std::string str) {
	zmq::message_t result(str.size());
	memcpy ((void *) result.data(), str.c_str(), str.size());
	return result;
}

//  Convert message into C++ STL string 
static std::string to_string(const zmq::message_t& msg) {
	std::string result(static_cast<const char*>(msg.data()), msg.size());
	return result;
}

//  Convert message into boost path 
static ::boost::filesystem::path to_file_path(const zmq::message_t& msg) {
	std::string path(static_cast<const char*>(msg.data()), msg.size());
	::boost::filesystem::path result(path);
	return result;
}

//  Convert message into double
static double to_double(const zmq::message_t& msg) {
	std::string number(static_cast<const char*>(msg.data()), msg.size());
    double result = ::boost::lexical_cast<double>(number);
    // int resultI = ::boost::lexical_cast<int>(number);
	return result;
}

//  Convert message into integer
static int to_int(const zmq::message_t& msg) {
	std::string number(static_cast<const char*>(msg.data()), msg.size());
    int result = ::boost::lexical_cast<int>(number);
	return result;
}
