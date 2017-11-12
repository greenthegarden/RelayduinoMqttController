#include <ICSC.h>

const char station = 3;  // change station number here (BEFORE uploading to each node)

#ifndef BAUD_RATE
#define BAUD_RATE 115200
#endif

ICSC icsc(Serial, station);
