#ifndef __SHOW_C__
#define __SHOW_C__

#include "../include/show.h"

void show_line(char *str, int pos)
{
	for(int i=pos;i<strlen(str);i++)
	{
		if(str[i]=='\n') break;
		fprintf(STDDEBUG,"%c",str[i]);
	}
}

void show_cell(ram_t *ram, cell_addr_t addr)
{
	fprintf(STDDEBUG,"" PRINTF_HEX "(" PRINTF_INT ") ",read_ram_raw(ram,addr),read_ram_raw(ram,addr));
}

void show_channel(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	fprintf(STDDEBUG,"data=%d, ",channel->data);
	fprintf(STDDEBUG,"has_data=%d, ",channel->has_data);
	fprintf(STDDEBUG,"closed=%d\n",channel->closed);
	pthread_mutex_unlock(&channel->mutex);
}

void show_ram(ram_t *ram, int show_all, int show_addr, int show_channels)
{
	for(int i=0,lasti=-1;i<RAM_SIZE;i++)
	{
		if(read_ram_raw(ram,i) || i<16 || i>=RAM_FLAGS || show_all)
		{
			if(i!=lasti || i%16==0 || show_addr) fprintf(STDDEBUG,"&" PRINTF_HEX ": ",i);
			show_cell(ram,i);
			lasti=i+1;
		}
	}
	fprintf(STDDEBUG,"\n");
	for(int i=0;i<4 && show_channels;i++)
	{
		fprintf(STDDEBUG,"channel %d: ",i);
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
	fprintf(STDDEBUG,"@" PRINTF_HEX "(" PRINTF_INT ") op:",pc,pc);
	uint8_t parg1=read_ram_raw(ram,pc+OP_ARG_1);
	uint8_t parg2=read_ram_raw(ram,pc+OP_ARG_2);
	uint8_t arg1=read_ram_raw(ram,parg1);
	uint8_t arg2;
	fprintf(STDDEBUG," %s",op_name);
	if(op.arg==arg_ptr_cell)
	{
		arg1=read_ram_raw(ram,arg1);
		fprintf(STDDEBUG," *[" PRINTF_HEX "]=[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg1,read_ram_raw(ram,parg1),arg1,arg1);
	}
	else
	{
		fprintf(STDDEBUG," [" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg1,arg1,arg1);
	}
	if(op.arg==arg_cell_lit)
	{
		arg2=parg2;
		fprintf(STDDEBUG," \'" PRINTF_HEX "(" PRINTF_INT ")\'",parg2,parg2);
	}
	else if(op.arg==arg_cell_ptr)
	{
		parg2=read_ram_raw(ram,parg2);
		arg2=read_ram_raw(ram,parg2);
		fprintf(STDDEBUG," *[" PRINTF_HEX "]=[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg2,read_ram_raw(ram,parg2),arg2,arg2);
	}
	else
	{
		arg2=read_ram_raw(ram,parg2);
		fprintf(STDDEBUG," [" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ")",parg2,arg2,arg2);
	}
	if(op.err) fprintf(STDDEBUG," if err");
	switch(op.ifn << 1 | op.ifz)
	{
		case if_eq:
			fprintf(STDDEBUG," if ==0");
			break;
		case if_lt:
			fprintf(STDDEBUG," if <0");
			break;
		case if_le:
			fprintf(STDDEBUG," if <=0");
			break;
	}
	if(check_flags(ram,pc))
	{
		fprintf(STDDEBUG," flags ok");
	}
	else
	{
		fprintf(STDDEBUG," flags not ok");
	}
	fprintf(STDDEBUG," : " PRINTF_HEX " (",op.data);
	fprintf(STDDEBUG,"op:" PRINTF_BITS ", ",op_op);
	fprintf(STDDEBUG,"arg:" PRINTF_BITS ", ",op_arg);
	fprintf(STDDEBUG,"set:" PRINTF_BITS ", ",op_set);
	fprintf(STDDEBUG,"err:" PRINTF_BITS ", ",op_err);
	fprintf(STDDEBUG,"ifn:" PRINTF_BITS ", ",op_ifn);
	fprintf(STDDEBUG,"ifz:" PRINTF_BITS ")\n",op_ifz);
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
	fprintf(STDDEBUG,"flags: " PRINTF_HEX " (",flags.data);
	fprintf(STDDEBUG,"port0:" PRINTF_BITS ", ",flags_port0);
	fprintf(STDDEBUG,"port1:" PRINTF_BITS ", ",flags_port1);
	fprintf(STDDEBUG,"port2:" PRINTF_BITS ", ",flags_port2);
	fprintf(STDDEBUG,"port3:" PRINTF_BITS ", ",flags_port3);
	fprintf(STDDEBUG,"unused:" PRINTF_BITS ", ",flags_unused);
	fprintf(STDDEBUG,"neg:" PRINTF_BITS ", ",flags_neg);
	fprintf(STDDEBUG,"zero:" PRINTF_BITS ", ",flags_zero);
	fprintf(STDDEBUG,"err:" PRINTF_BITS ")\n",flags_err);
}

void show_val(val_t val)
{
	uint8_t val_arg=val.arg;
	uint8_t val_lit=val.lit;
	uint8_t val_ptr=val.ptr;
	uint8_t val_ref=val.ref;
	fprintf(STDDEBUG,"val: " PRINTF_HEX "(" PRINTF_INT ") (",val.val,val.val);
	fprintf(STDDEBUG,"arg:" PRINTF_BITS ", ",val_arg);
	fprintf(STDDEBUG,"lit:" PRINTF_BITS ", ",val_lit);
	fprintf(STDDEBUG,"ptr:" PRINTF_BITS ", ",val_ptr);
	fprintf(STDDEBUG,"ref:" PRINTF_BITS ")\n",val_ref);
}

void show_ops(op_def_t *ops, int ops_size)
{	
	fprintf(STDDEBUG,"ops: %d\n",ops_size);
	for(int i=0;i<ops_size;i++)
	{
		fprintf(STDDEBUG,"%d(%s): (",i,ops[i].name);
		fprintf(STDDEBUG,"nargs:%hhu, ",ops[i].nargs);
		fprintf(STDDEBUG,"args_size:%hhu, ",ops[i].args_size);
		fprintf(STDDEBUG,"ops_size:%hhu, ",ops[i].ops_size);
		fprintf(STDDEBUG,"flagsetter:%hhu, ",ops[i].flagsetter);
		fprintf(STDDEBUG,"ops:[");
		for(int j=0;j<ops[i].ops_size;j++)
		{
			fprintf(STDDEBUG,"%d,",ops[i].ops[j]);
		}
		fprintf(STDDEBUG,"], ");
		fprintf(STDDEBUG,"args:[\n");
		for(int j=0;j<ops[i].args_size;j++)
		{
			fprintf(STDDEBUG,"\t");
			show_val(ops[i].args[j]);
		}
		fprintf(STDDEBUG,"])\n");
	}
}

void show_refs(cell_addr_t *ref, int ref_size)
{	
	fprintf(STDDEBUG,"refs: %d\n",ref_size);
	for(int i=0;i<ref_size;i++)
	{
		fprintf(STDDEBUG,"%d: [" PRINTF_HEX "]\n",i,ref[i]);
	}
}

void show_op_code(ram_t *ram, cell_addr_t pc, char *code, int *code_ops, int code_ops_size)
{
	if(pc>code_ops_size || code_ops[pc]<0)
	{
		fprintf(STDDEBUG,"no code\n");
	}
	else
	{
		fprintf(STDDEBUG,"code: ");
		show_line(code,code_ops[pc]);
		fprintf(STDDEBUG,"\n");
	}
	show_op(ram,pc);
}

#endif