#ifndef RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_MQTT_CONFIG_H_

#include <PubSubClient.h>

// MQTT parameters
//byte mqtt_server_addr[]           = { 192, 168, 1, 55 };  // Pi eth0 interfaceß
byte mqtt_server_addr[]           = { 192, 168, 42, 1 };    // Pi wlan interface
char mqtt_client_id[]             = "relayduino";
const int MQTT_PORT               = 1883;
#define MQTT_MAX_PACKET_SIZE        168
#define MQTT_KEEPALIVE              300

long lastReconnectAttempt         = 0;
const char COMMAND_SEPARATOR      = ':';

char message[BUFFER_SIZE];

const char WIFLY_STATUS[]      PROGMEM = "relayduino/status/wifly";
const char TIME_STATUS[]       PROGMEM = "relayduino/status/time";
const char MEMORY_STATUS[]     PROGMEM = "relayduino/status/memory";
const char RELAY_ON_STATUS[]   PROGMEM = "relayduino/status/relay_on";
const char RELAY_OFF_STATUS[]  PROGMEM = "relayduino/status/relay_off";
const char ALARM_STATUS[]      PROGMEM = "relayduino/status/alarm";
const char ALARMS_STATUS[]     PROGMEM = "relayduino/status/alarms";
const char DURATIONS_STATUS[]  PROGMEM = "relayduino/status/durations";

PGM_P const STATUS_TOPICS[]    PROGMEM = {WIFLY_STATUS,     // idx = 0
                                          TIME_STATUS,      // idx = 1
                                          MEMORY_STATUS,    // idx = 2
                                          RELAY_ON_STATUS,  // idx = 3
                                          RELAY_OFF_STATUS, // idx = 4
                                          ALARM_STATUS,     // idx = 5
                                          ALARMS_STATUS,    // idx = 6
                                          DURATIONS_STATUS, // idx = 7
                                          };


const char DST_SET[]           PROGMEM = "all/control/dst";

const char TIME_REQUEST[]      PROGMEM = "relayduino/request/time";
const char STATE_REQUEST[]     PROGMEM = "relayduino/request/relay_state";
const char DURATION_REQUEST[]  PROGMEM = "relayduino/request/durations";

const char TIMER_STOP[]        PROGMEM = "relayduino/control/stop";
const char ALARMS_CONTROL[]    PROGMEM = "relayduino/control/alarms";
const char RELAY_CONTROL[]     PROGMEM = "relayduino/control/relay";
const char DURATION_CONTROL[]  PROGMEM = "relayduino/control/duration";

PGM_P const CONTROL_TOPICS[]   PROGMEM = {DST_SET,          // idx = 0
                                          TIME_REQUEST,     // idx = 1
                                          STATE_REQUEST,    // idx = 2
                                          DURATION_REQUEST, // idx = 3
                                          TIMER_STOP,       // idx = 4
                                          ALARMS_CONTROL,   // idx = 5
                                          RELAY_CONTROL,    // idx = 6
                                          DURATION_CONTROL, // idx = 7
                                          };

// callback function definition required here as client needs to be defined before
// including relay.h
void callback(char* topic, uint8_t* payload, unsigned int length);

PubSubClient   mqtt_client(mqtt_server_addr, MQTT_PORT, callback, ethernet_client);

void publish_connected()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[0])));
  mqtt_client.publish(prog_buffer, "connected");
}

void publish_uptime()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[1])));
  char_buffer[0] = '\0';
  ltoa(millis(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);

}
void publish_memory()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[2])));
  char_buffer[0] = '\0';
  itoa(freeMemory(), char_buffer, 10);
  mqtt_client.publish(prog_buffer, char_buffer);
}






boolean mqtt_connect()
{
  DEBUG_LOG(1, "connecting to broker");
  if (mqtt_client.connect(mqtt_client_id)) {
    DEBUG_LOG(1, "  connected");
    publish_connected();
#if USE_FREEMEM
    publish_memory();
#endif
    // subscribe to topics
    mqtt_client.subscribe("relayduino/request/#");
    mqtt_client.subscribe("relayduino/control/#");
  }
  return mqtt_client.connected();
}


#endif
