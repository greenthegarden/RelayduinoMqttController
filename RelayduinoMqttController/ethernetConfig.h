#ifndef RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_

// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>

#ifndef ETHERNET_SHIELD_VERSION
#define ETHERNET_SHIELD_VERSION 1
#endif

#if ETHERNET_SHIELD_VERSION == 1
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };
#elif ETHERNET_SHIELD_VERSION == 2
byte mac[] = { 0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9 };
#endif

EthernetClient ethernetClient;

const unsigned long NETWORK_STARTUP_DELAY = 1500UL;

void ethernet_init()
{
  DEBUG_LOG(1, "Configuring Ethernet")
  // Configure Ethernet
  if (Ethernet.begin(mac) == 0) {
    DEBUG_LOG(1, "Failed to configure Ethernet using DHCP");
    // try to configure using IP address instead of DHCP:
    IPAddress ip(192, 168, 1, 250);
    Ethernet.begin(mac, ip);
  }
  delay(NETWORK_STARTUP_DELAY);

  DEBUG_LOG(1, "IP:");
  DEBUG_LOG(1, Ethernet.localIP());
}

#endif /* RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_ */
