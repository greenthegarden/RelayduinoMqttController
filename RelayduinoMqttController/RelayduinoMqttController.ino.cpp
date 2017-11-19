# 1 "/tmp/tmpTiujgh"
#include <Arduino.h>
# 1 "/home/philip/Development/projects/arduino/RelayduinoMqttController/RelayduinoMqttController/RelayduinoMqttController.ino"



#include "config.h"
boolean mqtt_connect();
void callback(char *topic, uint8_t *payload, unsigned int payloadLength);
void setup();
void loop();
#line 6 "/home/philip/Development/projects/arduino/RelayduinoMqttController/RelayduinoMqttController/RelayduinoMqttController.ino"
boolean mqtt_connect()
{
  DEBUG_LOG(1, "Attempting MQTT connection ...");
  if (mqttClient.connect(MQTT_CLIENT_ID, MQTT_USERNAME, MQTT_PASSWORD)) {
    DEBUG_LOG(1, "  connected");

    publish_connected();
    publish_configuration();
    publish_status();

    mqttClient.subscribe("relayduino/request/#");
    mqttClient.subscribe("relayduino/control/#");
  } else {
    DEBUG_LOG(1, "failed, rc = ");
    DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}

void callback(char *topic, uint8_t *payload, unsigned int payloadLength)
{
  DEBUG_LOG(1, "Payload length is");
  DEBUG_LOG(1, payloadLength);


  char *message =
      (char *)malloc((sizeof(char) * payloadLength) +
                     1);
  memcpy(message, payload, payloadLength * sizeof(char));
  message[payloadLength * sizeof(char)] = '\0';

  DEBUG_LOG(1, "Message with topic");
  DEBUG_LOG(1, topic);
  DEBUG_LOG(1, "arrived with payload");
  DEBUG_LOG(1, message);

  byte topicIdx = 0;
  boolean requestTopicFound = false;
  boolean controlTopicFound = false;



  for (byte i = 0; i < ARRAY_SIZE(REQUEST_TOPICS); i++) {
    topicBuffer[0] = '\0';
    strcpy_P(topicBuffer, (PGM_P)pgm_read_word(&(REQUEST_TOPICS[i])));
    if (strcmp(topic, topicBuffer) == 0) {
      topicIdx = i;
      requestTopicFound = true;
      break;
    }
  }
  if (requestTopicFound) {
    if (topicIdx == STATE_REQUEST_IDX) {
      DEBUG_LOG(1, "STATE_REQUEST topic arrived");
    } else {
      DEBUG_LOG(1, "Unknown request topic arrived");
    }
  } else {

    topicIdx = 0;

    for (byte i = 0; i < ARRAY_SIZE(CONTROL_TOPICS); i++) {
      topicBuffer[0] = '\0';
      strcpy_P(topicBuffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[i])));
      if (strcmp(topic, topicBuffer) == 0) {
        topicIdx = i;
        controlTopicFound = true;
        break;
      }
    }

    if (controlTopicFound) {
      DEBUG_LOG(1, "Control topic index");
      DEBUG_LOG(1, topicIdx);

      if (topicIdx == RELAY_CONTROL_IDX) {
        DEBUG_LOG(1, "RELAY_CONTROL topic arrived");




        char *separator = strchr(message, COMMAND_SEPARATOR);
        DEBUG_LOG(1, "separator: ");
        DEBUG_LOG(1, separator);
        if (separator != 0) {
          byte relayIdx = atoi(message) - 1;
          DEBUG_LOG(1, "relayIdx: ");
          DEBUG_LOG(1, relayIdx);
          ++separator;
          unsigned long duration = atoi(separator);
          DEBUG_LOG(1, "duration: ");
          DEBUG_LOG(1, duration);
          if (duration > 0) {
            relay_switch_on_with_timer(relayIdx, duration);
          } else {
            relay_switch_off(relayIdx);
          }
        }
      } else {
        DEBUG_LOG(1, "Unknown control topic arrived");
      }
    }
  }


  free(message);
}





void setup()
{
  Serial.begin(BAUD_RATE);
  icsc.begin();
  icsc.registerCommand('F', &flowrate);

  DEBUG_LOG(1, "RELAYDUINO");


  ethernet_init();


  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    pinMode(RELAY_PINS_USED[idx], OUTPUT);
    digitalWrite(RELAY_PINS_USED[idx], LOW);
  }
  DEBUG_LOG(1, "Number of relays is ");
  DEBUG_LOG(1, ARRAY_SIZE(RELAY_PINS_USED));

#if USE_INPUTS

  for (byte idx = 0; idx < ARRAY_SIZE(ANALOG_INPUTS); idx++) {
    pinMode(ANALOG_INPUTS[idx], INPUT);
  }
  DEBUG_LOG(1, "Number of analog inputs is ");
  DEBUG_LOG(1, ARRAY_SIZE(ANALOG_INPUTS));
  for (byte idx = 0; idx < ARRAY_SIZE(OPTICAL_INPUTS); idx++) {
    pinMode(OPTICAL_INPUTS[idx], INPUT);
  }
  DEBUG_LOG(1, "Number of optical (digital) inputs is ");
  DEBUG_LOG(1, ARRAY_SIZE(OPTICAL_INPUTS));
#endif
}





void loop()
{
  icsc.process();


  Alarm.delay(0);

  unsigned long now = millis();

  if (!mqttClient.connected()) {
    mqttClientConnected = false;
    if (now - lastReconnectAttempt > RECONNECTION_ATTEMPT_INTERVAL) {
      lastReconnectAttempt = now;

      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
        mqttClientConnected = true;
      }
    }
  } else {

    mqttClient.loop();
  }

  if (now - statusPreviousMillis >= STATUS_UPDATE_INTERVAL) {
    if (mqttClientConnected) {
      statusPreviousMillis = now;
      publish_status();
    }
  }

  if (now - flowratePreviousMillis >= FLOWRATE_UPDATE_INTERVAL) {
    if (mqttClientConnected) {
      statusPreviousMillis = now;
      publish_flowrate();
    }
  }

#if USE_INPUTS
  if (now - inputsPreviousMillis >= INPUT_READ_INTERVAL) {
    if (mqttClientConnected) {
      inputsPreviousMillis = now;
      read_inputs();
    }
  }
#endif

  if (!mqttClientConnected) {
    no_network_behaviour();
  }
}