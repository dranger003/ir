/*
 * ir.c
 *
 * ir - Tiny program to read and write infrared codes using the lirc_rpi
 *      driver written by Aron Robert Szabo and Michael Bishop.
 *      Receiving codes to stdout and sending codes from stdin. Codes are
 *      separated by a space. IR pauses and pulses are read and written
 *      as integers from and to the lirc_rpi driver char device /dev/lirc0.
 *      By default the char device is set to read from GPIO 17 and write to
 *      GPIO 18 (using PWM). It is assumed you are using LIRC_MODE_MODE2
 *      and there is no need to be running the lircd daemon for this program
 *      to work. Refer to http://www.lirc.org/ for more information.
 *
 * Copyright (C) 2014 Daniel Ranger
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <inttypes.h>
#include <errno.h>

static int read_codes()
{
	int fd = open("/dev/lirc0", O_RDONLY);
	if (!fd) {
		fprintf(stderr, "Error: open() failed.\n");
		return 1;
	}

	int i, c = 0;
	struct timespec t1, t2 = { 0 };

	printf("Waiting for codes, press <Ctrl+C> to quit. Wait > 1s before each button press.\n");
	printf("NOTE: You may need to receive the same code a few times before obtaining a reliable code.\n");

	while (1) {
		read(fd, &i, sizeof(i));

		t1 = t2;
		clock_gettime(CLOCK_MONOTONIC, &t2);
		long double e = t2.tv_sec - t1.tv_sec + (t2.tv_nsec - t1.tv_nsec) / 1000000000.0L;
		if (e > 1 && c > 0) {
			printf("\nRead %d code(s).\n\n", c);
			c = 0;
		}

		++c;

		if (i > 4096)
			i -= 0xffffff;

		printf("%d ", i);
	}

	close(fd);

	return 0;
}

static int write_codes()
{
	int fd = open("/dev/lirc0", O_WRONLY);
	if (!fd) {
		fprintf(stderr, "Error: open() failed.\n");
		return 1;
	}

	int l;
	char buf[4096];
	l = read(STDIN_FILENO, buf, sizeof(buf));
	if (l == sizeof(buf)) {
		fprintf(stderr, "Error: input limited to 4K.\n");
		goto quit;
	}

	int i;
	char *t;
	int codes[1024];
	for (i = 0, t = strtok(buf, " "); i < sizeof(codes) / sizeof(int) && t; i++, t = strtok(0, " "))
		codes[i] = atoi(t);

	l = write(fd, &codes[0], i * sizeof(int));
	printf("Sent %d code(s) using %d byte(s).\n", i, l);

quit:
	close(fd);

	return 0;
}

static void print_usage(char *argv0)
{
	fprintf(stderr, "Usage: %s [rx|tx]\n", argv0);
}

int main(int argc, char *argv[])
{
	if (argc == 2) {
		if (strcmp(argv[1], "rx") == 0)
			return read_codes();
		else if (strcmp(argv[1], "tx") == 0)
			return write_codes();
	}

	print_usage(argv[0]);

	return 1;
}
