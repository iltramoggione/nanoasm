#include "globals.h"
#include "compile.c"
#include "execute.c"

int main(int argc, char **argv)
{
	ram_t ram;
	int code_ops[256];
	int code_ops_size;
	memset(ram,0,RAM_SIZE);
	memset(code_ops,-1,sizeof(code_ops));
	char code[4096];
	strcpy(code,
	/*00*/"jmp '&6\n"//to start2
	/*01*/"var '$50\n"//pc //to prog
	/*02*/"var '$69\n"//ptr //to data
	/*03*/"var '0\n"//paren cnt
	/*04*/"var '0\n"//paren check dir
	/*05*/"inc '$1\n"//start
	/*06*/"cmx '*$1 '-c+ 'c+ f\n"//start2
	/*07*/"jmp '&23 =\n"//to +
	/*08*/"cmx '*$1 '-c- 'c- f\n"
	/*09*/"jmp '&25 =\n"//to -
	/*10*/"cmx '*$1 '-c> 'c> f\n"
	/*11*/"jmp '&27 =\n"//to >
	/*12*/"cmx '*$1 '-c< 'c< f\n"
	/*13*/"jmp '&29 =\n"//to <
	/*14*/"cmx '*$1 '-c[ 'c[ f\n"
	/*15*/"jmp '&31 =\n"//to [
	/*16*/"cmx '*$1 '-c] 'c] f\n"
	/*17*/"jmp '&35 =\n"//to ]
	/*18*/"chk '*$1 f\n"
	/*19*/"jmp '&21 =\n"//to end
	/*20*/"jmp '&5 =\n"//to start
	/*21*/"hlt\n"//end
	/*22*/"hlt e\n"
	/*23*/"inc '*$2\n"//+
	/*24*/"jmp '&5\n"//to start
	/*25*/"dec '*$2\n"//-
	/*26*/"jmp '&5\n"//to start
	/*27*/"inc '$2\n"//>
	/*28*/"jmp '&5\n"//to start
	/*29*/"dec '$2\n"//<
	/*30*/"jmp '&5\n"//to start
	/*31*/"chk '*$2 f\n"//[
	/*32*/"mov '$4 '1\n"
	/*33*/"jmp '&39 =\n"//to paren check
	/*34*/"jmp '&5\n"//to start
	/*35*/"chk '*$2 f\n"//]
	/*36*/"jmp '&5 =\n"//to start
	/*37*/"mov '$4 '-1\n"
	/*38*/"jmp '&39\n"//to paren check
	/*39*/"mov '$3 '0\n"//paren check
	/*40*/"cmx '*$1 '-c[ 'c[ f\n"//paren check2
	/*41*/"add '$3 '1 =\n"
	/*42*/"cmx '*$1 '-c] 'c] f\n"
	/*43*/"add '$3 '-1 =\n"
	/*44*/"chk '$3 f\n"
	/*45*/"jmp '&5 =\n"//to start
	/*46*/"add '$1 $4\n"
	/*47*/"jmp '&40\n"//to paren check2
	/*48*/"hlt\n"
	/*49*/"hlt e\n"
	/*50*/"var 'c+\n"//prog
	/*51*/"var 'c+\n"
	/*52*/"var 'c+\n"
	/*53*/"var 'c+\n"
	/*54*/"var 'c[\n"
	/*55*/"var 'c>\n"
	/*56*/"var 'c+\n"
	/*57*/"var 'c+\n"
	/*58*/"var 'c+\n"
	/*59*/"var 'c+\n"
	/*60*/"var 'c+\n"
	/*61*/"var 'c+\n"
	/*62*/"var 'c+\n"
	/*63*/"var 'c<\n"
	/*64*/"var 'c-\n"
	/*65*/"var 'c]\n"
	/*66*/"var 'c>\n"
	/*67*/"var 'c+\n"
	/*68*/"var '0\n"//end prog
	/*69*/"var '0\n"//data
	/*70*/"var '0\n"
	/*71*/"var '99\n"//for ctrl+f
	);
	printf("code: %s\n",code);
	compile(ram,code,code_ops,&code_ops_size);
	execute(ram,10000,code,code_ops,code_ops_size);
	return 0;
}