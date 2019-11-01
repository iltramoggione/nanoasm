#include "globals.h"
#include "compile.c"
#include "execute.c"

int main(int argc, char **argv)
{
	ram_t ram;
	cell_addr_t pc;
	int code_ops[256];
	int code_ops_size;
	memset(ram,0,RAM_SIZE);
	memset(code_ops,-1,sizeof(code_ops));
	char code[4096];
	strcpy(code,
	/*00*/"jmp '&6\n"//to start2
	/*01*/"var '&48\n"//pc //to prog
	/*02*/"var '&67\n"//ptr //to data
	/*03*/"var '0\n"//paren cnt
	/*04*/"var '0\n"//paren check dir
	/*05*/"inc '&1\n"//start
	/*06*/"cmx '*&1 '-c+ 'c+ f\n"//start2
	/*07*/"jmp '&22 =\n"//to +
	/*08*/"cmx '*&1 '-c- 'c- f\n"
	/*09*/"jmp '&24 =\n"//to -
	/*10*/"cmx '*&1 '-c> 'c> f\n"
	/*11*/"jmp '&26 =\n"//to >
	/*12*/"cmx '*&1 '-c< 'c< f\n"
	/*13*/"jmp '&28 =\n"//to <
	/*14*/"cmx '*&1 '-c[ 'c[ f\n"
	/*15*/"jmp '&30 =\n"//to [
	/*16*/"cmx '*&1 '-c] 'c] f\n"
	/*17*/"jmp '&34 =\n"//to ]
	/*18*/"chk '*&1 f\n"
	/*19*/"jmp '&21 =\n"//to end
	/*20*/"jmp '&5 =\n"//to start
	/*21*/"hlt\n"//end
	/*22*/"inc '*&2\n"//+
	/*23*/"jmp '&5\n"//to start
	/*24*/"dec '*&2\n"//-
	/*25*/"jmp '&5\n"//to start
	/*26*/"inc '&2\n"//>
	/*27*/"jmp '&5\n"//to start
	/*28*/"dec '&2\n"//<
	/*29*/"jmp '&5\n"//to start
	/*30*/"chk '*&2 f\n"//[
	/*31*/"mov '&4 '1\n"
	/*32*/"jmp '&38 =\n"//to paren check
	/*33*/"jmp '&5\n"//to start
	/*34*/"chk '*&2 f\n"//]
	/*35*/"jmp '&5 =\n"//to start
	/*36*/"mov '&4 '-1\n"
	/*37*/"jmp '&38\n"//to paren check
	/*38*/"mov '&3 '0\n"//paren check
	/*39*/"cmx '*&1 '-c[ 'c[ f\n"//paren check2
	/*40*/"add '&3 '1 =\n"
	/*41*/"cmx '*&1 '-c] 'c] f\n"
	/*42*/"add '&3 '-1 =\n"
	/*43*/"chk '&3 f\n"
	/*44*/"jmp '&5 =\n"//to start
	/*45*/"add '&1 &4\n"
	/*46*/"jmp '&39\n"//to paren check2
	/*47*/"hlt\n"
	/*48*/"var 'c+\n"//prog
	/*49*/"var 'c+\n"
	/*50*/"var 'c+\n"
	/*51*/"var 'c+\n"
	/*52*/"var 'c[\n"
	/*53*/"var 'c>\n"
	/*54*/"var 'c+\n"
	/*55*/"var 'c+\n"
	/*56*/"var 'c+\n"
	/*57*/"var 'c+\n"
	/*58*/"var 'c+\n"
	/*59*/"var 'c+\n"
	/*60*/"var 'c+\n"
	/*61*/"var 'c<\n"
	/*62*/"var 'c-\n"
	/*63*/"var 'c]\n"
	/*64*/"var 'c>\n"
	/*65*/"var 'c+\n"
	/*66*/"var '0\n"//end prog
	/*67*/"var '0\n"//data
	/*68*/"var '0\n"
	/*69*/"var '99\n"//for ctrl+f
	);
	printf("code: %s\n",code);
	compile(ram,&pc,code,code_ops,&code_ops_size);
	execute(ram,10000,code,code_ops,code_ops_size);
	return 0;
}