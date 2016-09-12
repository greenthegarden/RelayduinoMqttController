#ifndef RELAYDUINOMQTTCONTROLLER_RELAY_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_RELAY_CONFIG_H_


#include <Relayduino.h>

byte RELAY_PINS_USED[] = { RELAY_1, RELAY_2, RELAY_3, RELAY_4, RELAY_5, RELAY_6, RELAY_7, RELAY_8 };

// forward declarations of relay switch functions
byte relay_switch_off(byte idx, boolean report=true);
byte relay_switch_on(byte idx, boolean report=true);

// returns 1 if relay connected to given pin is on, else returns 0
byte relay_state(byte idx) {
  return(digitalRead(RELAY_PINS_USED[idx]));
}

// returns 1 if relay is currently on and switched off, else returns 0
byte relay_switch_off(byte idx, boolean report) {
  // only switch relay off if it is currently on
  if (relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], LOW);
    DEBUG_LOG(1, "relay off");
    if (report) {
      publish_relay_state(idx, false);
    }
    alarm_cancel();
    return 1;
  } else {
    DEBUG_LOG(1, "relay already off");
  }
  return 0;
}

// callback functions for Alarm
void relay1_switch_off() {
  byte relayIdx=0;
  relay_switch_off(relayIdx);
}

void relay2_switch_off() {
  byte relayIdx=1;
  relay_switch_off(relayIdx);
}

void relay3_switch_off() {
  byte relayIdx=2;
  relay_switch_off(relayIdx);
}

void relay4_switch_off() {
  byte relayIdx=3;
  relay_switch_off(relayIdx);
}

void relay5_switch_off() {
  byte relayIdx=4;
  relay_switch_off(relayIdx);
}

void relay6_switch_off() {
  byte relayIdx=5;
  relay_switch_off(relayIdx);
}

void relay7_switch_off() {
  byte relayIdx=6;
  relay_switch_off(relayIdx);
}

void relay8_switch_off() {
  byte relayIdx=7;
  relay_switch_off(relayIdx);
}

// used by callback as a void function to switch off relay which is currenlty on
void relays_switch_off() {
  byte relayCount = ARRAY_SIZE(RELAY_PINS_USED);
  for (byte idx = 0; idx < relayCount; idx++) {
    if (relay_state(idx))
      relay_switch_off(idx);
  }
}

// returns 1 if relay is currently off and switched on, else returns 0
byte relay_switch_on(byte idx, boolean report) {
  if (!relay_state(idx)) {
    digitalWrite(RELAY_PINS_USED[idx], HIGH);
    DEBUG_LOG(1, "relay on");
    if (report) {
      publish_relay_state(idx, true);
//      publish_alarm();
    }
    return 1;
  } else {
    DEBUG_LOG(1, "relay already on");
  }
  return 0;
}

byte relay_switch_on_with_timer(byte idx, int duration) {
  if (relay_switch_on(idx)) {
    currentTimerRef = Alarm.timerOnce(duration * SECS_PER_MIN, relays_switch_off);
    publish_alarm_id(currentTimerRef);
    return 1;
  }
  return 0;
}


#endif   /* RELAYDUINOMQTTCONTROLLER_RELAY_CONFIG_H_ */

