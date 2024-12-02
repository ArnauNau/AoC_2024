# Capture additional arguments passed to make
ARGS = $(filter-out $@,$(MAKECMDGOALS))

FOLDER ?= src

# make main
main:
	@echo "Compiling main.c in $(FOLDER)"
	gcc -std=c2x -Wall -Wextra -Werror -Wpedantic -g -O0 -fsanitize=undefined -lpthread $(FOLDER)/main.c -o main

# make clean
clean:
	rm -f main

# make val
val:
	valgrind -s --read-var-info=yes --leak-check=full --show-leak-kinds=all --track-origins=yes --trace-children=yes --track-fds=yes ./main $(ARGS)

# make run
run:
	./main $(ARGS)