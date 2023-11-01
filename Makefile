TARGET =spacers
SRC_DIR =src
CC =g++
CFLAGS =-Wall -Wextra -std=c++17
LIBS =-lSDL2

.PHONY: default all clean

default: $(TARGET)
all: default

debug: CFLAGS += -DDEBUG_MODE -g
debug: default

OBJECTS =$(patsubst %.cpp, %.o, $(wildcard $(SRC_DIR)/*.cpp))
HEADERS =$(wildcard $(SRC_DIR)/*.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -Wall $(LIBS) -o $@

clean:
	-rm -f $(SRC_DIR)/*.o
	-rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

install: all
	cp $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)
