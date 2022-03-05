.PHONY: clean
run: build
	./editor
build: clean
	gcc -Wall main.c -o editor -lm -std=c11 -ggdb3
clean:
	rm -f editor