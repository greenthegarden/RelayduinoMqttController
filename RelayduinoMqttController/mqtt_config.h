#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_


#include <PubSubClient.h>


// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 50);        // openhab
char mqttClientId[]                               = "relayduino";
const int MQTT_PORT                               = 1883;

boolean mqttClientConnected                       = false;

unsigned long lastReconnectAttempt                = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

// callback function definition
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);


// MQTT topic definitions

// MQTT payloads
const char MQTT_PAYLOAD_CONNECTED[]   PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_ERROR[]       PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[]       PROGMEM = "START";
const char MQTT_PAYLOAD_END[]         PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[]       PROGMEM = "SLEEP";
const char MQTT_PAYLOAD_OK[]          PROGMEM = "OK";

PGM_P const MQTT_PAYLOADS[]           PROGMEM = { MQTT_PAYLOAD_CONNECTED,   // idx = 0
                                                  MQTT_PAYLOAD_OK,          // idx = 1
                                                  MQTT_PAYLOAD_ERROR,       // idx = 2
                                                  MQTT_PAYLOAD_START,       // idx = 3
                                                  MQTT_PAYLOAD_END,         // idx = 4
                                                  MQTT_PAYLOAD_SLEEP,       // idx = 5
                                                };

/* MQTT_PAYLOADS indices, must match table above */
typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX        = 1,
  MQTT_PAYLOAD_ERROR_IDX     = 2,
  MQTT_PAYLOAD_START_IDX     = 3,
  MQTT_PAYLOAD_END_IDX       = 4,
  MQTT_PAYLOAD_SLEEP_IDX     = 5,
} mqtt_payloads;

// Status topics
const char MQTT_STATUS[]       PROGMEM = "relayduino/status/mqtt";
const char IP_ADDR_STATUS[]    PROGMEM = "relayduino/status/ip_addr";
const char UPTIME_STATUS[]     PROGMEM = "relayduino/status/uptime";
const char MEMORY_STATUS[]     PROGMEM = "relayduino/status/memory";
const char TIME_STATUS[]       PROGMEM = "relayduino/status/time";
const char ALARM_STATUS[]      PROGMEM = "relayduino/status/alarm";
const char RELAY_STATUS[]      PROGMEM = "relayduino/status/relay";

PGM_P const STATUS_TOPICS[]    PROGMEM = { MQTT_STATUS,         // idx = 0
                                           IP_ADDR_STATUS,      // idx = 1
                                           UPTIME_STATUS,       // idx = 2
                                           MEMORY_STATUS,       // idx = 3
                                           TIME_STATUS,         // idx = 4
                                           ALARM_STATUS,        // idx = 5
                                           RELAY_STATUS,        // idx = 6
                                          };

// STATUS_TOPICS indices, must match table above
typedef enum {
  MQTT_STATUS_IDX          = 0,
  IP_ADDR_STATUS_IDX       = 1,
  UPTIME_STATUS_IDX        = 2,
  MEMORY_STATUS_IDX        = 3,
  TIME_STATUS_IDX          = 4,
  ALARM_STATUS_IDX         = 5,
  RELAY_STATUS_IDX         = 6,
} status_topics;


// Relayduino Input topics

const char ANALOG_INPUT[]       PROGMEM = "relayduino/input/analog";
const char OPTO_INPUT[]         PROGMEM = "relayduino/input/opto";

PGM_P const INPUT_TOPICS[]      PROGMEM = { ANALOG_INPUT,   // idx = 0
                                            OPTO_INPUT,     // idx = 1
                                          };

// INPUT_TOPICS indices, must match table above
typedef enum {
  ANALOG_INPUT_IDX          = 0,
  OPTO_INPUT_IDX            = 1,
} input_topics;


// Request topics

const char UPTIME_REQUEST[]    PROGMEM = "relayduino/request/uptime";
const char STATE_REQUEST[]     PROGMEM = "relayduino/request/relay_state";

PGM_P const REQUEST_TOPICS[]   PROGMEM = { UPTIME_REQUEST,  // idx = 0
                                           STATE_REQUEST,   // idx = 1
                                         };

// INPUT_TOPICS indices, must match table above
typedef enum {
  UPTIME_REQUEST_IDX          = 0,
  STATE_REQUEST_IDX           = 1,
} request_topics;

// Control topics

const char RELAY_CONTROL[]     PROGMEM = "relayduino/control/relay";

PGM_P const CONTROL_TOPICS[]   PROGMEM = { RELAY_CONTROL,         // idx = 0
                                         };

// INPUT_TOPICS indices, must match table above
typedef enum {
  RELAY_CONTROL_IDX          = 0,
} control_topics;


const char COMMAND_SEPARATOR                      = ',';

//char message[BUFFER_SIZE];

void publish_connected() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer, (char*)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_ip_address() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  IPAddress ip = Ethernet.localIP();
//  strcpy(payloadBuffer, Ethernet.localIP());
  sprintf(payloadBuffer,
    "%i%c%i%c%i%c%i",
    ip[0], '.',
    ip[1], '.',
    ip[2], '.',
    ip[3]
  );
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_uptime() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

void publish_memory() {
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
//  itoa(getFreeMemory(), payloadBuffer, 10);
//  mqttClient.publish(topicBuffer, payloadBuffer);
  mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
}

void publish_relay_state(byte relayIdx, boolean relayState) {
  payloadBuffer[0] = '\0';
  if (relayState) { // relay ON
    DEBUG_LOG(1, "relay on");
    sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 1);
  } else {
    DEBUG_LOG(1, "relay off");
    sprintf(payloadBuffer, "%i%c%i", relayIdx + 1, COMMAND_SEPARATOR, 0);
  }
  DEBUG_LOG(1, "payloadBuffer: ");
  DEBUG_LOG(1, payloadBuffer);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[RELAY_STATUS_IDX])));
  // create message in format "idx,ON"
  // add relay index
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_status() {
  publish_ip_address();
  publish_uptime();
  publish_memory();
}



#endif   /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
