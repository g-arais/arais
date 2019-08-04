/*
*
* SYSTEM MODULE
*
* Copyright (C) 2019 by Gustavo Arais
*
*/

#include <Arduino.h>
#include <EEPROM.h>
#include "general.h"

// -----------------------------------------------------------------------------

unsigned long _loop_delay = 0;
bool _system_send_heartbeat = false;

// Calculated load average 0 to 100;
unsigned short int _load_average = 100;

void systemSendHeartbeat() 
{
    _system_send_heartbeat = true;
}

unsigned long systemLoopDelay() 
{
    return _loop_delay;
}

unsigned long systemLoadAverage() 
{
    return _load_average;
}

void systemLoop() 
{
    // -------------------------------------------------------------------------
    // Heartbeat
    // -------------------------------------------------------------------------

    #if HEARTBEAT_ENABLED
        // Heartbeat
        static unsigned long last_hbeat = 0;
        if (_system_send_heartbeat || (last_hbeat == 0) || (millis() - last_hbeat > HEARTBEAT_INTERVAL)) {
            _system_send_heartbeat = false;
            last_hbeat = millis();
            heartbeat();
        }
    #endif // HEARTBEAT_ENABLED

    // -------------------------------------------------------------------------
    // Load Average calculation
    // -------------------------------------------------------------------------

    static unsigned long last_loadcheck = 0;
    static unsigned long load_counter_temp = 0;
    load_counter_temp++;

    if (millis() - last_loadcheck > LOADAVG_INTERVAL) {

        static unsigned long load_counter = 0;
        static unsigned long load_counter_max = 1;

        load_counter = load_counter_temp;
        load_counter_temp = 0;
        if (load_counter > load_counter_max) {
            load_counter_max = load_counter;
        }
        _load_average = 100 - (100 * load_counter / load_counter_max);
        last_loadcheck = millis();
    }

    // -------------------------------------------------------------------------
    // Power saving delay
    // -------------------------------------------------------------------------

    delay(_loop_delay);
}

void systemSetup() 
{

    EEPROM.begin(EEPROM_SIZE);

    #if SPIFFS_SUPPORT
        SPIFFS.begin();
    #endif

    _loop_delay = constrain(LOOP_DELAY_TIME, 0, 250);

    // Register Loop
    registerLoop(systemLoop);
}
