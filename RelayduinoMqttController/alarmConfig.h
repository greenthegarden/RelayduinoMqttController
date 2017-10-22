#ifndef RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_

#include <TimeAlarms.h>

byte currentTimerRef = dtINVALID_ALARM_ID;

void publish_alarm_id(byte ref = dtINVALID_ALARM_ID)
{
  if (ref == dtINVALID_ALARM_ID) {
    ref = Alarm.getTriggeredAlarmId();
  }
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[ALARM_STATUS_IDX])));
  char str[4];
  mqttClient.publish(topicBuffer, itoa(ref, str, 10));
}

void alarm_cancel()
{
  // disable current alarm
  byte ref = Alarm.getTriggeredAlarmId();
  Alarm.disable(ref);
  publish_alarm_id(ref);
}

#endif /* RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_ */
