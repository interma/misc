
GCC=g++
CPPFLAGS = -g -fPIC -Wall -W -Winline -Werror  -Wno-unused-parameter -Wno-unused-function

INCLUDE_PATH=-I. \

LIB_PATH=-L. -lpthread -lev

SERVER=echo_server

all : $(SERVER) output
	
$(SERVER) : \
	echo_server.o \

	$(GCC) -o $@ $^  $(INCLUDE_PATH) $(LIB_PATH) 

%.o : %.cpp
	$(GCC) $(CPPFLAGS) -c $< -o $@ $(INCLUDE_PATH)

output :
	mkdir -p output
	cp $(SERVER) output/

.PHONY: clean 
clean:
	rm -f *.o $(SERVER);
	rm -fr output;

