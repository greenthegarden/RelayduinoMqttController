#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
byte mqttServerAddr[]           = { 192, 168, 1, 55 };  // Pi eth0 interfaceß
char mqttClientId[]             = "relayduino";
const int MQTT_PORT               = 1883;
//#define MQTT_MAX_PACKET_SIZE        168
//#define MQTT_KEEPALIVE              300

long lastReconnectAttempt         = 0;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

const char COMMAND_SEPARATOR      = ',';

char message[BUFFER_SIZE];


// Status topics

const char CONNECTED_STATUS[]  PROGMEM = "relayduino/status/connected";
const char IP_ADDR_STATUS[]    PROGMEM = "relayduino/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayduino/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayduino/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayduino/status/time";
const char ALARM_STATUS[]      PROGMEM = "relayduino/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "relayduino/status/relay";
//const char RELAY_1_STATUS[]    PROGMEM = "relayduino/status/relay_1";
//const char RELAY_2_STATUS[]    PROGMEM = "relayduino/status/relay_2";
//const char RELAY_3_STATUS[]    PROGMEM = "relayduino/status/relay_3";
//const char RELAY_4_STATUS[]    PROGMEM = "relayduino/status/relay_4";
//const char RELAY_5_STATUS[]    PROGMEM = "relayduino/status/relay_5";
//const char RELAY_6_STATUS[]    PROGMEM = "relayduino/status/relay_6";
//const char RELAY_7_STATUS[]    PROGMEM = "relayduino/status/relay_7";
//const char RELAY_8_STATUS[]    PROGMEM = "relayduino/status/relay_8";
//const char DURATION_1_STATUS[] PROGMEM = "relayduino/status/duration_1";
//const char DURATION_2_STATUS[] PROGMEM = "relayduino/status/duration_2";
//const char DURATION_3_STATUS[] PROGMEM = "relayduino/status/duration_3";
//const char DURATION_4_STATUS[] PROGMEM = "relayduino/status/duration_4";
//const char DURATION_5_STATUS[] PROGMEM = "relayduino/status/duration_5";
//const char DURATION_6_STATUS[] PROGMEM = "relayduino/status/duration_6";
//const char DURATION_7_STATUS[] PROGMEM = "relayduino/status/duration_7";
//const char DURATION_8_STATUS[] PROGMEM = "relayduino/status/duration_8";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                           IP_ADDR_STATUS,      // idx = 1
                                           UPTIME_STATUS,       // idx = 2
                                           MEMORY_STATUS,       // idx = 3
                                           TIME_STATUS,         // idx = 4
                                           ALARM_STATUS,        // idx = 5
                                           RELAY_STATUS,        // idx = 6
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

PGM_P const REQUEST_TOPICS[]   PROGMEM = {STATE_REQUEST,          // idx = 0
                                         };


// Control topics

const char RELAY_CONTROL[]     PROGMEM = "relayduino/control/relay";

//const char RELAY_1_CONTROL[]   PROGMEM = "relayduino/control/relay_1";
//const char RELAY_2_CONTROL[]   PROGMEM = "relayduino/control/relay_2";
//const char RELAY_3_CONTROL[]   PROGMEM = "relayduino/control/relay_3";
//const char RELAY_4_CONTROL[]   PROGMEM = "relayduino/control/relay_4";
//const char RELAY_5_CONTROL[]   PROGMEM = "relayduino/control/relay_5";
//const char RELAY_6_CONTROL[]   PROGMEM = "relayduino/control/relay_6";
//const char RELAY_7_CONTROL[]   PROGMEM = "relayduino/control/relay_7";
//const char RELAY_8_CONTROL[]   PROGMEM = "relayduino/control/relay_8";
//
//const char DURATION_1_CTRL[]   PROGMEM = "relayduino/control/duration_1";
//const char DURATION_2_CTRL[]   PROGMEM = "relayduino/control/duration_2";
//const char DURATION_3_CTRL[]   PROGMEM = "relayduino/control/duration_3";
//const char DURATION_4_CTRL[]   PROGMEM = "relayduino/control/duration_4";
//const char DURATION_5_CTRL[]   PROGMEM = "relayduino/control/duration_5";
//const char DURATION_6_CTRL[]   PROGMEM = "relayduino/control/duration_6";
//const char DURATION_7_CTRL[]   PROGMEM = "relayduino/control/duration_7";
//const char DURATION_8_CTRL[]   PROGMEM = "relayduino/control/duration_8";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,             // idx = 0
                                          };


// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

void publish_connected()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(progBuffer, "");
}

void publish_ip_address()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(progBuffer, "192.168.1.90");
}

void publish_uptime()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  charBuffer[0] = '\0';
  ltoa(millis(), charBuffer, 10);
  mqttClient.publish(progBuffer, charBuffer);
}

void publish_memory()
{
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[3])));
  charBuffer[0] = '\0';
  itoa(freeMemory(), charBuffer, 10);
  mqttClient.publish(progBuffer, charBuffer);
}

void publish_relay_state(byte relayIdx, boolean relayState) {
  charBuffer[0] = '\0';
  if (relayState) { // relay ON
    DEBUG_LOG(1, "relay on");
    sprintf(charBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 1);
  } else {
    DEBUG_LOG(1, "relay off");
    sprintf(charBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 0);
  }
  DEBUG_LOG(1, "charBuffer: ");
  DEBUG_LOG(1, charBuffer);
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[6])));
  // create message in format "idx,ON"
  // add relay index
  DEBUG_LOG(1, "progBuffer: ");
  DEBUG_LOG(1, progBuffer);
  mqttClient.publish(progBuffer, charBuffer);
}



#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
