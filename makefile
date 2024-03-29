CC = gcc
CFLAGS = -Wall -Werror -O2	-g
INCLUDES = -I./include

SRCS = src/main.c src/ressource.c src/afficher.c src/my_string.c
OBJS = $(SRCS:.c=.o)

TARGET = my_ls

$(TARGET) : $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -c src/main.c

ressource.o: ressource.c
	$(CC) $(CFLAGS) -c src/ressource.c

afficher.o: afficher.c
	$(CC) $(CFLAGS) -c src/afficher.c

my_string.o: my_string.c
	$(CC) $(CFLAGS) -c src/my_string.c

.PHONY: clean

all : $(TARGET)

re : fclean all

clean:
	rm -f $(OBJS) $(TARGET) *.o

fclean:
	rm -f $(OBJS) $(TARGET)