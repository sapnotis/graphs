CC = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: clean main

main: main-graph-tester.o tools.o graph-components.o
	$(CC) main-graph-tester.o tools.o graph-components.o -o main $(CFLAGS) $(SFMLFLAGS)

main-graph-tester.o:
	$(CC) -c main-graph-tester.cpp -o main-graph-tester.o $(CFLAGS) $(SFMLFLAGS)

tools.o:
	$(CC) -c tools.cpp -o tools.o $(CFLAGS) $(SFMLFLAGS)

graph-components.o:
	$(CC) -c graph-components.cpp -o graph-components.o  $(CFLAGS) $(SFMLFLAGS)

clean:
	rm -f main-graph-tester.o tools.o graph-components.o main

.PHONY: all clean graph