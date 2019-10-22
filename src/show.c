#ifndef __SHOW_C__
#define __SHOW_C__

#include "globals.h"
#include <string.h>

#define PRINTF_HEX "%02hhx"
#define PRINTF_INT "%3hhu"
#define PRINTF_BITS "%hhu"

void show_line(char *str, int pos)
{
	for(int i=pos;i<strlen(str);i++)
	{
		if(str[i]=='\n') break;
		printf("%c",str[i]);
	}
}

void show_cell(ram_t ram, cell_addr_t addr)
{
	printf("" PRINTF_HEX "(" PRINTF_INT ") ",ram[addr],ram[addr]);
}

void show_ram(ram_t ram, int show_all, int show_addr)
{
	for(int i=0,lasti=-1;i<RAM_SIZE;i++)
	{
		if(ram[i] || i<16 || i>=RAM_FLAGS || show_all)
		{
			if(i!=lasti || i%16==0 || show_addr) printf("&" PRINTF_HEX ": ",i);
			show_cell(ram,i);
			lasti=i+1;
		}
	}
	printf("\n");
}

void show_op(ram_t ram, cell_addr_t pc)
{
	op_t op;
	op.data=ram[pc];
	uint8_t op_op=op.op;
	uint8_t op_ptr=op.ptr;
	uint8_t op_val=op.val;
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
	printf("@" PRINTF_HEX "(" PRINTF_INT ") op: ",pc,pc);
	uint8_t parg1=ram[(cell_addr_t)(pc+OP_ARG_1)];
	uint8_t parg2=ram[(cell_addr_t)(pc+OP_ARG_2)];
	uint8_t arg1=ram[parg1];
	uint8_t arg2;
	printf("%s ",op_name);
	if(op.ptr)
	{
		arg1=ram[arg1];
		printf("*[" PRINTF_HEX "]=[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ") ",parg1,ram[parg1],arg1,arg1);
	}
	else
	{
		printf("[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ") ",parg1,arg1,arg1);
	}
	if(op.val)
	{
		arg2=parg2;
		printf("\'" PRINTF_HEX "(" PRINTF_INT ")\' ",parg2,parg2);
	}
	else
	{
		arg2=ram[parg2];
		printf("[" PRINTF_HEX "]=" PRINTF_HEX "(" PRINTF_INT ") ",parg2,arg2,arg2);
	}
	if(op.err) printf(" if err");
	switch(op.ifn << 1 | op.ifz)
	{
		case if_eq:
			printf("if ==0");
			break;
		case if_lt:
			printf("if <0");
			break;
		case if_le:
			printf("if <=0");
			break;
	}
	printf(": " PRINTF_HEX " (",op.data);
	printf("op:" PRINTF_BITS ", ",op_op);
	printf("ptr:" PRINTF_BITS ", ",op_ptr);
	printf("val:" PRINTF_BITS ", ",op_val);
	printf("set:" PRINTF_BITS ", ",op_set);
	printf("err:" PRINTF_BITS ", ",op_err);
	printf("ifn:" PRINTF_BITS ", ",op_ifn);
	printf("ifz:" PRINTF_BITS ")\n",op_ifz);
}

void show_flags(ram_t ram)
{
	flags_t flags;
	flags.data=ram[RAM_FLAGS];
	uint8_t flags_neg=flags.neg;
	uint8_t flags_zero=flags.zero;
	uint8_t flags_err=flags.err;
	printf("flags: " PRINTF_HEX " (",flags.data);
	printf("neg:" PRINTF_BITS ", ",flags_neg);
	printf("zero:" PRINTF_BITS ", ",flags_zero);
	printf("err:" PRINTF_BITS ")\n",flags_err);
}

void show_ops(op_def_t *ops, int ops_size)
{	
	printf("ops: %d\n",ops_size);
	for(int i=0;i<ops_size;i++)
	{
		printf("%s: (",ops[i].name);
		printf("nargs:%hhu, ",ops[i].nargs);
		printf("nops:%hhu, ",ops[i].nops);
		printf("flagsetter:%hhu, ",ops[i].flagsetter);
		printf("ops:[");
		for(int j=0;j<sizeof(ops[i].ops)/sizeof(ops[i].ops[0]);j++)
		{
			printf(PRINTF_BITS ",",ops[i].ops[j]);
		}
		printf("], ");
		printf("args:%s)\n",ops[i].args);
	}
}

void show_refs(cell_addr_t *ref, int ref_size)
{	
	printf("refs: %d\n",ref_size);
	for(int i=0;i<ref_size;i++)
	{
		if(ref[i]) printf("%d: [" PRINTF_HEX "]\n",i,ref[i]);
	}
}

void show_op_code(ram_t ram, cell_addr_t pc, char *code, int *code_ops, int code_ops_size)
{
	if(pc>=code_ops_size || code_ops[pc]<0)
	{
		printf("no code\n");
	}
	else
	{
		printf("code: ");
		show_line(code,code_ops[pc]);
		printf("\n");
	}
	show_op(ram,pc);
}

#endif