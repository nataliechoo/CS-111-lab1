OBJS = pipe.o

CFLAGS = -std=c17 -Wpedantic -Wall -O2 -pipe -fno-plt
#LDFLAGS = -Wl,relro,-z,now

pipe: ${OBJS}

.PHONY: clean
clean:
	rm -f ${OBJS} pipe
