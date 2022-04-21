#pragma once

#include <Arduino.h>

#include "CustomWebSocketClients/ArduinoClientFactory.h"

#include "EthernetClient.h"

#include "NetworkInitializeException.h"

class EthernetClientFactory : public ArduinoClientFactory {
  public:
    EthernetClientFactory(void) {}

    virtual ~EthernetClientFactory(void);

   // Client * constructConcreteClientObject(void);

    EthernetClient * produceClient(void);

    bool initClient(Client * clientObj);

  private:
    bool initEthernet(void);
};