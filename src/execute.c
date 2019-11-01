#ifndef __EXECUTE_C__
#define __EXECUTE_C__

#include "globals.h"
#include "flags.c"

void execute(ram_t ram, int max_op, char *code, int *code_ops, int code_ops_size)
{
	cell_addr_t pc=-OP_SIZE;
	cell_val_t parg1, parg2, arg1, arg2;
	uint8_t err;
	flags_t flags;
	op_t op;
	for(int i=0;;i++)
	{
		show_ram(ram,1,1);
		show_flags(ram);
		pc=ram[RAM_PC];
		ram[RAM_PC]+=OP_SIZE;
		if(pc>=RAM_PC_HLT) break;
		if(i>=max_op && max_op!=0)
		{
			printf("aborted execution\n");
			break;
		}
		show_op_code(ram,pc,code,code_ops,code_ops_size);
		op.data=ram[pc];
		flags.data=ram[RAM_FLAGS];
		parg1=ram[(cell_addr_t)(pc+OP_ARG_1)];
		parg2=ram[(cell_addr_t)(pc+OP_ARG_2)];
		arg1=ram[parg1];
		if(op.ptr)
		{
			parg1=arg1;
			arg1=ram[parg1];
		}
		if(op.val)
		{
			arg2=parg2;
		}
		else
		{
			arg2=ram[parg2];
		}
		if(check_flags(ram,pc))
		{
			err=0;
			switch(op.op)
			{
				case op_mov:
					ram[parg1]=arg2;
					break;
				case op_add:
					if(calc_carry(arg1,arg2)) err=1;
					ram[parg1]=arg1+arg2;
					break;
				case op_nor:
					ram[parg1]=~(arg1|arg2);
					break;
				case op_and:
					ram[parg1]=arg1&arg2;
					break;
			}
			if(op.set)
			{
				flags.zero = ram[parg1]==0;
				flags.neg = calc_neg(ram[parg1]);
				flags.err=err;
				ram[RAM_FLAGS]=flags.data;
			}
		}
	}
}

#endif