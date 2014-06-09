all:
	gcc -Wall -Ofast -mfpu=vfp -mfloat-abi=hard -march=armv6zk -mtune=arm1176jzf-s -mcpu=arm1176jzf-s ir.c -o ir

clean:
	rm -f *.o ir
