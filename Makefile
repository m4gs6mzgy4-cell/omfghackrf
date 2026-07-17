CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = -lhackrf
SOURCES = rf_core/rf_engine.c
TARGET = rf_engine

all: $(TARGET)

$(TARGET): $(SOURCES)
    $(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LDFLAGS)

clean:
    rm -f $(TARGET)