#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_

#include <PubSubClient.h>

// MQTT parameters
IPAddress mqttServerAddr(192, 168, 1, 52); // emonPi
const char MQTT_CLIENT_ID[] = "relayduino";
const char MQTT_USERNAME[]  = "emonpi";
const char MQTT_PASSWORD[]  = "emonpimqtt2016";
const int  MQTT_PORT        = 1883;

unsigned long lastReconnectAttempt = 0UL;
const unsigned long RECONNECTION_ATTEMPT_INTERVAL = 5000UL;

boolean mqttClientConnected = false;

const char COMMAND_SEPARATOR = ',';

// callback function definition
void callback(char *topic, uint8_t *payload, unsigned int length);

// mqttclient definition
PubSubClient mqttClient(mqttServerAddr, MQTT_PORT, callback, ethernetClient);

const char MQTT_PAYLOAD_CONNECTED[] PROGMEM = "CONNECTED";
const char MQTT_PAYLOAD_OK[] PROGMEM = "OK";
const char MQTT_PAYLOAD_ERROR[] PROGMEM = "ERROR";
const char MQTT_PAYLOAD_START[] PROGMEM = "START";
const char MQTT_PAYLOAD_END[] PROGMEM = "END";
const char MQTT_PAYLOAD_SLEEP[] PROGMEM = "SLEEP";

PGM_P const MQTT_PAYLOADS[] PROGMEM = {
    MQTT_PAYLOAD_CONNECTED, // idx = 0
    MQTT_PAYLOAD_OK,        // idx = 1
    MQTT_PAYLOAD_ERROR,     // idx = 2
    MQTT_PAYLOAD_START,     // idx = 3
    MQTT_PAYLOAD_END,       // idx = 4
    MQTT_PAYLOAD_SLEEP,     // idx = 5
};

typedef enum {
  MQTT_PAYLOAD_CONNECTED_IDX = 0,
  MQTT_PAYLOAD_OK_IDX = 1,
  MQTT_PAYLOAD_ERROR_IDX = 2,
  MQTT_PAYLOAD_START_IDX = 3,
  MQTT_PAYLOAD_END_IDX = 4,
  MQTT_PAYLOAD_SLEEP_IDX = 5,
} mqtt_payloads;

const char MQTT_STATUS[] PROGMEM = "relayduino/status/mqtt";
const char VERSION_STATUS[] PROGMEM = "relayduino/status/version";
const char INTERVAL_STATUS[] PROGMEM = "relayduino/status/interval";
const char IP_ADDR_STATUS[] PROGMEM = "relayduino/status/ip_addr";
const char UPTIME_STATUS[] PROGMEM = "relayduino/status/uptime";
const char MEMORY_STATUS[] PROGMEM = "relayduino/status/memory";
const char TIME_STATUS[] PROGMEM = "relayduino/status/time";
const char ALARM_STATUS[] PROGMEM = "relayduino/status/alarm";
const char RELAY_STATUS[] PROGMEM = "relayduino/status/relay";
const char FLOWRATE_STATUS[] PROGMEM = "relayduino/status/flowrate";

PGM_P const STATUS_TOPICS[] PROGMEM = {
    MQTT_STATUS,     // idx = 0
    VERSION_STATUS,  // idx = 1
    INTERVAL_STATUS, // idx = 2
    IP_ADDR_STATUS,  // idx = 3
    UPTIME_STATUS,   // idx = 4
    MEMORY_STATUS,   // idx = 5
    TIME_STATUS,     // idx = 6
    ALARM_STATUS,    // idx = 7
    RELAY_STATUS,    // idx = 8
    FLOWRATE_STATUS,
};

typedef enum {
  MQTT_STATUS_IDX = 0,
  VERSION_STATUS_IDX = 1,
  INTERVAL_STATUS_IDX = 2,
  IP_ADDR_STATUS_IDX = 3,
  UPTIME_STATUS_IDX = 4,
  MEMORY_STATUS_IDX = 5,
  TIME_STATUS_IDX = 6,
  ALARM_STATUS_IDX = 7,
  RELAY_STATUS_IDX = 8,
  FLOWRATE_STATUS_IDX = 9,
} status_topics;

const char ANALOG_INPUT[] PROGMEM = "relayduino/input/analog";
const char OPTO_INPUT[] PROGMEM = "relayduino/input/opto";

PGM_P const INPUT_TOPICS[] PROGMEM = {
    ANALOG_INPUT, // idx = 0
    OPTO_INPUT,   // idx = 1
};

typedef enum {
  ANALOG_INPUT_IDX = 0,
  OPTO_INPUT_IDX = 1,
} input_topics;

const char UPTIME_REQUEST[] PROGMEM = "relayduino/request/uptime";
const char STATE_REQUEST[] PROGMEM = "relayduino/request/relay_state";

PGM_P const REQUEST_TOPICS[] PROGMEM = {
    UPTIME_REQUEST, // idx = 0
    STATE_REQUEST,  // idx = 1
};

typedef enum {
  UPTIME_REQUEST_IDX = 0,
  STATE_REQUEST_IDX = 1,
} request_topics;

// Control topics

const char RELAY_CONTROL[] PROGMEM = "relayduino/control/relay";

PGM_P const CONTROL_TOPICS[] PROGMEM = {
    RELAY_CONTROL, // idx = 0
};

// INPUT_TOPICS indices, must match table above
typedef enum {
  RELAY_CONTROL_IDX = 0,
} control_topics;

void publish_connected()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[MQTT_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  strcpy_P(payloadBuffer,
           (char *)pgm_read_word(&(MQTT_PAYLOADS[MQTT_PAYLOAD_CONNECTED_IDX])));
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_version()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[VERSION_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  sprintf(payloadBuffer, "%s", VERSION);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_status_interval()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[INTERVAL_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer,
                     ltoa(STATUS_UPDATE_INTERVAL, payloadBuffer, 10));
}

void publish_ip_address()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[IP_ADDR_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  IPAddress ip = Ethernet.localIP();
  //  strcpy(payloadBuffer, Ethernet.localIP());
  sprintf(payloadBuffer, "%i%c%i%c%i%c%i", ip[0], '.', ip[1], '.', ip[2], '.',
          ip[3]);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_uptime()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[UPTIME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  mqttClient.publish(topicBuffer, ltoa(millis(), payloadBuffer, 10));
}

void publish_memory()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[MEMORY_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  //  itoa(getFreeMemory(), payloadBuffer, 10);
  //  mqttClient.publish(topicBuffer, payloadBuffer);
  mqttClient.publish(topicBuffer, itoa(getFreeMemory(), payloadBuffer, 10));
}

void publish_relay_state(byte relayIdx, boolean relayState)
{
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
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[RELAY_STATUS_IDX])));
  // create message in format "idx,ON"
  // add relay index
  DEBUG_LOG(1, "topicBuffer: ");
  DEBUG_LOG(1, topicBuffer);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

void publish_configuration()
{
  publish_version();
  publish_status_interval();
  publish_ip_address();
}

void publish_status()
{
  publish_uptime();
  publish_memory();
}

#endif /* RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_ */
