CC = gcc
CFLAGS = -Wall -Wextra -g
TARGET = server
OBJ = server.o router.o api_handlers.o

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)