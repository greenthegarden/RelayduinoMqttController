#ifndef RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_

// Ethernet libraries
#include <SPI.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 2
#endif

#if ETHERNET_SHIELD_VERSION == 1
#include <Ethernet.h>
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#elif ETHERNET_SHIELD_VERSION == 2
#include <Ethernet2.h>
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9 };
#endif

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void ethernet_init() {
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    IPAddress ip(192, 168, 1, 29);
    Ethernet.begin(mac, ip);
  }
  delay(NETWORK_STARTUP_DELAY);

  DEBUG_LOG(1, "IP:");
  DEBUG_LOG(1, Ethernet.localIP());
}

#endif /* RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
