#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>

// MQTT parameters
byte mqtt_server_addr[]           = { 192, 168, 1, 55 };  // Pi eth0 interfaceß
char mqtt_client_id[]             = "relayduino";
const int MQTT_PORT               = 1883;
#define MQTT_MAX_PACKET_SIZE        168
#define MQTT_KEEPALIVE              300

long lastReconnectAttempt         = 0;
const char COMMAND_SEPARATOR      = ':';

char message[BUFFER_SIZE];


// Status topics

const char CONNECTED_STATUS[]  PROGMEM = "relayduino/status/connected";
const char IP_ADDR_STATUS[]    PROGMEM = "relayduino/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayduino/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayduino/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayduino/status/time";
const char RELAY_ON_STATUS[]   PROGMEM = "relayduino/status/relay_on";
const char RELAY_OFF_STATUS[]  PROGMEM = "relayduino/status/relay_off";
const char ALARM_STATUS[]      PROGMEM = "relayduino/status/alarm";
const char ALARMS_STATUS[]     PROGMEM = "relayduino/status/alarms";
const char DURATIONS_STATUS[]  PROGMEM = "relayduino/status/durations";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS, // idx = 0
                                           IP_ADDR_STATUS,   // idx = 1
                                           UPTIME_STATUS,    // idx = 2
                                           MEMORY_STATUS,    // idx = 3
                                           TIME_STATUS,      // idx = 4
                                          };

// Relayduino Input topics

const char ANALOG_IN_1_INPUT[]       PROGMEM = "relayduino/input/analog_in_1";
const char ANALOG_IN_2_INPUT[]       PROGMEM = "relayduino/input/analog_in_2";
const char ANALOG_IN_3_INPUT[]       PROGMEM = "relayduino/input/analog_in_3";

const char OPTO_INPUT_1_INPUT[]      PROGMEM = "relayduino/input/opto_input_1";
const char OPTO_INPUT_2_INPUT[]      PROGMEM = "relayduino/input/opto_input_2";
const char OPTO_INPUT_3_INPUT[]      PROGMEM = "relayduino/input/opto_input_3";
const char OPTO_INPUT_4_INPUT[]      PROGMEM = "relayduino/input/opto_input_4";

PGM_P const INPUT_TOPICS[]     PROGMEM = { ANALOG_IN_1_INPUT,     // idx = 0
                                           ANALOG_IN_2_INPUT,     // idx = 1
                                           ANALOG_IN_3_INPUT,     // idx = 2
                                           OPTO_INPUT_1_INPUT,    // idx = 3
                                           OPTO_INPUT_2_INPUT,    // idx = 4
                                           OPTO_INPUT_3_INPUT,    // idx = 5
                                           OPTO_INPUT_4_INPUT,    // idx = 6
                                          };

// Request topics

const char TIME_REQUEST[]      PROGMEM = "relayduino/request/time";
const char STATE_REQUEST[]     PROGMEM = "relayduino/request/relay_state";
const char DURATION_REQUEST[]  PROGMEM = "relayduino/request/durations";

PGM_P const REQUEST_TOPICS[]   PROGMEM = {TIME_REQUEST,          // idx = 0
                                         };


// Control topics

const char DST_SET[]           PROGMEM = "all/control/dst";

const char RELAY_1_CONTROL[]   PROGMEM = "relayduino/control/relay_1";
const char RELAY_2_CONTROL[]   PROGMEM = "relayduino/control/relay_2";
const char RELAY_3_CONTROL[]   PROGMEM = "relayduino/control/relay_3";
const char RELAY_4_CONTROL[]   PROGMEM = "relayduino/control/relay_4";
const char RELAY_5_CONTROL[]   PROGMEM = "relayduino/control/relay_5";
const char RELAY_6_CONTROL[]   PROGMEM = "relayduino/control/relay_6";
const char RELAY_7_CONTROL[]   PROGMEM = "relayduino/control/relay_7";
const char RELAY_8_CONTROL[]   PROGMEM = "relayduino/control/relay_8";

const char DURATION_1_CTRL[]   PROGMEM = "relayduino/control/duration_1";
const char DURATION_2_CTRL[]   PROGMEM = "relayduino/control/duration_2";
const char DURATION_3_CTRL[]   PROGMEM = "relayduino/control/duration_3";
const char DURATION_4_CTRL[]   PROGMEM = "relayduino/control/duration_4";
const char DURATION_5_CTRL[]   PROGMEM = "relayduino/control/duration_5";
const char DURATION_6_CTRL[]   PROGMEM = "relayduino/control/duration_6";
const char DURATION_7_CTRL[]   PROGMEM = "relayduino/control/duration_7";
const char DURATION_8_CTRL[]   PROGMEM = "relayduino/control/duration_8";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { DST_SET,             // idx = 0
                                           RELAY_1_CONTROL,     // idx = 1
                                           RELAY_2_CONTROL,     // idx = 2
                                           RELAY_3_CONTROL,     // idx = 3
                                           RELAY_4_CONTROL,     // idx = 4
                                           RELAY_5_CONTROL,     // idx = 5
                                           RELAY_6_CONTROL,     // idx = 6
                                           RELAY_7_CONTROL,     // idx = 7
                                           RELAY_8_CONTROL,     // idx = 8
                                           DURATION_1_CTRL,     // idx = 9
                                           DURATION_2_CTRL,     // idx = 10
                                           DURATION_3_CTRL,     // idx = 11
                                           DURATION_4_CTRL,     // idx = 12
                                           DURATION_5_CTRL,     // idx = 13
                                           DURATION_6_CTRL,     // idx = 14
                                           DURATION_7_CTRL,     // idx = 15
                                           DURATION_8_CTRL,     // idx = 16
                                          };


// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqtt_client(mqtt_server_addr, MQTT_PORT, callback, ethernet_client);

void publish_connected()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqtt_client.publish(prog_buffer, "");
}

void publish_ip_address()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqtt_client.publish(prog_buffer, "192.168.1.90");
}

void publish_uptime()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  char_buffer[0] = '\0';
  ltoa(millis(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);
}

void publish_memory()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[3])));
  char_buffer[0] = '\0';
  itoa(freeMemory(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);
}


#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */

