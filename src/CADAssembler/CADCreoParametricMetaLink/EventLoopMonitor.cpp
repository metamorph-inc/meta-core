#include "stdafx.h"
#include "EventLoopMonitor.h"
#include <ProCore.h>

namespace isis {
EventLoopMonitor::EventLoopMonitor() :
    m_logcat( ::log4cpp::Category::getInstance(std::string("metalink.assembly.monitor")))
{ }

/**
*/

void EventLoopMonitor::CreoEventLoop() {
    ProEventProcess();
}


} // namespace isis