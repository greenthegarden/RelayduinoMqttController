#ifndef RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_


// Ethernet libraries
#include <SPI.h>
#include <Ethernet.h>


// Update these with values suitable for your network.
byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 90);

EthernetClient ethernetClient;


#endif   /* RELAYDUINOMQTTCONTROLLER_ETHERNET_CONFIG_H_ */

