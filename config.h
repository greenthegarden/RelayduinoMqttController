#ifndef RELAYDUINOMQTTCONTROLLER_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_CONFIG_H_


#include "Debug.h"

// external libraries
#include <MemoryFree.h>
//#include <SwitchedDevice.h>
#include <Relayduino.h>


const byte BUFFER_SIZE            = 32;
char progBuffer[BUFFER_SIZE];
char charBuffer[BUFFER_SIZE];


// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const int BAUD_RATE               = 9600;


#include "ethernet_config.h"
#include "mqtt_config.h"
#include "alarm_config.h"
#include "relay_config.h"


boolean mqttClientConnected = false;


void no_network_behaviour() {
  relays_switch_off();
}


const byte OPTICAL_INPUTS[] = { OPTO_INPUT_1, OPTO_INPUT_2, OPTO_INPUT_3, OPTO_INPUT_4 };
const byte ANALOG_INPUTS[]= { ANALOG_IN_1, ANALOG_IN_2, ANALOG_IN_3 };
unsigned long inputsPreviousMillis = 0UL;
const unsigned long INPUT_READ_INTERVAL = 5UL * 60UL * 1000UL;


void publish_reading(byte inputType, byte idx, byte reading) {
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(INPUT_TOPICS[inputType])));
  charBuffer[0] = '\0';
  sprintf(charBuffer, "%i%c%i", idx, ',', reading);
  mqttClient.publish(progBuffer, charBuffer);
}

void read_inputs() {
  // read analog inputs
  for (byte idx = 0; idx < ARRAY_SIZE(ANALOG_INPUTS); idx++) {
    byte reading = analogRead(ANALOG_INPUTS[idx]);
    publish_reading(0, idx, reading);
  }
  // read optical (digital) inputs
  for (byte idx = 0; idx < ARRAY_SIZE(OPTICAL_INPUTS); idx++) {
    byte reading = digitalRead(OPTICAL_INPUTS[idx]);
    publish_reading(1, idx, reading);
  }
}


#endif   /* RELAYDUINOMQTTCONTROLLER_CONFIG_H_ */

