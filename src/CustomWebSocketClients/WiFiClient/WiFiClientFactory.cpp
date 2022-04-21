#pragma once

#include <Arduino.h>

#include "WiFiClientFactory.h"

#include "NetworkInitializeException.h"

WiFiClientFactory::~WiFiClientFactory(void) {
}

WiFiClient * WiFiClientFactory::produceClient(void) {
    WiFiClient * wifiClient = new WiFiClient();

    uint8_t MAX_TRIES = 1;
    for(uint8_t i = 0; i < MAX_TRIES; i++) {
        if(initWiFi()) {
            return wifiClient;
        } else {
            Serial.println("[Wifidfd.h] init() failed");
            Serial.print("[Wifidfd.h] Retries: ");
            Serial.print(i);
            Serial.print(" out of ");
            Serial.println(MAX_TRIES);
        }
    }
    throw NetworkInitializeException();
    return wifiClient;
}

bool  WiFiClientFactory::initWiFi(void) {
    return true;
}
