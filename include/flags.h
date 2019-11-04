#ifndef __FLAGS_H__
#define __FLAGS_H__

#include "globals.h"
#include "write_ram.h"

uint8_t check_flags(ram_t *ram, uint8_t pc);
uint8_t calc_neg(cell_val_t a);
cell_val_t calc_abs(cell_val_t a);
uint8_t calc_overflow(cell_val_t a, cell_val_t b);
uint8_t calc_subcarry(cell_val_t a, cell_val_t b);
uint8_t calc_carry(cell_val_t a, cell_val_t b);

#endif