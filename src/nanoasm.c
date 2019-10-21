#include "globals.h"
#include "compile.c"
#include "execute.c"

int main(int argc, char **argv)
{
	ram_t ram;
	memset(ram,0,RAM_SIZE);
	compile(ram,
	"jmp &2\n"
	"var '3\n"
	"add $1 '-1 f\n"
	"add *$1 '-1\n"
	"jmp &6 <=\n"
	"jmp &2\n"
	"hlt\n"
	"hlt e\n");
	execute(ram,100);
	return 0;
}