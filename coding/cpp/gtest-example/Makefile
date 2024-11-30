top_builddir = ../../..
include $(top_builddir)/src/Makefile.global

GOOGLE_TEST_LIB = -L../../../depends/thirdparty/googletest/build/googlemock/gtest/ -lgtest
GOOGLE_TEST_INCLUDE = -I ../../../depends/thirdparty/googletest/googletest/include/

G++ = g++
G++_FLAGS = -c -Wall -I../../../src/include $(GOOGLE_TEST_INCLUDE) -L $(CPPFLAGS)
LD_FLAGS = $(GOOGLE_TEST_LIB) -l pthread

OBJECTS = TestMain.o TestBatch.o
TARGET = unittest_main

all: $(TARGET)

$(TARGET): $(OBJECTS)
	g++ -o $(TARGET) $(OBJECTS) ../tuplebatch.o $(LD_FLAGS)

%.o : %.cpp
	$(G++) $(G++_FLAGS) $<

clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: all clean
