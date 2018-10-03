CC = g++
CFFLAGS = -W -WALL
TARGET = spi_com
LINK = -lwiringPi
OBJECTS = spi_transmit.o

all : $(TARGET)

$(TARGET) : $(OBJECTS)
	$(CC) $(CFLAGS) $(LINK) -o $@ $^

clean : 
	rm *.o $(TARGET)
