CC = g++
CFLAGS = -std=c++11 -Wall
LDFLAGS = -ljpeg

SOURCES = main.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE = main

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(EXECUTABLE) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(EXECUTABLE) $(OBJECTS)