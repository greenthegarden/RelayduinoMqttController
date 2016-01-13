#include <Relayduino.h>

// default times (minutes)
int relay_1_duration              = 30;    // front grass
int relay_2_duration              = 60;    // shrubs
int relay_3_duration              = 180;   // trees
int relay_4_duration              = 15;    // vegetables

const byte RELAY_PINS_USED[] = {RELAY_1, RELAY_2, RELAY_3, RELAY_4};
int        relay_durations[] = {relay_1_duration, relay_2_duration, relay_3_duration, relay_4_duration};

void publish_relay_durations()
{
//  for ( byte idx=0; idx<sizeof(relay_durations)/sizeof(int); idx++ ) {
  for (byte idx = 0; idx < ARRAY_SIZE(relay_durations); idx++) {
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[7])));
    char str[4];
    mqtt_client.publish(prog_buffer, itoa(relay_durations[idx], str, 10));
  }
}

#if USE_MASTER_RELAY
// returns 1 if relay is currently on and switched off, else returns 0
byte master_relay_off()
{
  // only switch relay off if it is currently on
  if (digitalRead(RELAY_MASTER)) {
    digitalWrite(RELAY_MASTER, LOW);
#if DEBUG
    debug(F("master off"));
#endif
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[4])));
    mqtt_client.publish(prog_buffer, "M");
    return 1;
  }
  return 0;
}

// returns 1 if relay is currently on and switched off, else returns 0
byte master_relay_on()
{
  // only switch relay on if it is currently off
  if(!digitalRead(RELAY_MASTER)) {
    digitalWrite(RELAY_MASTER, HIGH);
#if DEBUG
    debug(F("master on"));
#endif
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[3])));
    mqtt_client.publish(prog_buffer, "M");
    return 1;
  }
  return 0;
}
#endif

// returns 1 if relay connected to given pin is on, else returns 0
byte relay_state(byte idx)
{
  return(digitalRead(RELAY_PINS_USED[idx]));
}

void relays_state()
{
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    if (digitalRead(RELAY_PINS_USED[idx])) {
#if DEBUG
      debug(F("relay on"));
#endif
      prog_buffer[0] = '\0';
      strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[3])));
      char str[2];
      mqtt_client.publish(prog_buffer, itoa(idx+1, str, 10));
    } else {
#if DEBUG
      debug(F("relay off"));
#endif
      prog_buffer[0] = '\0';
      strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[4])));
      char str[2];
      mqtt_client.publish(prog_buffer, itoa(idx+1, str, 10));
    }
  }
}

// returns 1 if relay is currently on and switched off, else returns 0
byte relay_switch_off(byte idx)
{
  // only switch relay off if it is currently on
  if (relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], LOW);
#if DEBUG
    debug(F("relay off"));
#endif
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[4])));
    char str[2];
    mqtt_client.publish(prog_buffer, itoa(idx+1, str, 10));
#if USE_MASTER_RELAY
    master_relay_off();
#endif
#if USE_LED
    digitalWrite(LED_PIN, LOW);
#endif
    return 1;
  }
  return 0;
}

// used by callback as a void function to switch off relay which is currenlty on
// and then switches off master
void relays_switch_off()
{
  for (byte idx = 0; idx < ARRAY_SIZE(RELAY_PINS_USED); idx++) {
    if (relay_state(idx))
      relay_switch_off(idx);
  }
}

byte timer_stop()
{
  return (Alarm.timerOnce(1, relays_switch_off));
}

// returns 1 if relay is currently off and switched on, else returns 0
byte relay_switch_on(byte idx)
{
  if (!relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], HIGH);
#if DEBUG
    debug(F("relay on"));
#endif
    prog_buffer[0] = '\0';
    strcpy_P(prog_buffer, (PGM_P)pgm_read_word(&(STATUS_TOPICS[3])));
    char str[2];
    mqtt_client.publish(prog_buffer, itoa(idx+1, str, 10));
#if USE_MASTER_RELAY
    master_relay_on();
#endif
#if USE_LED
    digitalWrite(LED_PIN, HIGH);
#endif
    return 1;
  }
  return 0;
}

byte relay_switch_on_with_timer(byte idx, int duration)
{
  if (relay_switch_on(idx)) {
    current_timer_ref = Alarm.timerOnce(duration * SECS_PER_MIN, relays_switch_off);
    publish_alarm_id(current_timer_ref);
    return 1;
  }
  return 0;
}

// define void functions for each irrigation zone required as alarm callbacks
void turn_on_relay_1()
{
  byte idx = 0;
  relay_switch_on_with_timer(idx, relay_durations[idx]);
}

void turn_on_relay_2()
{
  byte idx = 1;
  relay_switch_on_with_timer(idx, relay_durations[idx]);
}

void turn_on_relay_3()
{
  byte idx = 2;
  relay_switch_on_with_timer(idx, relay_durations[idx]);
}

void turn_on_relay_4()
{
  byte idx = 3;
  relay_switch_on_with_timer(idx, relay_durations[idx]);
}
