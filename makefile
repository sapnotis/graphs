CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: clean main

main: main.o tools.o graph-components.o combinator.o
	$(CC) main.o tools.o graph-components.o combinator.o -o main $(OPT) $(CFLAGS) $(SFMLFLAGS)

main.o:
	$(CC) -c main.cpp -o main.o $(OPT) $(CFLAGS) $(SFMLFLAGS)

tools.o:
	$(CC) -c tools.cpp -o tools.o $(OPT) $(CFLAGS) $(SFMLFLAGS)

graph-components.o:
	$(CC) -c graph-components.cpp -o graph-components.o $(OPT) $(CFLAGS) $(SFMLFLAGS)

combinator.o:
	$(CC) -c combinator.cpp -o combinator.o $(OPT) $(CFLAGS) $(SFMLFLAGS)

clean:
	rm -f *.o main

.PHONY: all clean
