#ifndef RELAYDUINOMQTTCONTROLLER_TIME_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_TIME_CONFIG_H_


#include <Time.h>


IPAddress timeServer(192, 168, 1, 100);

// time parameters
// Offset hours from gps time (UTC)
#define TZ_OFFSET_HOURS             (9UL)  // Australian CST + 30 MINS (+10 for DST)
#define TZ_OFFSET_MINUTES           (30UL)
bool daylight_summer_time         = false;

// write time and date to char_buffer (19 chars) in format
// 0000000000111111111
// 0123456789012345678
// HH:MM:SS DD-MM-YYYY
void date_string()
{
  char_buffer[0] = '\0';
  sprintf(char_buffer,
          "%02d:%02d:%02d %02d-%02d-%02d",
          hour(),
          minute(),
          second(),
          day(),
          month(),
          year()
          );
   if (daylight_summer_time)
     strcat(char_buffer, " DST");
}

//void time_set()
//{
//  if (!wifly_connected)
//    wifly_connect();
//
//  if (wifly_connected) {
//#if DEBUG
//    debug(F("setting time"));
//#endif
//    // set time on Arduino
//    setTime(WiFly.getTime());
//    // adjust time to timezone
//    adjustTime(TZ_OFFSET_HOURS * SECS_PER_HOUR + TZ_OFFSET_MINUTES * SECS_PER_MIN);
//  }
//}


void publish_date()
{
  prog_buffer[0] = '\0';
  strcpy_P(prog_buffer, (char*)pgm_read_word(&(STATUS_TOPICS[3])));
  date_string();  // date is stored in char_buffer
  mqtt_client.publish(prog_buffer, char_buffer);
}


/*-------- NTP code ----------*/

#include <EthernetUdp.h>

EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49;
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:                 
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

time_t getNtpTime()
{
  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  DEBUG_LOG(3, "Transmit NTP Request");
  sendNTPpacket(timeServer);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      DEBUG_LOG(3, "Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + TZ_OFFSET_HOURS * SECS_PER_HOUR + TZ_OFFSET_MINUTES * SECS_PER_MIN;
    }
  }
  DEBUG_LOG(1, "No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}


#endif  /* RELAYDUINOMQTTCONTROLLER_TIME_CONFIG_H_ */

