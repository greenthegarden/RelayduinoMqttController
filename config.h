#ifndef RELAYDUINOMQTTCONTROLLER_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_CONFIG_H_


#include "Debug.h"

// external libraries
#include <MemoryFree.h>
#include <SwitchedDevice.h>
#include <Relayduino.h>

// Macros
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))


const byte BUFFER_SIZE            = 32;
char prog_buffer[BUFFER_SIZE];
char char_buffer[BUFFER_SIZE];

const int BAUD_RATE               = 9600;


#include "ethernet_config.h"
#include "mqtt_config.h"
#include "time_config.h"
#include "alarm_config.h"
#include "relay_config.h"


#endif   /* RELAYDUINOMQTTCONTROLLER_CONFIG_H_ */

