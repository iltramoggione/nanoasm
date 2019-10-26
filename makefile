.PHONY: all clean log

all:
	gcc -Wall -Wno-comment src/nanoasm.c -o nanoasm.exe

clean:
	-rm nanoasm.exe

log:
	./nanoasm.exe | tee log.txt

alllog: all log