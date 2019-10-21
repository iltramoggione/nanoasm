#include "globals.h"
#include "compile.c"
#include "execute.c"

int main(int argc, char **argv)
{
	ram_t ram;
	memset(ram,0,256);
	/**
	//load program
	//jmp start = mov 255 '1'
	//var i=3
	//start: dec i = add 3 '255'
	//jmp>0 start = mov 255 '1' if gt
	pc=0;
	pc=set_op(ram,pc,op_mov,255,1,1,0,0,0,0,0);
	pc=set_val(ram,pc,3);
	pc=set_op(ram,pc,op_add,3,255,1,1,0,0,0,0);
	pc=set_op(ram,pc,op_mov,255,1,1,0,0,1,0,0);
	pc=set_op(ram,pc,op_mov,3,128,1,0,0,0,0,0);
	pc=set_op(ram,pc,op_mov,255,10,1,0,0,0,0,0);
	/**
	compile(ram,
	"mov 255 '1\n"
	"var '5\n"
	"add 3 '255 f\n"
	"mov 255 '1 >\n"
	"mov 255 '250\n"
	"mov 255 '247 e\n");
	/**
	compile(ram,
	"jmp '1\n"
	"var '5\n"
	"dec 3 f\n"
	"jmp '10 <=\n"
	"jmp '1\n"
	"hlt\n"
	"hlt e\n");
	"mov 255 '247 e\n");
	/**/
	compile(ram,
	"jmp &2\n"
	"var '3\n"
	"dec $1 f\n"
	"dec *$1\n"
	"jmp &6 <=\n"
	"jmp &2\n"
	"hlt\n"
	"hlt e\n");
	execute(ram,100);
	return 0;
}