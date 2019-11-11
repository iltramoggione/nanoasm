#include "../include/nanoasm.h"

void compile_execute(char *code, int max_op, channel_t **port)
{
	ram_t ram;
	cell_addr_t pc;
	int code_ops[256];
	int code_ops_size;
	memset(ram.data,0,RAM_SIZE);
	ram.port[0]=port[0];
	ram.port[1]=port[1];
	ram.port[2]=port[2];
	ram.port[3]=port[3];
	memset(code_ops,-1,sizeof(code_ops));
	compile(&ram,&pc,code,code_ops,&code_ops_size);
	execute(&ram,max_op,code,code_ops,code_ops_size);
	channel_close(ram.port[0]);
	channel_close(ram.port[1]);
	channel_close(ram.port[2]);
	channel_close(ram.port[3]);
}

int main(int argc, char **argv)
{
	//char *code=
	///*00*/"jmp '&6\n"//to start2
	///*01*/"var '&48\n"//pc //to prog
	///*02*/"var '&67\n"//ptr //to data
	///*03*/"var '0\n"//paren cnt
	///*04*/"var '0\n"//paren check dir
	///*05*/"inc '&1\n"//start
	///*06*/"cmx '*&1 '-c+ 'c+ f\n"//start2
	///*07*/"jmp '&22 =\n"//to +
	///*08*/"cmx '*&1 '-c- 'c- f\n"
	///*09*/"jmp '&24 =\n"//to -
	///*10*/"cmx '*&1 '-c> 'c> f\n"
	///*11*/"jmp '&26 =\n"//to >
	///*12*/"cmx '*&1 '-c< 'c< f\n"
	///*13*/"jmp '&28 =\n"//to <
	///*14*/"cmx '*&1 '-c[ 'c[ f\n"
	///*15*/"jmp '&30 =\n"//to [
	///*16*/"cmx '*&1 '-c] 'c] f\n"
	///*17*/"jmp '&34 =\n"//to ]
	///*18*/"chk '*&1 f\n"
	///*19*/"jmp '&21 =\n"//to end
	///*20*/"jmp '&5 =\n"//to start
	///*21*/"hlt\n"//end
	///*22*/"inc '*&2\n"//+
	///*23*/"jmp '&5\n"//to start
	///*24*/"dec '*&2\n"//-
	///*25*/"jmp '&5\n"//to start
	///*26*/"inc '&2\n"//>
	///*27*/"jmp '&5\n"//to start
	///*28*/"dec '&2\n"//<
	///*29*/"jmp '&5\n"//to start
	///*30*/"chk '*&2 f\n"//[
	///*31*/"mov '&4 '1\n"
	///*32*/"jmp '&38 =\n"//to paren check
	///*33*/"jmp '&5\n"//to start
	///*34*/"chk '*&2 f\n"//]
	///*35*/"jmp '&5 =\n"//to start
	///*36*/"mov '&4 '-1\n"
	///*37*/"jmp '&38\n"//to paren check
	///*38*/"mov '&3 '0\n"//paren check
	///*39*/"cmx '*&1 '-c[ 'c[ f\n"//paren check2
	///*40*/"add '&3 '1 =\n"
	///*41*/"cmx '*&1 '-c] 'c] f\n"
	///*42*/"add '&3 '-1 =\n"
	///*43*/"chk '&3 f\n"
	///*44*/"jmp '&5 =\n"//to start
	///*45*/"add '&1 &4\n"
	///*46*/"jmp '&39\n"//to paren check2
	///*47*/"hlt\n"
	///*48*/"var 'c+\n"//prog
	///*49*/"var 'c+\n"
	///*50*/"var 'c+\n"
	///*51*/"var 'c+\n"
	///*52*/"var 'c[\n"
	///*53*/"var 'c>\n"
	///*54*/"var 'c+\n"
	///*55*/"var 'c+\n"
	///*56*/"var 'c+\n"
	///*57*/"var 'c+\n"
	///*58*/"var 'c+\n"
	///*59*/"var 'c+\n"
	///*60*/"var 'c+\n"
	///*61*/"var 'c<\n"
	///*62*/"var 'c-\n"
	///*63*/"var 'c]\n"
	///*64*/"var 'c>\n"
	///*65*/"var 'c+\n"
	///*66*/"var '0\n"//end prog
	///*67*/"var '0\n"//data
	///*68*/"var '0\n"
	///*69*/"var '99\n"//for ctrl+f
	//;
	//char *code=
	///*00*/"add *&7 &6 f\n"// to ptr and zero
	///*01*/"jmp '&5 =\n"// to hlt
	///*02*/"mov 252 *&7\n"// to ptr
	///*03*/"inc &7\n"// to ptr
	///*04*/"jmp '&0\n"// to start
	///*05*/"hlt\n"
	///*06*/"var '0\n"// zero
	///*07*/"var '&8\n"// ptr // to data
	///*08*/"var 'cH\n"// data
	///*09*/"var 'ce\n"
	///*10*/"var 'cl\n"
	///*11*/"var 'cl\n"
	///*12*/"var 'co\n"
	///*13*/"var 'c,\n"
	///*14*/"var 'c \n"
	///*15*/"var 'cw\n"
	///*16*/"var 'co\n"
	///*17*/"var 'cr\n"
	///*18*/"var 'cl\n"
	///*19*/"var 'cd\n"
	///*20*/"var 'c!\n"
	///*21*/"var 'c\n\n"
	///*22*/"var '0\n";
	char *code=
	/*00*/"mov &6 252\n"// to data
	/*01*/"cmx &6 '-10 '10 f\n"// to data
	/*02*/"jmp '&5 =\n"// to hlt
	/*03*/"mov 252 &6\n"// to data
	/*04*/"jmp '&0\n"// to start
	/*05*/"hlt\n"
	/*06*/"var '0\n"// data
	;
	fprintf(STDDEBUG,"code: %s\n",code);
	FILE *f=fopen("test.txt","w");
	thread_channel_t *tp1=new_stdin_reader_channel();
	thread_channel_t *tp2=new_null_reader_channel();
	//thread_channel_t *tp3=new_stdout_writer_channel();
	thread_channel_t *tp3=new_file_writer_channel(f);
	thread_channel_t *tp4=new_null_writer_channel();
	channel_t *port[4]={
		tp1->channel,
		tp2->channel,
		tp3->channel,
		tp4->channel
	};
	compile_execute(code,1000,port);
	fflush(stdout);
	pthread_join(tp1->thread,NULL);
	pthread_join(tp2->thread,NULL);
	pthread_join(tp3->thread,NULL);
	pthread_join(tp4->thread,NULL);
	fclose(f);
	return 0;
}