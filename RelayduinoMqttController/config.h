#ifndef RELAYDUINOMQTTCONTROLLER_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_CONFIG_H_


#include "Debug.h"

// external libraries
#include <MemoryFree.h>
//#include <SwitchedDevice.h>
#include <Relayduino.h>


const byte BUFFER_SIZE            = 32;
char topicBuffer[BUFFER_SIZE];
char payloadBuffer[BUFFER_SIZE];


// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


#if DEBUG_LEVEL > 0
const int BAUD_RATE               = 9600;
#endif

#include "ethernet_config.h"
#include "mqtt_config.h"
#include "alarm_config.h"
#include "relay_config.h"


void no_network_behaviour() {
  relays_switch_off();
}

#define USE_INPUTS false

#if USE_INPUTS
const byte OPTICAL_INPUTS[] = { OPTO_INPUT_1, OPTO_INPUT_2, OPTO_INPUT_3, OPTO_INPUT_4 };
const byte ANALOG_INPUTS[]= { ANALOG_IN_1, ANALOG_IN_2, ANALOG_IN_3 };
unsigned long inputsPreviousMillis = 0UL;
const unsigned long INPUT_READ_INTERVAL = 5UL * 60UL * 1000UL;


void publish_reading(byte inputType, byte idx, byte reading) {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(INPUT_TOPICS[inputType])));
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%i%c%i", idx, ',', reading);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void read_inputs() {
  // read analog inputs
  for (byte idx = 0; idx < ARRAY_SIZE(ANALOG_INPUTS); idx++) {
    byte reading = analogRead(ANALOG_INPUTS[idx]);
    publish_reading(ANALOG_INPUT_IDX, idx, reading);
  }
  // read optical (digital) inputs
  for (byte idx = 0; idx < ARRAY_SIZE(OPTICAL_INPUTS); idx++) {
    byte reading = digitalRead(OPTICAL_INPUTS[idx]);
    publish_reading(OPTO_INPUT_IDX, idx, reading);
  }
}
#endif  /* USE_INPUTS */


#endif   /* RELAYDUINOMQTTCONTROLLER_CONFIG_H_ */