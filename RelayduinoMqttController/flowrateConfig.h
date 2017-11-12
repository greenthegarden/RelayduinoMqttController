const unsigned long FLOWRATE_UPDATE_INTERVAL = 60UL * 1000UL; // 1 minute
unsigned long flowratePreviousMillis = 0UL;


unsigned long volumeCumulative = 0UL;
int volume = 0;
unsigned long flowratePeriod = 1UL; // in seconds
boolean flowOn = false;

void flowrate(unsigned char src, char command, unsigned char len, char *data)
{
  int flowrate = atoi(data);
  if (!flowOn && flowrate > 0) {
    flowOn = true;
  }
  if (flowOn && flowrate < 1) {
    flowOn = false;
    volumeCumulative = 0;
    flowrate = 0;
  }
  if (flowrate > 0) {
    volumeCumulative += flowrate * flowratePeriod;
  }
}

void publish_flowrate()
{
//  Serial.println(data);
  topicBuffer[0] = '\0';
  strcpy_P(topicBuffer,
           (char *)pgm_read_word(&(STATUS_TOPICS[FLOWRATE_STATUS_IDX])));
  payloadBuffer[0] = '\0';
  ltoa(volumeCumulative, payloadBuffer, 10);
  mqttClient.publish(topicBuffer, payloadBuffer);
}
