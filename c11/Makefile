
CXX=g++
#CXXFLAGS = -g -fPIC -Wall -W -Winline -Werror -Wno-unused-parameter -Wno-unused-function
CXXFLAGS = -g -fPIC -std=c++11

INCLUDE_PATH=-I. \

LIB_PATH=-L. -lpthread 

EXE=test

all : $(EXE) output
	
SOURCES += $(shell find . -name '*.cpp')
OBJECTS = $(SOURCES:.cpp=.o)

$(EXE) : $(OBJECTS)
	$(CXX) -o $@ $^  $(INCLUDE_PATH) $(LIB_PATH) 

%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE_PATH)

output :
	mkdir -p output
	cp $(EXE) output/

.PHONY: clean 
clean:
	-rm -f *.o $(EXE);
	-rm -fr output;
