.PHONY: build buildlog clean runlog buildrunlog

build:
	gcc -Wall -Wno-comment src/nanoasm.c -o nanoasm.exe

buildlog:
	make build |& tee log.txt

clean:
	-rm nanoasm.exe

runlog:
	./nanoasm.exe |& tee log.txt

buildrunlog: build runlog