#include "listener.h"

#include "../common/cmake_config.h"

namespace co
{
#if ENABLE_DEBUGGER
Listener::TaskListener*& Listener::GetTaskListener() {
        static TaskListener* task_listener = nullptr;
        return task_listener;
    }
#endif
} // namespace co
