CC = g++

#debug levle
DEBUG = -g -ggdb -DINFO
CFLAGS = -W -Wall -Werror -fPIC $(DEBUG)

LIB_OBJS = \
		   ./ul_prime.o \
		   ./ul_sign.o

LIBRARY = libsign.so

all: $(LIBRARY)

.PHONY: clean 
clean:
	-rm *.o test
	-rm $(LIBRARY)

$(LIBRARY): $(LIB_OBJS)
	$(CC) $(CFLAGS) -pthread -fPIC -shared $(LIB_OBJS) -o $(LIBRARY) -lm

test: $(LIB_OBJS) example.o
	$(CC) $(CFLAGS) -pthread $(LIB_OBJS) example.o -o $@ -lm

