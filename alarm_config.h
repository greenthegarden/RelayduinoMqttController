#ifndef RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_


#include <TimeAlarms.h>


byte currentTimerRef              = 255;

//const byte MAX_ALARMS             = 10;
//byte alarm_refs[MAX_ALARMS];
//byte alarm_refs_cnt               = 0;


void alarm_cancel()
{
  // disable current alarm
  Alarm.disable(Alarm.getTriggeredAlarmId());
}

void publish_alarm_id(byte ref = 255)
{
  if (ref == 255)
    ref = Alarm.getTriggeredAlarmId();
  progBuffer[0] = '\0';
  strcpy_P(progBuffer, (char*)pgm_read_word(&(STATUS_TOPICS[5])));
  char str[4];
  mqttClient.publish(progBuffer, itoa(ref, str, 10));
}


#endif   /* RELAYDUINOMQTTCONTROLLER_ALARM_CONFIG_H_ */

