CC=cc
CFLAGS= -g -std=c99 -Wall -Werror -pthread

TARGET1=pi_series_serial
OBJECTS1=pi_series_serial.o unixtimer.o

TARGET2=pi_series_threads
OBJECTS2=pi_series_threads.o unixtimer.o


all : $(TARGET1) $(TARGET2)

$(TARGET1): $(OBJECTS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJECTS1)

$(TARGET2): $(OBJECTS2)
	$(CC) $(CFLAGS) -o $(TARGET2) $(OBJECTS2)

clean:
	@rm -rf *.o *.s a.out $(TARGET1) $(OBJECTS1) $(TARGET2) $(OBJECTS2)
