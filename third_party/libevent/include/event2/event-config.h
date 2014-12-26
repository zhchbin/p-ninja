#ifndef  EVENT_CONFIG_H_
#define  EVENT_CONFIG_H_

#if defined(WIN32)
#include "event-config_win.h"
#elif defined(__linux__)
#include "event-config_linux.h"
#else
#error generate event-config.h for your platform
#endif

#endif  // EVENT_CONFIG_H_