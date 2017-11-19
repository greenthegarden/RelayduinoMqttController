#ifndef RELAYDUINOMQTTCONTROLLER_FLOW_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_FLOW_CONFIG_H_

const unsigned long VOLUME_UPDATE_INTERVAL = 60UL * 1000UL; // 1 minute
unsigned long volumePreviousMillis = 0UL;

unsigned long volumeCumulative = 0UL;
int volume = 0;

unsigned long previousFlowrate = 0UL;
boolean flowOn = false;

void flowrate(unsigned char src, char command, unsigned char len, char *data)
{
  unsigned long now = millis();   // consider using micros()
  int flowrate = atoi(data);
  if (!flowOn && flowrate > 0) {
    flowOn = true;
  }
  if (flowOn) {
    if (flowrate < 1) {
      flowOn = false;
      volumeCumulative = 0;
      flowrate = 0;
    }
    if (flowrate > 0) {
      volumeCumulative += flowrate * (now - previousFlowrate) / 1000;
    }
    topicBuffer[0] = '\0';
    strcpy_P(topicBuffer,
             (char *)pgm_read_word(&(STATUS_TOPICS[FLOWRATE_STATUS_IDX])));
    payloadBuffer[0] = '\0';
    ltoa(volumeCumulative, payloadBuffer, 10);
    mqttClient.publish(topicBuffer, payloadBuffer);
  }
  previousFlowrate = now;
}

void publish_volume()
{
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[VOLUME_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  ltoa(volumeCumulative, payloadBuffer, 10);
  mqttClient.publish(topicBuffer, payloadBuffer);
}

#endif /* RELAYDUINOMQTTCONTROLLER_FLOW_CONFIG_H_ */
