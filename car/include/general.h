#ifndef _GENERAL_H
#define _GENERAL_H

#include "globals.h"

//------------------------------------------------------------------------------
// GENERAL
//------------------------------------------------------------------------------

#define DEVICE_NAME             MANUFACTURER "_" DEVICE     // Concatenate both to get a unique device name
#define ADMIN_PASS              "fibonacci"                 // Default password (WEB, OTA, WIFI)
#define USE_PASSWORD            1                           // Insecurity caution! Disabling this will disable password querying completely.
#define LOOP_DELAY_TIME         10                          // Delay for this millis in the main loop [0-250]
#define LOADAVG_INTERVAL        30000                       // Interval between calculating load average (in ms)

//------------------------------------------------------------------------------
// DEBUG
//------------------------------------------------------------------------------

#define DEBUG_SUPPORT           1
#define DEBUG_ADD_TIMESTAMP     1
#define DEBUG_PORT              Serial          // Default debugging port
#define SERIAL_BAUDRATE         115200          // Default baudrate

//------------------------------------------------------------------------------
// EEPROM
//------------------------------------------------------------------------------

#define EEPROM_SIZE             4096            // EEPROM size in bytes
#define EEPROM_RELAY_STATUS     0               // Address for the relay status (1 byte)
#define EEPROM_ENERGY_COUNT     1               // Address for the energy counter (4 bytes)
#define EEPROM_CUSTOM_RESET     5               // Address for the reset reason (1 byte)
#define EEPROM_CRASH_COUNTER    6               // Address for the crash counter (1 byte)
#define EEPROM_MESSAGE_ID       7               // Address for the MQTT message id (4 bytes)
#define EEPROM_DATA_END         11              // End of custom EEPROM data block

//------------------------------------------------------------------------------
// HEARTBEAT
//------------------------------------------------------------------------------

#define HEARTBEAT_ENABLED       0

// -----------------------------------------------------------------------------
// SPIFFS
// -----------------------------------------------------------------------------

#define SPIFFS_SUPPORT          0               // Do not add support for SPIFFS by default

#endif