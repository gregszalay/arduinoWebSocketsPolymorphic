#pragma once

#include <Arduino.h>

#ifndef TINY_GSM_MODEM_SIM7600
#define TINY_GSM_MODEM_SIM7600
#endif

#ifndef GSM_SERIAL
#define GSM_SERIAL Serial1
#endif

#ifndef SERIAL_MON
#define SERIAL_MON Serial0
#endif

// Your GPRS credentials, if any
const char apn[] = "internet"; //  Telekom: internet , Yettel: online
// https://wiki.droam.com/wiki/index.php/Hungary#Magyar_Telekom_Plc_.28T-Mobile_Hungary.29
const char gprsUser[] = "";
const char gprsPass[] = "";
const char gsm_sim[] = "1234";

#include "TinyGsmClientFactory.h"

#include "NetworkInitializeException.h"

TinyGsmClientFactory::~TinyGsmClientFactory() {
}

TinyGsmSim7600::GsmClientSim7600 * TinyGsmClientFactory::produceClient() {
    TinyGsmSim7600 * modem                    = new TinyGsmSim7600(GSM_SERIAL);
    TinyGsmSim7600::GsmClientSim7600 * client = new TinyGsmSim7600::GsmClientSim7600(*modem);

    modemInit(modem);
    uint8_t MAX_TRIES = 1;
    for(uint8_t i = 0; i < MAX_TRIES; i++) {
        if(connectGPRS(modem))
            return client;
        Serial.print("[TCPClientFactory.h][produceGSMClient] connectGPRS() failed");
        Serial.print("[TCPClientFactory.h][produceGSMClient] Retries: ");
        Serial.print(i);
        Serial.print(" out of ");
        Serial.println(MAX_TRIES);
    }
    Serial.println("[TCPClientFactory.h][produceGSMClient] throwing NetworkInitializeException");
    throw NetworkInitializeException();
    return client;
}

void TinyGsmClientFactory::modemInit(TinyGsmSim7600 * modem) {
    Serial.println("[TCPClientFactory.h][produceGSMClient] Beginning serial communication with GSM modem.");
    /* Set GSM module baud rate.
     *
     * E.g.:
     *      SerialAT.begin(9600);
     *
     * Or for experimenting:
     *      TinyGsmAutoBaud(SerialAT, 9600, 115200);
     *
     * For ESP32 some additional parameters have to be set: */
    GSM_SERIAL.end();
    delay(2000);
    GSM_SERIAL.begin(115200, SERIAL_8N1, 16, 17, false);
    delay(6000);

    Serial.println("[TCPClientFactory.h][produceGSMClient] (Re)Starting GSM modem...");
    /*
     * Restart takes quite some time; To skip it, call init() instead of restart()
     */
    // this->at->restart();
    modem->init();

    Serial.println("[TCPClientFactory.h][produceGSMClient] Waiting for GSM modem info...");
    while(!modem->getModemInfo())
        ;
    Serial.println("[TCPClientFactory.h][produceGSMClient] Modem Info: ");
    Serial.println(modem->getModemInfo());

    Serial.println("[TCPClientFactory.h][produceGSMClient]  this->_modem->setNetworkMode(2); ");
    // 2 Automatic
    // 13 GSM only
    // 38 LTE only
    // 51 GSM and LTE only
    modem->setNetworkMode(2);

/*
    // Unlock your SIM card with a PIN if needed/* 
    if(!strcmp(*(this->configPointers->gsmPIN), "") && modem->getSimStatus() != 3) {
        Serial.println("[TCPClientFactory.h][produceGSMClient] Unlocking SIM card...");
        Serial.println("[TCPClientFactory.h][produceGSMClient] SIM pin: ");
        Serial.println(*(this->configPointers->gsmPIN));

        modem->simUnlock(*(this->configPointers->gsmPIN));
    } 
    */
    if(!strcmp(gsm_sim, "") && modem->getSimStatus() != 3) {
        Serial.println("[TCPClientFactory.h][produceGSMClient] Unlocking SIM card...");
        Serial.println("[TCPClientFactory.h][produceGSMClient] SIM pin: ");
        Serial.println(gsm_sim);

        modem->simUnlock(gsm_sim);
    } 
}

bool TinyGsmClientFactory::connectGPRS(TinyGsmSim7600 * modem, boolean serverMode) {
    Serial.println("[TCPClientFactory.h][produceGSMClient] Waiting for network attachment...");
    if(modem->waitForNetwork())
        Serial.println("[TCPClientFactory.h][produceGSMClient] Network attached successfully!");

    if(modem->isNetworkConnected())
        Serial.println("[TCPClientFactory.h][produceGSMClient] Network connected!");

    // GPRS connection parameters are usually set after network registration
    Serial.print("[TCPClientFactory.h][produceGSMClient] Connecting to apn: ");
    Serial.println(apn);

    uint8_t MAX_TRIES = 1;
    for(uint8_t i = 0; i < MAX_TRIES; i++) {
        if(modem->gprsConnect(apn, gprsUser, gprsPass)) {
            break;
        }
        Serial.print("[TCPClientFactory.h][produceGSMClient] this->at->gprsConnect(apn, gprsUser, gprsPass) failed");
        Serial.print("[TCPClientFactory.h][produceGSMClient] Retries: ");
        Serial.print(i);
        Serial.print(" out of ");
        Serial.println(MAX_TRIES);
    }

    if(serverMode) {
    /*
            SerialMonitor.println("GPRS connected");
          Put modem in server mode
            modem->setIPserver();
            SerialMonitor.println("success"); // Todo: proper react on return values
            SerialMonitor.print("Local IP: ");
            SerialMonitor.println(modem->localIP());
        */}

    if(modem->isGprsConnected()) {
        Serial.println("[TCPClientFactory.h][produceGSMClient] GPRS data connected!");
        return true;
    } else {
        return false;
    }
};

void TinyGsmClientFactory::disconnectGPRS(TinyGsmSim7600 * modem) {
    modem->gprsDisconnect();
    Serial.println("[TCPClientFactory.h][produceGSMClient] GPRS disconnected");
}
