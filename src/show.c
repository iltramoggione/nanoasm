#ifndef __SHOW_C__
#define __SHOW_C__

#include "../include/show.h"

void show_line(char *str, int pos)
{
	for(int i=pos;i<strlen(str);i++)
	{
		if(str[i]=='\n') break;
		DEBUG("%c",str[i]);
	}
}

void show_cell(ram_t *ram, cell_addr_t addr)
{
	DEBUG("" PRINTF_HEX "(" PRINTF_INT ") ",read_ram_raw(ram,addr),read_ram_raw(ram,addr));
}

void show_channel(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	DEBUG("data=%d, ",channel->data);
	DEBUG("has_data=%d, ",channel->has_data);
	DEBUG("closed=%d\n",channel->closed);
	pthread_mutex_unlock(&channel->mutex);
}

void show_ram(ram_t *ram, int show_all, int show_addr, int show_channels)
{
	for(int i=0,lasti=-1;i<RAM_SIZE;i++)
	{
		if(read_ram_raw(ram,i) || i<16 || i>=RAM_FLAGS || show_all)
		{
			if(i!=lasti || i%16==0 || show_addr) DEBUG("&" PRINTF_HEX ": ",i);
			show_cell(ram,i);
			lasti=i+1;
		}
	}
	DEBUG("\n");
	for(int i=0;i<4 && show_channels;i++)
	{
		DEBUG("channel %d: ",i);
		show_channel(ram->port[i]);
	}
}

void show_op(ram_t *ram, cell_addr_t pc)
{
	op_t op;
	op.data=read_ram_raw(ram,pc);
	uint8_t op_op=op.op;
	uint8_t op_arg=op.arg;
	uint8_t op_set=op.set;
	uint8_t op_err=op.err;
	uint8_t op_ifn=op.ifn;
	uint8_t op_ifz=op.ifz;
	char op_name[4]="";
	switch(op.op)
	{
		case op_mov:
			strcpy(op_name,"mov");
			break;
		case op_add:
			strcpy(op_name,"add");
			break;
		case op_nor:
			strcpy(op_name,"nor");
			break;
		case op_and:
			strcpy(op_name,"and");
			break;
	}
	DEBUG("@" PRINTF_HEX "(" PRINTF_INT ") op:",pc,pc);
	uint8_t parg1=read_ram_raw(ram,pc+OP_ARG_1);
	uint8_t parg2=read_ram_raw(ram,pc+OP_ARG_2);
	uint8_t arg1=read_ram_raw(ram,parg1);
	uint8_t arg2;
	DEBUG(" %s",op_name);
	if(op.arg==arg_ptr_cell)
	{
		arg1=read_ram_raw(ram,arg1);
		DEBUG(" *[" PRINTF_HEX "]=[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg1,read_ram_raw(ram,parg1),arg1,arg1);
	}
	else
	{
		DEBUG(" [" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg1,arg1,arg1);
	}
	if(op.arg==arg_cell_lit)
	{
		arg2=parg2;
		DEBUG(" \'" PRINTF_HEX "(" PRINTF_INT ")\'",parg2,parg2);
	}
	else if(op.arg==arg_cell_ptr)
	{
		parg2=read_ram_raw(ram,parg2);
		arg2=read_ram_raw(ram,parg2);
		DEBUG(" *[" PRINTF_HEX "]=[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg2,read_ram_raw(ram,parg2),arg2,arg2);
	}
	else
	{
		arg2=read_ram_raw(ram,parg2);
		DEBUG(" [" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg2,arg2,arg2);
	}
	if(op.err) DEBUG(" if err");
	switch(op.ifn << 1 | op.ifz)
	{
		case if_eq:
			DEBUG(" if ==0");
			break;
		case if_lt:
			DEBUG(" if <0");
			break;
		case if_le:
			DEBUG(" if <=0");
			break;
	}
	if(check_flags(ram,pc))
	{
		DEBUG(" flags ok");
	}
	else
	{
		DEBUG(" flags not ok");
	}
	DEBUG(" : " PRINTF_HEX " (",op.data);
	DEBUG("op:" PRINTF_BITS ", ",op_op);
	DEBUG("arg:" PRINTF_BITS ", ",op_arg);
	DEBUG("set:" PRINTF_BITS ", ",op_set);
	DEBUG("err:" PRINTF_BITS ", ",op_err);
	DEBUG("ifn:" PRINTF_BITS ", ",op_ifn);
	DEBUG("ifz:" PRINTF_BITS ")\n",op_ifz);
}

void show_flags(ram_t *ram)
{
	flags_t flags;
	flags.data=read_ram_raw(ram,RAM_FLAGS);
	uint8_t flags_neg=flags.neg;
	uint8_t flags_zero=flags.zero;
	uint8_t flags_err=flags.err;
	uint8_t flags_unused=flags.unused;
	uint8_t flags_port0=flags.port0;
	uint8_t flags_port1=flags.port1;
	uint8_t flags_port2=flags.port2;
	uint8_t flags_port3=flags.port3;
	DEBUG("flags: " PRINTF_HEX " (",flags.data);
	DEBUG("port0:" PRINTF_BITS ", ",flags_port0);
	DEBUG("port1:" PRINTF_BITS ", ",flags_port1);
	DEBUG("port2:" PRINTF_BITS ", ",flags_port2);
	DEBUG("port3:" PRINTF_BITS ", ",flags_port3);
	DEBUG("unused:" PRINTF_BITS ", ",flags_unused);
	DEBUG("neg:" PRINTF_BITS ", ",flags_neg);
	DEBUG("zero:" PRINTF_BITS ", ",flags_zero);
	DEBUG("err:" PRINTF_BITS ")\n",flags_err);
}

void show_val(val_t val)
{
	uint8_t val_arg=val.arg;
	uint8_t val_lit=val.lit;
	uint8_t val_ptr=val.ptr;
	uint8_t val_ref=val.ref;
	DEBUG("val: " PRINTF_HEX "(" PRINTF_INT ") (",val.val,val.val);
	DEBUG("arg:" PRINTF_BITS ", ",val_arg);
	DEBUG("lit:" PRINTF_BITS ", ",val_lit);
	DEBUG("ptr:" PRINTF_BITS ", ",val_ptr);
	DEBUG("ref:" PRINTF_BITS ")\n",val_ref);
}

void show_ops(op_def_t *ops, int ops_size)
{	
	DEBUG("ops: %d\n",ops_size);
	for(int i=0;i<ops_size;i++)
	{
		DEBUG("%d(%s): (",i,ops[i].name);
		DEBUG("nargs:%hhu, ",ops[i].nargs);
		DEBUG("args_size:%hhu, ",ops[i].args_size);
		DEBUG("ops_size:%hhu, ",ops[i].ops_size);
		DEBUG("flagsetter:%hhu, ",ops[i].flagsetter);
		DEBUG("ops:[");
		for(int j=0;j<ops[i].ops_size;j++)
		{
			DEBUG("%d,",ops[i].ops[j]);
		}
		DEBUG("], ");
		DEBUG("args:[\n");
		for(int j=0;j<ops[i].args_size;j++)
		{
			DEBUG("\t");
			show_val(ops[i].args[j]);
		}
		DEBUG("])\n");
	}
}

void show_refs(cell_addr_t *ref, int ref_size)
{	
	DEBUG("refs: %d\n",ref_size);
	for(int i=0;i<ref_size;i++)
	{
		DEBUG("%d: [" PRINTF_HEX "]\n",i,ref[i]);
	}
}

void show_op_code(ram_t *ram, cell_addr_t pc, char *code, int *code_ops, int code_ops_size)
{
	if(pc>code_ops_size || code_ops[pc]<0)
	{
		DEBUG("no code\n");
	}
	else
	{
		DEBUG("code: ");
		show_line(code,code_ops[pc]);
		DEBUG("\n");
	}
	show_op(ram,pc);
}

#endif