/*
* DEBUG MODULE
*
* Copyright (C) 2019 by Gustavo Arais
*
*/

#include "debug.h"

#if DEBUG_SUPPORT

#include <stdio.h>
#include <stdarg.h>
#include <EEPROM.h>

extern "C" {
    #include "user_interface.h"
}

void _debugSend(char * message) 
{
    #if DEBUG_ADD_TIMESTAMP
        static bool add_timestamp = true;
        char timestamp[10] = {0};
        if (add_timestamp) snprintf_P(timestamp, sizeof(timestamp), PSTR("[%06lu] "), millis() % 1000000);
        add_timestamp = (message[strlen(message)-1] == 10) || (message[strlen(message)-1] == 13);
    #endif

    #if DEBUG_ADD_TIMESTAMP
        DEBUG_PORT.printf(timestamp);
    #endif
    DEBUG_PORT.printf(message);

    #if DEBUG_WEB_SUPPORT
        if (wsConnected() && (getFreeHeap() > 10000)) {
            String m = String(message);
            m.replace("\"", "&quot;");
            m.replace("{", "&#123");
            m.replace("}", "&#125");
            char buffer[m.length() + 24];
            #if DEBUG_ADD_TIMESTAMP
                snprintf_P(buffer, sizeof(buffer), PSTR("{\"weblog\": \"%s%s\"}"), timestamp, m.c_str());
            #else
                snprintf_P(buffer, sizeof(buffer), PSTR("{\"weblog\": \"%s\"}"), m.c_str());
            #endif
            wsSend(buffer);
        }
    #endif
}

void debugSend(const char * format, ...) 
{
    va_list args;
    va_start(args, format);
    char test[1];
    int len = ets_vsnprintf(test, 1, format, args) + 1;
    char * buffer = new char[len];
    ets_vsnprintf(buffer, len, format, args);
    va_end(args);

    _debugSend(buffer);

    delete[] buffer;
}

void debugSend_P(PGM_P format_P, ...) 
{
    char format[strlen_P(format_P)+1];
    memcpy_P(format, format_P, sizeof(format));

    va_list args;
    va_start(args, format_P);
    char test[1];
    int len = ets_vsnprintf(test, 1, format, args) + 1;
    char * buffer = new char[len];
    ets_vsnprintf(buffer, len, format, args);
    va_end(args);

    _debugSend(buffer);

    delete[] buffer;
}

#if DEBUG_WEB_SUPPORT

void debugWebSetup() 
{
    wsOnSendRegister([](JsonObject& root) {
        root["dbgVisible"] = 1;
    });

    wsOnActionRegister([](uint32_t client_id, const char * action, JsonObject& data) {
        if (strcmp(action, "dbgcmd") == 0) {
            const char* command = data.get<const char*>("command");
            char buffer[strlen(command) + 2];
            snprintf(buffer, sizeof(buffer), "%s\n", command);
            settingsInject((void*) buffer, strlen(buffer));
        }
    });
}

#endif

void debugSetup() 
{
    DEBUG_PORT.begin(SERIAL_BAUDRATE);
    #if DEBUG_ESP_WIFI
        DEBUG_PORT.setDebugOutput(true);
    #endif
}

#endif // DEBUG_SUPPORT
