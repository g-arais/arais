#include "main.h"
#include <vector>

std::vector<void (*)()> _loop_callbacks;

void registerLoop(void (*callback)()) 
{
    _loop_callbacks.push_back(callback);
}



byte remoteDevice[6] = {0x30, 0xAE, 0xA4, 0x74, 0x20, 0x9D};

#define CHANNEL 1

void printMacAddress(uint8_t* macaddr) {
    Serial.print("{");
    for (int i = 0; i < 6; i++) {
        Serial.print("0x");
        Serial.print(macaddr[i], HEX);
        if (i < 5) Serial.print(',');
    }
    Serial.println("};");
}

// Init ESP Now with fallback
void InitESPNow() {
    if (esp_now_init() == 0) {
        Serial.println("ESPNow Init Success");
        esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    } else {
        Serial.println("ESPNow Init Failed");
    }
}

uint8_t data = 0;
// send data
void sendData() {
    data++;
    int result = esp_now_send(remoteDevice, &data, sizeof(data));
    Serial.print("Send Status: ");
    if (result ==0) {
        Serial.println("Success " + String(result));
    } else {
        Serial.println("Failed " + String(result));
    }
}

void setup() {

    // Serial debug
    #if DEBUG_SUPPORT
        debugSetup();
    #endif

    // Init EEPROM, SPIFFS and system stuff
    systemSetup();

    pinMode(2, OUTPUT);
    digitalWrite(2, HIGH);

    InitESPNow();

    esp_now_register_send_cb([](uint8_t* macaddr, uint8_t status) {
        printMacAddress(macaddr);
        static uint16_t ok = 0;
        static uint16_t fail = 0;
        if (status == 0) {
            Serial.println("ESPNOW: SEND_OK");
            ok++;
        } else {
            Serial.println("ESPNOW: SEND_FAILED");
            fail++;
        }

        Serial.printf("[SUCCESS] = %d/%d \r\n", ok, ok + fail);
    });

        esp_now_register_recv_cb([](uint8_t *mac, uint8_t *data, uint8_t len) {
        Serial.print("Recebido: "); 
        Serial.println(*data);
        digitalWrite(2, !digitalRead(2));
    });
}

void loop() {

    sendData();

    delay(1000);
}