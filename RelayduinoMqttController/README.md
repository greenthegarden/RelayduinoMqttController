# RelayduinoMqttController

See https://oceancontrols.com.au/KTA-223.html

When the jumper labeled AUTO is installed the board will reset each time a serial connection is made to
the USB serial port. This should only be installed when reprogramming via the Arduino Environment, or
the device will reset each time a serial connection is made to the unit.

When using the KTA-223 with the Arduino Environment select “Arduino Duemilenove w/ ATmega328”
from the “Tools->Board” menu, and install the “AUTO” jumper on the PCB for ease of programming.

https://github.com/sudar/MemoryFree
