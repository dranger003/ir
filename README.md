ir
==

Tiny program to read and write infrared codes using the lirc_rpi<br />
driver written by Aron Robert Szabo and Michael Bishop.<br />
Receiving codes to stdout and sending codes from stdin. Codes are<br />
separated by a space. IR pauses and pulses are read and written<br />
as integers from and to the lirc_rpi driver char device /dev/lirc0.<br />
By default the char device is set to read from GPIO 17 and write to<br />
GPIO 18 (using PWM). It is assumed you are using LIRC_MODE_MODE2<br />
and there is no need to be running the lircd daemon for this program<br />
to work. Refer to http://www.lirc.org/ for more information.<br />

To receive codes I use a TSOP38238 IR receiver diode:
https://www.sparkfun.com/products/10266

To send codes I use the IR LED kit:
https://www.sparkfun.com/products/10732

NOTE: The codes are raw codes - meaning they may need to be manually modified and/or stipped before they become useful to send back to the device.
