#ifndef RELAYDUINOMQTTCONTROLLER_ONEWIRE_CONFIG_H_
#define RELAYDUINOMQTTCONTROLLER_ONEWIRE_CONFIG_H_

#include <OneWire.h>

const byte DS18B20_PIN = 10;

// DS18S20 Temperature chip i/o
OneWire ds(DS18B20_PIN);  // on pin 10

//void getCurrentTemp(int *sign, int *whole, int *fract);
char temp_string[10];

void onewire_init()
{
  digitalWrite(DS18B20_PIN, LOW);
  pinMode(DS18B20_PIN, INPUT);
}

void OneWireReset (int Pin) // reset.  Should improve to act as a presence pulse
{
  digitalWrite (Pin, LOW);
  pinMode (Pin, OUTPUT);        // bring low for 500 us
  delayMicroseconds (500);
  pinMode (Pin, INPUT);
  delayMicroseconds (500);
}

// output byte d (least sig bit first).
void OneWireOutByte(int Pin, byte d)
{
  byte n;
  for (n=8; n!=0; n--) {
    if ((d & 0x01) == 1) {
      // test least sig bit
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (5);
      pinMode (Pin, INPUT);
      delayMicroseconds (60);
    } else {
      digitalWrite (Pin, LOW);
      pinMode (Pin, OUTPUT);
      delayMicroseconds (60);
      pinMode (Pin, INPUT);
    }
    d = d>>1; // now the next bit is in the least sig bit position.
  }
}

// read byte, least sig byte first
byte OneWireInByte(int Pin)
{
  byte d, n, b;
  for (n=0; n<8; n++) {
    digitalWrite (Pin, LOW);
    pinMode (Pin, OUTPUT);
    delayMicroseconds (5);
    pinMode (Pin, INPUT);
    delayMicroseconds (5);
    b = digitalRead (Pin);
    delayMicroseconds (50);
    d = (d >> 1) | (b<<7); // shift d to right and insert b in most sig bit position
  }
  return (d);
}

void getCurrentTemp(char *temp)
{
  int HighByte, LowByte, TReading, Tc_100, sign, whole, fract;

  OneWireReset(DS18B20_PIN);
  OneWireOutByte(DS18B20_PIN, 0xcc);
  OneWireOutByte(DS18B20_PIN, 0x44); // perform temperature conversion, strong pullup for one sec

  OneWireReset(DS18B20_PIN);
  OneWireOutByte(DS18B20_PIN, 0xcc);
  OneWireOutByte(DS18B20_PIN, 0xbe);

  LowByte = OneWireInByte(DS18B20_PIN);
  HighByte = OneWireInByte(DS18B20_PIN);
  TReading = (HighByte << 8) + LowByte;
  sign = TReading & 0x8000;  // test most sig bit
  if (sign) {
    // negative
    TReading = (TReading ^ 0xffff) + 1; // 2's comp
  }
  Tc_100 = (6 * TReading) + TReading / 4;    // multiply by (100 * 0.0625) or 6.25

  whole = Tc_100 / 100;  // separate off the whole and fractional portions
  fract = Tc_100 % 100;

  if (sign) {
    temp[0] = '-';
  } else {
    temp[0] = '+';
  }

  if (whole/100 == 0) {
    temp[1] = ' ';
  } else {
    temp[1] = whole/100+'0';
  }

  temp[2] = (whole-(whole/100)*100)/10 +'0' ;
  temp[3] = whole-(whole/10)*10 +'0';
  temp[4] = '.';
  temp[5] = fract/10 +'0';
  temp[6] = fract-(fract/10)*10 +'0';
  temp[7] = '\0';
}

#endif /* RELAYDUINOMQTTCONTROLLER_ONEWIRE_CONFIG_H_ */
