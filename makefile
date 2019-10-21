.PHONY: clean

build:
	gcc -Wall -Wno-comment src/nanoasm.c -o nanoasm.exe

clean:
	-rm nanoasm.exe