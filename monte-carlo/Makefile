CC=cc
CFLAGS= -g -o -lm -std=c99 -Wall -Werror -pthread 

TARGET1=pi_darts
OBJECTS1 = pi_darts.o

all : $(TARGET1)

$(TARGET1): $(OBJECTS1)
	$(CC) $(CFLAGS) -o $(TARGET1) $(OBJECTS1)

clean:
	@rm -rf *.o *.s a.out $(TARGET1) $(OBJECTS1)
