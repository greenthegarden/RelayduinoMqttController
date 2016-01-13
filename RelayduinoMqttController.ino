#include "config.h"

void callback(char* topic, uint8_t* payload, unsigned int payload_length)
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
  DEBUG_LOG(1, payload_length);

  // Copy the payload to the new buffer
  char* message = (char*)malloc((sizeof(char) * payload_length) + 1); // get the size of the bytes and store in memory
  memcpy(message, payload, payload_length * sizeof(char));        // copy the memory
  message[payload_length * sizeof(char)] = '\0';                  // add terminating character

  DEBUG_LOG(1, "Message with topic");
  DEBUG_LOG(1, topic);
  DEBUG_LOG(1, "arrived with payload");
  DEBUG_LOG(1, message);

  byte topic_idx = 0;
  // find if topic is matched
  for (byte i = 0; i < ARRAY_SIZE(CONTROL_TOPICS); i++) {
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(CONTROL_TOPICS[i])));
    if (strcmp(topic, prog_buffer) == 0) {
      topic_idx = i;
      break;
    }
  }
  DEBUG_LOG(1, "Control topic index");
  //  DEBUG_LOG(1, topic_idx);

  if (topic_idx == 0) {  // topic is UPTIME_REQUEST
    publish_uptime();
    DEBUG_LOG(1, "UPTIME_REQUEST topic arrived");
    //    DEBUG_LOG(1, (char) millis());
  } else if (topic_idx == 1) {  // topic is MEMORY_REQUEST
    publish_memory();
    DEBUG_LOG(1, "MEMORY_REQUEST topic arrived");
    //    DEBUG_LOG(1, (char) freeMemory());
  } else if (topic_idx == 2) {  // LED_CONTROL
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
  Serial.begin(BAUD_RATE);

  // Configure Ethernet
  Ethernet.begin(mac, ip);
  delay(1500);

  // configure relay pins as outputs and set to LOW
#if USE_MASTER_RELAY  
  pinMode(RELAY_MASTER, OUTPUT);
  digitalWrite(RELAY_MASTER, LOW);
#endif
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    pinMode(RELAY_PINS_USED[idx], OUTPUT);
    digitalWrite(RELAY_PINS_USED[idx], LOW);
  }
  
//  if (timeStatus() != timeSet)
//    time_set();
  date_string();

  DEBUG_LOG(3, "Date: ");
  DEBUG_LOG(3, char_buffer);
  DEBUG_LOG(3, "Number of relays is ");
  DEBUG_LOG(3, ARRAY_SIZE(RELAY_PINS_USED));

  // Define default irrigation alarms
  // start times are fixed and days are fixed
  // durations are defined by relayDurations[]

  // Irrigation Zone 1: Front grass
  // Set to Sunday and Wednesday evenings @ 1800
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(dowSunday, 18, 0, 0, turn_on_relay_1);
  alarm_refs_cnt++;
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(dowWednesday, 18, 0, 0, turn_on_relay_1);
  alarm_refs_cnt++;
    
  // Irrigation Zone 2: Flower beds
  // Set to Sunday and Wednesday evenings @ 1900
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(dowSunday, 19, 0, 0, turn_on_relay_2);
  alarm_refs_cnt++;
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(dowWednesday, 19, 0, 0, turn_on_relay_2);
  alarm_refs_cnt++;
  
  // Irrigation Zone 3: Fruit trees
  // Set to Saturday morning @ 0730
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(dowSaturday, 7, 30, 0, turn_on_relay_3);
  alarm_refs_cnt++;

  // Relay 4: Vegetable beds
  // Set to everyday @ 0700
  alarm_refs[alarm_refs_cnt] = Alarm.alarmRepeat(7, 0, 0, turn_on_relay_4); //alarm_refs_cnt++;  
}

/*--------------------------------------------------------------------------------------
  loop()
  Arduino main loop
  --------------------------------------------------------------------------------------*/
void loop()
{
  // require an Alarm.delay in order to allow alarms to work
  Alarm.delay(0);

  if (!mqtt_client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (mqtt_connect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    // Client connected
    mqtt_client.loop();
  }
}

