TARGET =spacers
SRC_DIR =src
CC =g++
CFLAGS =-Wall -Wextra -Wpedantic -std=c++17
LDFLAGS =
LIBS =-lSDL2

.PHONY: default all clean debug install release

default: $(TARGET)
all: default

debug: CFLAGS += -fsanitize=address -DDEBUG_MODE -g
debug: LDFLAGS += -fsanitize=address
debug: default

release: CFLAGS += -O3
release: default

OBJECTS =$(patsubst %.cpp, %.o, $(wildcard $(SRC_DIR)/*.cpp))
HEADERS =$(wildcard $(SRC_DIR)/*.h)

%.o: %.cpp $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

.PRECIOUS: $(TARGET) $(OBJECTS)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) $(LIBS) -o $@

clean:
	-rm -f $(SRC_DIR)/*.o
	-rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET)

install: all
	cp $(TARGET) /usr/local/bin/$(TARGET)

uninstall:
	rm -f /usr/local/bin/$(TARGET)
