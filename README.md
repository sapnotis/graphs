## КОМПОЗИЦИЯ ПРОЕКТА:
1) Модель игровой доски с прямоугольными блоками
2) Граф вершин, отражающих состояние игровой доски, его визуализация
3) Исполняемый код, принимающий на вход начальное состояние поля и вызывающий окно с визуализацией графа состояний.

- Разработка доски, графа и итогового main ведётся в раздельных побочных ветках.

- Памятка по сборке мейна (потом будет makefile):

g++ *.cpp -o main
-Wall -Wextra -Wpedantic -Werror
-lsfml-graphics -lsfml-window -lsfml-system

valgrind ./main
(при участии sfml должно утекать 1,049 bytes in 3 blocks)

g++ *.cpp -o main -Wall -Wextra -Wpedantic -Werror -lsfml-graphics -lsfml-window -lsfml-system && valgrind ./main