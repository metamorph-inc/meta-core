
#ifndef EVENT_LOOP_MONITOR_H
#define EVENT_LOOP_MONITOR_H

#include <boost/thread/thread.hpp>

namespace isis {
class EventLoopMonitor {
public:
	boost::mutex m_mutex;
	log4cpp::Category& m_logcat;

	EventLoopMonitor();
    void CreoEventLoop();

};
}

#endif