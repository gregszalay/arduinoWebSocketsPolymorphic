#pragma once

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "CustomWebSocketClients/ArduinoClientFactory.h"

#include "NetworkInitializeException.h"

class WiFiClientFactory : public ArduinoClientFactory {
  public:
    WiFiClientFactory(void) {}

    virtual ~WiFiClientFactory(void);

  //  Client * constructConcreteClientObject(void);

    WiFiClient * produceClient(void);

    bool initClient(Client * clientObj);

  private:
    bool initWiFi(void);
};