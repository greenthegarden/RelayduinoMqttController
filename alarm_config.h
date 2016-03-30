#ifndef RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_


#include <TimeAlarms.h>


byte currentTimerRef              = 255;


void publish_alarm_id(byte ref = 255)
{
  if (ref == 255)
    ref = Alarm.getTriggeredAlarmId();
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[5])));
  char str[4];
  mqttClient.publish(progBuffer, itoa(ref, str, 10));
}

void alarm_cancel()
{
  // disable current alarm
  byte ref = Alarm.getTriggeredAlarmId();
  Alarm.disable(ref);
  publish_alarm_id(ref);
}


#endif   /* RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_ */

