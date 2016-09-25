#ifndef RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_

// Ethernet libraries
#include <SPI.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 2
#endif

#if ETHERNET_SHIELD_VERSION == 1
#include <Ethernet.h>
#elif ETHERNET_SHIELD_VERSION == 2
#include <Ethernet2.h>
#endif

// Update these with values suitable for your network.
byte mac[] = {0x90, 0xA2, 0xDA,
              0x0F, 0xFC, 0xA9}; // value for Arduino Ethernet Shield 2

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void ethernet_init() {
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for (;;)
      ;
  }
  delay(NETWORK_STARTUP_DELAY);

  DEBUG_LOG(1, "IP:");
  DEBUG_LOG(1, Ethernet.localIP());
}

#endif /* RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
