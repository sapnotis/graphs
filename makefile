C = g++
# CFLAGS = -Wall -Wextra -Wpedantic -Werror
SFMLFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

all: clean main

main: main.o board.o combinator.o graph-components.o tools.o
	$(C) main.o board.o combinator.o graph-components.o tools.o -o main $(OPT) $(CFLAGS) $(SFMLFLAGS)

%.o: %.cpp
	$(C) -c $< -o $@ $(OPT) $(CFLAGS) $(SFMLFLAGS) $(CFLAGS)

clean:
	rm -f *.o main

run:
	./main

.PHONY: all clean run