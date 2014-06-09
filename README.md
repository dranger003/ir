ir
==

Tiny program to read and write infrared codes using the lirc_rpi
driver written by Aron Robert Szabo and Michael Bishop.
Receiving codes to stdout and sending codes from stdin. Codes are
separated by a space. IR pauses and pulses are read and written
as integers from and to the lirc_rpi driver char device /dev/lirc0.
By default the char device is set to read from GPIO 17 and write to
GPIO 18 (using PWM). It is assumed you are using LIRC_MODE_MODE2
and there is no need to be running the lircd daemon for this program
to work. Refer to http://www.lirc.org/ for more information.
