// “Arduino Duemilenove w/ ATmega328”
// install the “AUTO” jumper on the PCB for ease of programming

#include "config.h"

boolean mqtt_connect()
{
  DEBUG_LOG(1, "Attempting MQTT connection ...");
  if (mqttClient.connect(mqttClientId)) {
    DEBUG_LOG(1, "  connected");
    // Once connected, publish an announcement ...
    publish_connected();
    publish_ip_address();
#if USE_FREEMEM
    publish_memory();
#endif
    // ... and subscribe to topics (should have list)
    mqttClient.subscribe("relayduino/request/#");
    mqttClient.subscribe("relayduino/control/#");
  } else {
    DEBUG_LOG(1, "failed, rc = ");
    DEBUG_LOG(1, mqttClient.state());
  }
  return mqttClient.connected();
}

void callback(char* topic, uint8_t* payload, unsigned int payloadLength)
{
  // handle message arrived
  /* topic = part of the variable header:has topic name of the topic where the publish received
       NOTE: variable header does not contain the 2 bytes with the
            publish msg ID
      payload = pointer to the first item of the buffer array that
                contains the message tha was published
               EXAMPLE of payload: lights,1
      length = the length of the payload, until which index of payload
  */

  DEBUG_LOG(1, "Payload length is");
  DEBUG_LOG(1, payloadLength);

  // Copy the payload to the new buffer
  char* message = (char*)malloc((sizeof(char) * payloadLength) + 1); // get the size of the bytes and store in memory
  memcpy(message, payload, payloadLength * sizeof(char));        // copy the memory
  message[payloadLength * sizeof(char)] = '\0';                  // add terminating character

  DEBUG_LOG(1, "Message with topic");
  DEBUG_LOG(1, topic);
  DEBUG_LOG(1, "arrived with payload");
  DEBUG_LOG(1, message);

  byte topicIdx = 0;
  boolean requestTopicFound = false;
  boolean controlTopicFound = false;

  // find if topic is matched
  // first check request topics
  for (byte i = 0; i < ARRAY_SIZE(REQUEST_TOPICS); i++) {
    progBuffer[0] = '\0';
    strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(REQUEST_TOPICS[i])));
    if (strcmp(topic, progBuffer) == 0) {
      topicIdx = i;
      requestTopicFound = true;
      break;
    }
  }
  if (requestTopicFound) {
    if (topicIdx == 0) {  // topic is STATE_REQUEST
      DEBUG_LOG(1, "STATE_REQUEST topic arrived");
    } else {  // unknown request topic has arrived - ignore!!
      DEBUG_LOG(1, "Unknown request topic arrived");
    }
  } else {
    // check control topics
    topicIdx = 0;
    // find if topic is matched
    for (byte i = 0; i < ARRAY_SIZE(CONTROL_TOPICS); i++) {
      progBuffer[0] = '\0';
      strcpy_P(progBuffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[i])));
      if (strcmp(topic, progBuffer) == 0) {
        topicIdx = i;
        controlTopicFound = true;
        break;
      }
    }

    if (controlTopicFound) {
      DEBUG_LOG(1, "Control topic index");
      DEBUG_LOG(1, topicIdx);
      //switch to case statements
      if (topicIdx == 0) {  // topic is RELAY_CONTROL
        DEBUG_LOG(1, "RELAY_CONTROL topic arrived");
        // message is expected to be in format "relay,duration"
        // get relay and duration from message
        // see http://arduino.stackexchange.com/questions/1013/how-do-i-split-an-incoming-string
        char* separator = strchr(message, COMMAND_SEPARATOR);
        DEBUG_LOG(1, "separator: ");
        DEBUG_LOG(1, separator);
        if (separator != 0) {
            byte relayIdx = atoi(message)-1;
            DEBUG_LOG(1, "relayIdx: ");
            DEBUG_LOG(1, relayIdx);
            ++separator;
            unsigned long duration = atoi(separator);
            DEBUG_LOG(1, "duration: ");
            DEBUG_LOG(1, duration);
            if (duration>0) {
              relay_switch_on_with_timer(relayIdx, duration);
            } else {
              relay_switch_off(relayIdx);
            }
        }
      } else {  // unknown control topic has arrived - ignore!!
        DEBUG_LOG(1, "Unknown control topic arrived");
      }
    }
  }

  // free memory assigned to message
  free(message);
}

/*--------------------------------------------------------------------------------------
  setup()
  Called by the Arduino framework once, before the main loop begins
  --------------------------------------------------------------------------------------*/
void setup()
{
#if DEBUG_LEVEL > 0
  Serial.begin(BAUD_RATE);
#endif

  // Configure Ethernet
  Ethernet.begin(mac, ip);
  delay(1500);

  // configure relay pins as outputs and set to LOW
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    pinMode(RELAY_PINS_USED[idx], OUTPUT);
    digitalWrite(RELAY_PINS_USED[idx], LOW);
  }

  DEBUG_LOG(1, "Number of relays is ");
  DEBUG_LOG(1, ARRAY_SIZE(RELAY_PINS_USED));
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  // require an Alarm.delay in order to allow alarms to work
  Alarm.delay(0);

  if (!mqttClient.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > RECONNECTION_ATTEMPT_INTERVAL) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    mqttClient.loop();
  }
}

