#pragma once

#include <Arduino.h>

#include "CustomWebSocketClients/ArduinoClientFactory.h"

#include "NetworkInitializeException.h"

#ifndef TINY_GSM_MODEM_SIM7600
#define TINY_GSM_MODEM_SIM7600
#endif
#include "TinyGsmClient.h"
#include "TinyGsmClientSIM7600.h"

class TinyGsmClientFactory : public ArduinoClientFactory {
  public:
    TinyGsmClientFactory(void) {}

    virtual ~TinyGsmClientFactory(void);

 //   Client * constructConcreteClientObject(void);

    TinyGsmSim7600::GsmClientSim7600 * produceClient(void);

    bool initClient(Client * clientObj);

  private:
    void modemInit(TinyGsmSim7600 * modem);
    bool connectGPRS(TinyGsmSim7600 * modem, boolean serverMode = false);
    void disconnectGPRS(TinyGsmSim7600 * modem);
};