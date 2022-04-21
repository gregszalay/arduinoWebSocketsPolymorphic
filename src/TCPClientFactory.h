#pragma once

#include <Arduino.h>

#ifndef TINY_GSM_MODEM_SIM7600
#define TINY_GSM_MODEM_SIM7600
#endif

#include "NetworkInitializeException.h"

#include <mtconfig.h>

#ifndef GSM_SERIAL
#define GSM_SERIAL Serial1
#endif

class TCPClientFactory
{

    ConfigPointers *configPointers = new ConfigPointers();

public:
    TCPClientFactory() {}

    TinyGsmSim7600::GsmClientSim7600 *produceGSMClient()
    {
        TinyGsmSim7600 *modem = new TinyGsmSim7600(GSM_SERIAL);
        TinyGsmSim7600::GsmClientSim7600 *client = new TinyGsmSim7600::GsmClientSim7600(*modem);

        modemInit(modem);
        uint8_t MAX_TRIES = 1;
        for (uint8_t i = 0; i < MAX_TRIES; i++)
        {
            if (connectGPRS(modem))
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

    EthernetClient *produceEthernetClient()
    {

        EthernetClient *ethClient = new EthernetClient();

        uint8_t MAX_TRIES = 1;
        for (uint8_t i = 0; i < MAX_TRIES; i++)
        {
            if (initEthernet())
            {
                return ethClient;
            }
            else
            {
                Serial.println("[Ethernet.h] init() failed");
                Serial.print("[Ethernet.h] Retries: ");
                Serial.print(i);
                Serial.print(" out of ");
                Serial.println(MAX_TRIES);
            }
        }
        throw NetworkInitializeException();
        return ethClient;
    }

    WiFiClient *produceWiFiClient()
    {

        WiFiClient *wifiClient = new WiFiClient();

        uint8_t MAX_TRIES = 1;
        for (uint8_t i = 0; i < MAX_TRIES; i++)
        {
            if (initWiFi())
            {
                return wifiClient;
            }
            else
            {
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
private:
    void modemInit(TinyGsmSim7600 *modem)
    {
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
        while (!modem->getModemInfo())
            ;
        Serial.println("[TCPClientFactory.h][produceGSMClient] Modem Info: ");
        Serial.println(modem->getModemInfo());

        Serial.println("[TCPClientFactory.h][produceGSMClient]  this->_modem->setNetworkMode(2); ");
        // 2 Automatic
        // 13 GSM only
        // 38 LTE only
        // 51 GSM and LTE only
        modem->setNetworkMode(2);

        // Unlock your SIM card with a PIN if needed
        if (!strcmp(*(this->configPointers->gsmPIN), "") && modem->getSimStatus() != 3)
        {
            Serial.println("[TCPClientFactory.h][produceGSMClient] Unlocking SIM card...");
            Serial.println("[TCPClientFactory.h][produceGSMClient] SIM pin: ");
            Serial.println(*(this->configPointers->gsmPIN));

            modem->simUnlock(*(this->configPointers->gsmPIN));
        }
    }

    bool connectGPRS(TinyGsmSim7600 *modem, boolean serverMode = false)
    {

        Serial.println("[TCPClientFactory.h][produceGSMClient] Waiting for network attachment...");
        if (modem->waitForNetwork())
            Serial.println("[TCPClientFactory.h][produceGSMClient] Network attached successfully!");

        if (modem->isNetworkConnected())
            Serial.println("[TCPClientFactory.h][produceGSMClient] Network connected!");

        // GPRS connection parameters are usually set after network registration
        Serial.print("[TCPClientFactory.h][produceGSMClient] Connecting to apn: ");
        Serial.println(apn);

        uint8_t MAX_TRIES = 1;
        for (uint8_t i = 0; i < MAX_TRIES; i++)
        {
            if (modem->gprsConnect(apn, gprsUser, gprsPass))
            {
                break;
            }
            Serial.print("[TCPClientFactory.h][produceGSMClient] this->at->gprsConnect(apn, gprsUser, gprsPass) failed");
            Serial.print("[TCPClientFactory.h][produceGSMClient] Retries: ");
            Serial.print(i);
            Serial.print(" out of ");
            Serial.println(MAX_TRIES);
        }

        if (serverMode)
        {
    /*
            SerialMonitor.println("GPRS connected");
          Put modem in server mode
            modem->setIPserver();
            SerialMonitor.println("success"); // Todo: proper react on return values
            SerialMonitor.print("Local IP: ");
            SerialMonitor.println(modem->localIP());
        */}

    if (modem->isGprsConnected())
    {
        Serial.println("[TCPClientFactory.h][produceGSMClient] GPRS data connected!");
        return true;
    }
    else
    {
        return false;
    }
    };

    void disconnectGPRS(TinyGsmSim7600 *modem)
    {
        modem->gprsDisconnect();
        Serial.println("[TCPClientFactory.h][produceGSMClient] GPRS disconnected");
    }


    bool initEthernet()
    {
        // Enter a MAC address for your controller below.
        // Newer Ethernet shields have a MAC address printed on a sticker on the shield
        byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

        // Set the static IP address to use if the DHCP fails to assign
        IPAddress ip(192, 168, 0, 177);
        IPAddress myDns(192, 168, 0, 1);

        // Initialize the Ethernet client library
        // with the IP address and port of the server
        // that you want to connect to (port 80 is default for HTTP):
        EthernetClient client;
        // You can use Ethernet.init(pin) to configure the CS pin
        // Ethernet.init(10);  // Most Arduino shields
        Ethernet.init(5); // MKR ETH shield
        // Ethernet.init(0);   // Teensy 2.0
        // Ethernet.init(20);  // Teensy++ 2.0
        // Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
        // Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

        // Open serial communications and wait for port to open:
        Serial.begin(115200);
        while (!Serial)
        {
            ; // wait for serial port to connect. Needed for native USB port only
        }

        // start the Ethernet connection:
        Serial.println("Initialize Ethernet with DHCP:");
        if (Ethernet.begin(mac) == 0)
        {
            Serial.println("Failed to configure Ethernet using DHCP");
            // Check for Ethernet hardware present
            if (Ethernet.hardwareStatus() == EthernetNoHardware)
            {
                Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
                return false;
            }
            if (Ethernet.linkStatus() == LinkOFF)
            {
                Serial.println("Ethernet cable is not connected.");
                return false;
            }
            // try to congifure using IP address instead of DHCP:
            Ethernet.begin(mac, ip, myDns);
        }
        else
        {
            Serial.print("  DHCP assigned IP ");
            Serial.println(Ethernet.localIP());
        }
        return true;
    }

    bool initWiFi()
    {
        return true;
    }
};