#ifndef __SHOW_H__
#define __SHOW_H__

#include "globals.h"
#include "flags.h"

void show_line(char *str, int pos);
void show_cell(ram_t *ram, cell_addr_t addr);
void show_channel(channel_t *channel);
void show_ram(ram_t *ram, int show_all, int show_addr);
void show_op(ram_t *ram, cell_addr_t pc);
void show_flags(ram_t *ram);
void show_val(val_t val);
void show_ops(op_def_t *ops, int ops_size);
void show_refs(cell_addr_t *ref, int ref_size);
void show_op_code(ram_t *ram, cell_addr_t pc, char *code, int *code_ops, int code_ops_size);

#endif