#ifndef __FLAGS_C__
#define __FLAGS_C__

#include "globals.h"
#include <string.h>

uint8_t check_flags(ram_t ram, uint8_t pc)
{
	op_t op;
	op.data=ram[pc];
	flags_t flags;
	flags.data=ram[RAM_FLAGS];
	uint8_t err=!(op.err ^ flags.err);
	uint8_t ifs=1;
	switch(op.ifn << 1 | op.ifz)
	{
		case if_no:
			ifs=1;
			break;
		case if_eq:
			ifs=flags.zero;
			break;
		case if_lt:
			ifs=flags.neg;
			break;
		case if_le:
			ifs=flags.zero || flags.neg;
			break;
	}
	return err && ifs;
}

uint8_t calc_overflow(cell_val_t a, cell_val_t b)
{
	int bitsize=1<<(sizeof(cell_val_t)*8-1);
	return !((a&bitsize)^(b&bitsize)) && ((a&~bitsize)+(b&~bitsize))&bitsize;
}

uint8_t calc_neg(cell_val_t a)
{
	int bitsize=1<<(sizeof(cell_val_t)*8-1);
	return (a&bitsize);
}

#endif