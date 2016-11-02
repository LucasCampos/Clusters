CC          = g++
MAKE        = make
SHELL       = /bin/sh

CFLAGS  = -std=c++11
FOLDERS =

LIBS = 
EXECS = clusterLife
SOURCES = main.cpp
HEADERS = inc/
OBJECTS = $(SOURCES:.cpp=.o)

$(EXECS): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LIBS) -O3 $(CFLAGS) -I $(HEADERS)

.cpp.o: 
	$(CC) -c $(SOURCES) $(LIBS) -O3 $(CFLAGS) -I $(HEADERS)


clean:
	/bin/rm -f *.o *.mod $(EXECS) *.gnu *.sh *.gif
run: $(EXECS)
	./$(EXECS) -c transannealingRusso.dat 384 1  -b 50 -e

install: $(EXECS)
	mv $(EXECS) /usr/bin
