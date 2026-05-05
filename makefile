C = g++
CFLAGS = -Wall -Wextra -Wpedantic -Werror
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

help:
	@echo "Make command runs g++ compiler"
	@echo "Requires SFML (dynamic linking)"
	@echo "Use make (all) / make clean / make run"
	@echo "Use OPT=-O2 (or big graphs will lag horribly)"

.PHONY: all clean run help