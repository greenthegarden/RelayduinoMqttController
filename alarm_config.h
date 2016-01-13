#include <TimeAlarms.h>
byte current_timer_ref = 255;

const byte MAX_ALARMS             = 10;
byte alarm_refs[MAX_ALARMS];
byte alarm_refs_cnt               = 0;



void publish_alarm_id(byte ref = 255)
{
  if (ref == 255)
    ref = Alarm.getTriggeredAlarmId();
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[5])));
  char str[4];
  mqtt_client.publish(prog_buffer, itoa(ref, str, 10));
}

void publish_alarms(bool enabled)
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[6])));
  if ( enabled )
    mqtt_client.publish(prog_buffer, "enabled");
  else
    mqtt_client.publish(prog_buffer, "disabled");
}

void disable_alarms()
{
  for (byte idx = 0; idx < ARRAY_SIZE(alarm_refs); idx++) {
    Alarm.disable(alarm_refs[idx]);
  }
  publish_alarms(false);
}

void enable_alarms()
{
  for (byte idx = 0; idx < alarm_refs_cnt; idx++) {
    Alarm.enable(alarm_refs[idx]);
  }
  publish_alarms(true);
}

