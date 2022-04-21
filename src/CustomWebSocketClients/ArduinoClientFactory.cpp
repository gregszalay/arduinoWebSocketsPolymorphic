
#pragma once

#include <Arduino.h>
#include "ArduinoClientFactory.h"

#include "NetworkInitializeException.h"

ArduinoClientFactory::~ArduinoClientFactory() {
    //TODO: disconnect();
}

/*
Client * ArduinoClientFactory::produceClient() {
    Client * clientObj = this->constructConcreteClientObject();

    uint8_t MAX_TRIES = 1;
    for(uint8_t i = 0; i < MAX_TRIES; i++) {
        if(initClient(clientObj)) {
            return clientObj;
        } else {
            Serial.println("[ArduinoClientFactory] initClient() failed");
            Serial.print("[ArduinoClientFactory] Retries: ");
            Serial.print(i);
            Serial.print(" out of ");
            Serial.println(MAX_TRIES);
        }
    }
    throw NetworkInitializeException();
    return clientObj;
}
*/