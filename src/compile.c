#ifndef __COMPILE_C__
#define __COMPILE_C__

#include "../include/compile.h"

void op_def_new(op_def_t *ops, int *ops_size, char name[16], char *str)
{
	op_def_t op;
	strcpy(op.name,name);
	op.nargs=0;
	op.args_size=0;
	op.ops_size=0;
	op.flagsetter=0;
	int pos=0;
	int opcode=0;
	//printf("new op %s\n",name);
	for(int i=0,end=0;!end;i++)
	{
		pos=trim(str,pos);
		opcode=get_op(ops,*ops_size,str,pos);
		pos=skip(str,pos);
		//printf("subop %d\n",opcode);
		switch(opcode)
		{
			case OP_END:
			case OP_VAR:
				end=1;
				break;
			/**
			case op_mov:
			case op_add:
			case op_nor:
			case op_and:
			/**/
			default:
				op.ops[op.ops_size]=opcode;
				op.ops_size++;
				//printf("subop %d(%s)\n",opcode,ops[opcode].name);
				for(int j=0;j<ops[opcode].nargs;j++)
				{
					op.args[op.args_size]=get_val(str,pos);
					pos=skip(str,pos);
					op.args_size++;
					//printf("arg %d\n",j);
					//show_val(op.args[op.args_size-1]);
					if(op.args[op.args_size-1].arg)
					{
						op.nargs=max(op.nargs,op.args[op.args_size-1].val+1);
					}
					//printf("nargs %d\n",op.nargs);
				}
				op_flags_t flags=get_flags(str,pos);
				if(flags.set || flags.err || flags.ifn || flags.ifz)
				{
					pos=skip(str,pos);
					op.flagsetter=i;
				}
				break;
		}
	}
	ops[*ops_size]=op;
	(*ops_size)++;
}

op_def_t op_def_copy(op_def_t op)
{
	op_def_t r;
	strcpy(r.name,op.name);
	r.nargs=op.nargs;
	r.args_size=op.args_size;
	r.ops_size=op.ops_size;
	r.flagsetter=op.flagsetter;
	memcpy(r.ops,op.ops,r.ops_size*sizeof(int));
	memcpy(r.args,op.args,r.args_size*sizeof(int));
	return r;
}

int op_id(op_def_t *ops, int ops_size, char name[16])
{
	return get_op(ops,ops_size,name,0);
}

int op_size_id(op_def_t *ops, int ops_size, int op)
{
	int r=0;
	switch(op)
	{
		case OP_END: return 0;
		case OP_VAR: return 1;
		case op_mov:
		case op_add:
		case op_nor:
		case op_and:
			return OP_SIZE;
		default:
			for(int i=0;i<ops[op].ops_size;i++)
			{
				r+=op_size_id(ops,ops_size,ops[op].ops[i]);
			}
			return r;
	}
}

int op_size(op_def_t *ops, int ops_size, char name[16])
{
	return op_size_id(ops,ops_size,op_id(ops,ops_size,name));
}

void calc_ref(ram_t *ram, cell_addr_t pc, char *str, op_def_t *ops, int ops_size, cell_addr_t *ref, int *ref_size)
{
	cell_op_t op;
	op_flags_t flags;
	int pos=0;
	int i,end;
	for(i=0,end=0;!end && i<*ref_size;i++)
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
				pc+=op_size_id(ops,ops_size,op);
				break;
		}
	}
	//*ref_size=i;
}

void compile_op(ram_t *ram, cell_addr_t *pc, op_def_t *ops, int ops_size, int op, val_t args[16], op_flags_t flags, int flagsetter, int *code_ops, int *code_ops_size, int op_pos)
{
	if(op>=ops_size) return;
	//printf("op %d(%s)\n",op,ops[op].name);
	if(op<4)
	{
		code_ops[*pc]=op_pos;
		*code_ops_size=*pc;
		//printf("base op\n");
		*pc=set_op(ram,*pc,ops[op].ops[0],args[0].val,args[1].val,args[0].ptr,args[1].lit,flags.set & flagsetter,flags.err,flags.ifn,flags.ifz);
		return;
	}
	//printf("complex op\n");
	val_t unargs[16];
	memcpy(unargs,ops[op].args,sizeof(val_t)*16);
	for(int i=0;i<ops[op].args_size;i++)
	{
		//printf("unarg %d\n\t",i);
		if(unargs[i].arg) unargs[i]=args[unargs[i].val];
		//show_val(unargs[i]);
	}
	int argn=0;
	for(int i=0;i<ops[op].ops_size;i++)
	{
		val_t subargs[16];
		memcpy(subargs,unargs+argn,sizeof(val_t)*ops[ops[op].ops[i]].nargs);
		for(int j=0;j<ops[ops[op].ops[i]].nargs;j++)
		{
			//printf("subarg %d(%d)\n\t",j,argn+j);
			//show_val(subargs[j]);
		}
		argn+=ops[ops[op].ops[i]].nargs;
		//printf("sub op %d\n",i);
		compile_op(ram,pc,ops,ops_size,ops[op].ops[i],subargs,flags,ops[op].flagsetter==i,code_ops,code_ops_size,op_pos);
		//printf("/sub op %d\n",i);
	}
}

void compile(ram_t *ram, cell_addr_t *pc, char *str, int *code_ops, int *code_ops_size)
{
	op_def_t ops[256]={
		{"mov",2,2,1,0,{op_mov},{{0,1,0,0,0},{1,1,0,0,0}}},
		{"add",2,2,1,0,{op_add},{{0,1,0,0,0},{1,1,0,0,0}}},
		{"nor",2,2,1,0,{op_nor},{{0,1,0,0,0},{1,1,0,0,0}}},
		{"and",2,2,1,0,{op_and},{{0,1,0,0,0},{1,1,0,0,0}}},
	};
	int ops_size=4;
	op_def_new(ops,&ops_size,"inc","add /0 '1 f");
	op_def_new(ops,&ops_size,"dec","add /0 '255 f");
	op_def_new(ops,&ops_size,"jmp","mov 255 /0 f");
	op_def_new(ops,&ops_size,"hlt","jmp '255 f");
	op_def_new(ops,&ops_size,"rsf","and 254 /0 f");
	op_def_new(ops,&ops_size,"rse","rsf 'b11111110 f");
	op_def_new(ops,&ops_size,"rsz","rsf 'b11111101 f");
	op_def_new(ops,&ops_size,"rsn","rsf 'b11111011 f");
	op_def_new(ops,&ops_size,"not","nor /0 '0 f");
	op_def_new(ops,&ops_size,"chk","add /0 '0 f");
	op_def_new(ops,&ops_size,"neg","not /0 inc /0 f");
	op_def_new(ops,&ops_size,"sub","neg /1 add /0 /1 f neg /1");
	op_def_new(ops,&ops_size,"cmp","sub /0 /1 f add /0 /1");
	op_def_new(ops,&ops_size,"cma","add /0 /1 f sub /0 /1");
	op_def_new(ops,&ops_size,"cmx","add /0 /1 f add /0 /2");
	show_ops(ops,ops_size);
	cell_addr_t ref[256];
	int ref_size=sizeof(ref)/sizeof(cell_addr_t);
	memset(ref,0,ref_size*sizeof(cell_addr_t));
	cell_op_t op;
	int pos=0, op_pos;
	val_t val[16];
	op_flags_t flags;
	calc_ref(ram,*pc,str,ops,ops_size,ref,&ref_size);
	show_refs(ref,ref_size);
	*code_ops_size=0;
	for(int i=0,end=0;!end && i<ref_size;i++)
	{
		pos=trim(str,pos);
		op_pos=pos;
		op=get_op(ops,ops_size,str,pos);
		pos=skip(str,pos);
		switch(op)
		{
			case OP_END:
				end=1;
				break;
			case OP_VAR:
				val[0]=get_val(str,pos);
				pos=skip(str,pos);
				if(val[0].ref) val[0].val=ref[val[0].val];
				code_ops[*pc]=op_pos;
				*code_ops_size=*pc;
				*pc=set_val(ram,*pc,val[0].val);
				break;
			default:
				for(int j=0;j<ops[op].nargs;j++)
				{
					val[j]=get_val(str,pos);
					pos=skip(str,pos);
					if(val[j].ref) val[j].val=ref[val[j].val];
				}
				flags=get_flags(str,pos);
				if(flags.set || flags.err || flags.ifn || flags.ifz) pos=skip(str,pos);
				compile_op(ram,pc,ops,ops_size,op,val,flags,flags.set,code_ops,code_ops_size,op_pos);
				break;
		}
	}
}

#endif