#pragma once

#include <Arduino.h>

#include "EthernetClientFactory.h"

#include "Ethernet.h"

#include "NetworkInitializeException.h"

EthernetClientFactory::~EthernetClientFactory() {
}

EthernetClient * EthernetClientFactory::produceClient() {
    EthernetClient * ethClient = new EthernetClient();

    uint8_t MAX_TRIES = 1;
    for(uint8_t i = 0; i < MAX_TRIES; i++) {
        if(initEthernet()) {
            return ethClient;
        } else {
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

bool EthernetClientFactory::initEthernet(void) {
    // Enter a MAC address for your controller below.
    // Newer Ethernet shields have a MAC address printed on a sticker on the shield
    byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

    // Set the static IP address to use if the DHCP fails to assign
    IPAddress ip(192, 168, 0, 177);
    IPAddress myDns(192, 168, 0, 1);

    // Initialize the Ethernet client library
    // with the IP address and port of the server
    // that you want to connect to (port 80 is default for HTTP):
    EthernetClient client;
    // You can use Ethernet.init(pin) to configure the CS pin
    // Ethernet.init(10);  // Most Arduino shields
    Ethernet.init(5);    // MKR ETH shield
    // Ethernet.init(0);   // Teensy 2.0
    // Ethernet.init(20);  // Teensy++ 2.0
    // Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
    // Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet

    // Open serial communications and wait for port to open:
    Serial.begin(115200);
    while(!Serial) {
        ;    // wait for serial port to connect. Needed for native USB port only
    }

    // start the Ethernet connection:
    Serial.println("Initialize Ethernet with DHCP:");
    if(Ethernet.begin(mac) == 0) {
        Serial.println("Failed to configure Ethernet using DHCP");
        // Check for Ethernet hardware present
        if(Ethernet.hardwareStatus() == EthernetNoHardware) {
            Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
            return false;
        }
        if(Ethernet.linkStatus() == LinkOFF) {
            Serial.println("Ethernet cable is not connected.");
            return false;
        }
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, ip, myDns);
    } else {
        Serial.print("  DHCP assigned IP ");
        Serial.println(Ethernet.localIP());
    }
    return true;
}
