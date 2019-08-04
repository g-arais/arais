#ifndef _DEBUG_H
#define _DEBUG_H

#include <Arduino.h>
#include "general.h"

void debugSetup();

#if DEBUG_SUPPORT
    #define DEBUG_MSG(...) debugSend(__VA_ARGS__)
    #define DEBUG_MSG_P(...) debugSend_P(__VA_ARGS__)
#endif

#ifndef DEBUG_MSG
    #define DEBUG_MSG(...)
    #define DEBUG_MSG_P(...)
#endif

#endif