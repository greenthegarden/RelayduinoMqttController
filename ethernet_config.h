#ifndef RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_


// Ethernet libraries
#include <SPI.h>

// If you want to use the w5100
#include <Ethernet.h>

// If you want to use the w5200/w5500
//#include <EthernetV2_0.h>

// Update these with values suitable for your network.
byte mac[]    = { 0x90, 0xA2, 0xDA, 0x0F, 0xFC, 0xA9 }; // value for Arduino Ethernet Shield 2

#define USE_DHCP false

#if !USE_DHCP
IPAddress ip(192, 168, 2, 90);
IPAddress myIp(192, 168, 2, 90);
IPAddress myDns(192, 168, 2, 1);
IPAddress myGateway(192, 168, 2, 1);
IPAddress mySubnet(255, 255, 255, 0);
#endif

EthernetClient ethernetClient;


#endif   /* RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_ */

