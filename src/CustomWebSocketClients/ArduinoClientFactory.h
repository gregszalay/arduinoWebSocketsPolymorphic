#pragma once

#include <Arduino.h>
#include <WebSockets.h>
#include "NetworkInitializeException.h"

class ArduinoClientFactory {
  public:
     ArduinoClientFactory(void);

    virtual ~ArduinoClientFactory(void);

    // virtual Client * constructConcreteClientObject(void);

    virtual Client * produceClient(void);

    bool initClient(Client * clientObj);
};