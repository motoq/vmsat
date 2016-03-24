CC = g++
CPPFLAGS = -g -std=c++11 -Wall -I$$VMSAT_INC -I$$SOFA_INC
LFLAGS = -L$$SOFA_LIB -lsofa_c

OBJECTS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

vmsat : $(OBJECTS)
	$(CC) $(CFLAGS) -o vmsat $(OBJECTS) $(LFLAGS)

.PHONY : clean
clean :
	rm vmsat $(OBJECTS)
