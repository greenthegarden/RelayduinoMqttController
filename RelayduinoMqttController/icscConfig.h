#ifndef RELAYDUINOMQTTCONTROLLER_ICSC_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ICSC_CONFIG_H_

#include <ICSC.h>

// on relayduino
// D0 RS485 RO      Receiver out
// D1 RS485 DI      Driver in (the transmitter pin)
// A5 RS485 RE/DE   Receiver Enable pin (pulled low to receive data)/Driver Enable pin (pulled high while transmitting data)

const byte RS485_RO_PIN = 0; // D0
const byte RS485_DI_PIN = 1; // D1
const byte RS485_DE_PIN = 19; // D19/A5

const char station = 3;

#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif

ICSC icsc(Serial, station, RS485_DE_PIN);

#endif /* RELAYDUINOMQTTCONTROLLER_ICSC_CONFIG_H_ */
