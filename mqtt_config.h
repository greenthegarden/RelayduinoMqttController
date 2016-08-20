#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50);        // openhab
char mqttClientId[]                               = "relayduino";
const int MQTT_PORT                               = 1883;
//#define MQTT_MAX_PACKET_SIZE                      168
//#define MQTT_KEEPALIVE                            300

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

const char COMMAND_SEPARATOR                      = ',';

char message[BUFFER_SIZE];


// Status topics

const char CONNECTED_STATUS[]  PROGMEM = "relayduino/status/connected";
const char IP_ADDR_STATUS[]    PROGMEM = "relayduino/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayduino/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayduino/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayduino/status/time";
const char ALARM_STATUS[]      PROGMEM = "relayduino/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "relayduino/status/relay";

PGM_P const STATUS_TOPICS[]    PROGMEM = { CONNECTED_STATUS,    // idx = 0
                                           IP_ADDR_STATUS,      // idx = 1
                                           UPTIME_STATUS,       // idx = 2
                                           MEMORY_STATUS,       // idx = 3
                                           TIME_STATUS,         // idx = 4
                                           ALARM_STATUS,        // idx = 5
                                           RELAY_STATUS,        // idx = 6
                                          };


// Relayduino Input topics

const char ANALOG_INPUT[]       PROGMEM = "relayduino/input/analog_in";
const char OPTO_INPUT[]         PROGMEM = "relayduino/input/opto_input";

PGM_P const INPUT_TOPICS[]      PROGMEM = { ANALOG_INPUT,   // idx = 0
                                            OPTO_INPUT,     // idx = 1
                                          };


// Request topics

const char TIME_REQUEST[]      PROGMEM = "relayduino/request/time";
const char STATE_REQUEST[]     PROGMEM = "relayduino/request/relay_state";

PGM_P const REQUEST_TOPICS[]   PROGMEM = {STATE_REQUEST,          // idx = 0
                                         };


// Control topics

const char RELAY_CONTROL[]     PROGMEM = "relayduino/control/relay";


PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,         // idx = 0
                                          };


// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

void publish_connected() {
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqttClient.publish(progBuffer, "");
}

void publish_ip_address() {
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  mqttClient.publish(progBuffer, "192.168.1.90");
}

void publish_uptime() {
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  charBuffer[0] = '\0';
  ltoa(millis(), charBuffer, 10);
  mqttClient.publish(progBuffer, charBuffer);
}

void publish_memory() {
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[3])));
  charBuffer[0] = '\0';
  itoa(getFreeMemory(), charBuffer, 10);
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

void publish_status() {
  publish_uptime();
  publish_memory();
}



#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
