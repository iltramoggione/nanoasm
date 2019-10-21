.PHONY: build clean log

build:
	gcc -Wall -Wno-comment src/nanoasm.c -o nanoasm.exe

clean:
	-rm nanoasm.exe

log:
	./nanoasm.exe | tee log.txt