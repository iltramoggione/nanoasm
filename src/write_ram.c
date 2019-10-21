#ifndef __WRITE_RAM_C__
#define __WRITE_RAM_C__

#include "globals.h"
#include "show.c"

uint8_t set_op(ram_t ram, cell_addr_t addr, uint8_t op, cell_val_t arg1, cell_val_t arg2, uint8_t ptr, uint8_t val, uint8_t set, uint8_t err, uint8_t ifn, uint8_t ifz)
{
	op_t opr;
	opr.op=op;
	opr.ptr=ptr;
	opr.val=val;
	opr.set=set;
	opr.err=err;
	opr.ifn=ifn;
	opr.ifz=ifz;
	ram[addr]=opr.data;
	ram[(cell_addr_t)(addr+1)]=arg1;
	ram[(cell_addr_t)(addr+2)]=arg2;
	show_op(ram,addr);
	return addr+OP_SIZE;
}

uint8_t set_val(ram_t ram, cell_addr_t addr, cell_val_t val)
{
	ram[addr]=val;
	printf("value: ");
	show_cell(ram,addr);
	printf("\n");
	return addr+1;
}

#endif