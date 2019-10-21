#ifndef __COMPILE_C__
#define __COMPILE_C__

#include "globals.h"
#include "show.c"
#include "string.c"
#include "write_ram.c"

cell_addr_t calc_ref(ram_t ram, char *str, op_def_t *ops, int ops_size, cell_addr_t *ref, int ref_size)
{
	cell_addr_t pc=0;
	cell_op_t op;
	op_flags_t flags;
	int pos=0;
	for(int i=0,end=0;!end && i<ref_size;i++)
	{
		ref[i]=pc;
		pos=trim(str,pos);
		op=get_op(ops,ops_size,str,pos);
		pos=skip(str,pos);
		switch(op)
		{
			case OP_END:
				end=1;
				break;
			case OP_VAR:
				pos=skip(str,pos);
				pc+=1;
				break;
			default:
				for(int j=0;j<ops[op].nargs;j++)
				{
					pos=skip(str,pos);
				}
				flags=get_flags(str,pos);
				if(flags.set || flags.err || flags.ifn || flags.ifz) pos=skip(str,pos);
				pc+=OP_SIZE;
				break;
		}
	}
	return pc;
}

cell_addr_t compile(ram_t ram, char *str)
{
	op_def_t ops[]={
		{"mov",2,1,{op_mov,0,0,0},"&0 &1"},
		{"add",2,1,{op_add,0,0,0},"&0 &1"},
		{"nor",2,1,{op_nor,0,0,0},"&0 &1"},
		{"and",2,1,{op_and,0,0,0},"&0 &1"},
		{"inc",1,1,{op_add,0,0,0},"&0 '1"},
		{"dec",1,1,{op_add,0,0,0},"&0 '255"},
		{"jmp",1,1,{op_mov,0,0,0},"255 &0"},
		{"hlt",0,1,{op_mov,0,0,0},"255 '250"},
		{"not",1,1,{op_nor,0,0,0},"&0 '0"},
		{"chk",1,1,{op_mov,0,0,0},"&0 &0"},
		{"neg",1,2,{op_nor,op_add,0,0},"&0 '0 &0 '1"},
		{"sub",1,2,{op_nor,op_add,op_nor,op_add},"&1 '0 &0 &1 &1 '0 &0 '1"},
	};
	int ops_size=sizeof(ops)/sizeof(op_def_t);
	show_ops(ops,ops_size);
	cell_addr_t pc=0, ref[32];
	int ref_size=sizeof(ref)/sizeof(cell_addr_t);
	cell_op_t op;
	int pos=0;
	val_t val[8], valarg[2];
	op_flags_t flags;
	//printf("str: \"%s\"\n",str+pos);
	calc_ref(ram,str,ops,ops_size,ref,ref_size);
	show_refs(ref,ref_size);
	for(int i=0,end=0;!end && i<ref_size;i++)
	{
		pos=trim(str,pos);
		//printf("str get_op: \"%s\"\n",str+pos);
		op=get_op(ops,ops_size,str,pos);
		pos=skip(str,pos);
		//printf("op: \"%d\"\n",op);
		switch(op)
		{
			case OP_END:
				end=1;
				break;
			case OP_VAR:
				val[0]=get_val(str,pos);
				pos=skip(str,pos);
				pc=set_val(ram,pc,val[0].val);
				break;
			default:
				for(int j=0;j<ops[op].nargs;j++)
				{
					//printf("str get_val: \"%s\"\n",str+pos);
					val[j]=get_val(str,pos);
					pos=skip(str,pos);
				}
				//printf("str get_flags: \"%s\"\n",str+pos);
				flags=get_flags(str,pos);
				if(flags.set || flags.err || flags.ifn || flags.ifz) pos=skip(str,pos);
				int posarg=0;
				for(int j=0;j<ops[op].nops;j++)
				{
					//printf("str get_val: \"%s\"\n",str+pos);
					valarg[0]=get_val(ops[op].args,posarg);
					posarg=skip(ops[op].args,posarg);
					valarg[1]=get_val(ops[op].args,posarg);
					posarg=skip(ops[op].args,posarg);
					if(valarg[0].ref || valarg[0].pos) valarg[0]=val[valarg[0].val];
					if(valarg[1].ref || valarg[1].pos) valarg[1]=val[valarg[1].val];
					if(valarg[0].ref)
					{
						valarg[0].val=ref[valarg[0].val]-OP_SIZE;
						valarg[0].lit=1;
					}
					if(valarg[1].ref)
					{
						valarg[1].val=ref[valarg[1].val]-OP_SIZE;
						valarg[1].lit=1;
					}
					if(valarg[0].pos)
					{
						valarg[0].val=ref[valarg[0].val];
						valarg[0].lit=1;
					}
					if(valarg[1].pos)
					{
						valarg[1].val=ref[valarg[1].val];
						valarg[1].lit=1;
					}
					pc=set_op(ram,pc,ops[op].ops[j],valarg[0].val,valarg[1].val,valarg[0].ptr,valarg[1].lit,flags.set,flags.err,flags.ifn,flags.ifz);
				}
				break;
		}
	}
	return pc;
}

#endif