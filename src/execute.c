#ifndef __EXECUTE_C__
#define __EXECUTE_C__

#include "../include/execute.h"

void execute(ram_t *ram, int max_op, char *code, int *code_ops, int code_ops_size)
{
	cell_addr_t pc=-OP_SIZE;
	cell_val_t parg1, parg2, arg1, arg2;
	uint8_t err;
	flags_t flags;
	op_t op;
	for(int i=0;;i++)
	{
		show_ram(ram,1,1,0);
		show_flags(ram);
		pc=read_ram(ram,RAM_PC);
		write_ram(ram,RAM_PC,pc+OP_SIZE);
		if(pc>=RAM_PC_HLT) break;
		if(i>=max_op && max_op!=0)
		{
			DEBUG("aborted execution\n");
			break;
		}
		show_op_code(ram,pc,code,code_ops,code_ops_size);
		op.data=read_ram(ram,pc);
		flags.data=read_ram(ram,RAM_FLAGS);
		parg1=read_ram(ram,pc+OP_ARG_1);
		parg2=read_ram(ram,pc+OP_ARG_2);
		if(op.arg==arg_ptr_cell)
		{
			arg1=read_ram(ram,parg1);
			parg1=arg1;
		}
		if(op.arg==arg_cell_lit)
		{
			arg2=parg2;
		}
		else if(op.arg==arg_cell_ptr)
		{
			parg2=read_ram(ram,parg2);
			arg2=read_ram(ram,parg2);
		}
		else
		{
			arg2=read_ram(ram,parg2);
		}
		if(check_flags(ram,pc))
		{
			err=0;
			switch(op.op)
			{
				case op_mov:
					write_ram(ram,parg1,arg2);
					break;
				case op_add:
					arg1=read_ram(ram,parg1);
					if(calc_carry(arg1,arg2)) err=1;
					write_ram(ram,parg1,arg1+arg2);
					break;
				case op_nor:
					arg1=read_ram(ram,parg1);
					write_ram(ram,parg1,~(arg1|arg2));
					break;
				case op_and:
					arg1=read_ram(ram,parg1);
					write_ram(ram,parg1,arg1&arg2);
					break;
			}
			if(op.set)
			{
				flags.zero=read_ram(ram,parg1)==0;
				flags.neg=calc_neg(read_ram(ram,parg1));
				flags.err=err;
				write_ram(ram,RAM_FLAGS,flags.data);
			}
		}
	}
}

#endif