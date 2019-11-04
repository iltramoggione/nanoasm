#ifndef __COMPILE_H__
#define __COMPILE_H__

#include "globals.h"
#include "show.h"
#include "string.h"
#include "write_ram.h"

void op_def_new(op_def_t *ops, int *ops_size, char name[16], char *str);
op_def_t op_def_copy(op_def_t op);
int op_id(op_def_t *ops, int ops_size, char name[16]);
int op_size_id(op_def_t *ops, int ops_size, int op);
int op_size(op_def_t *ops, int ops_size, char name[16]);
void calc_ref(ram_t *ram, cell_addr_t pc, char *str, op_def_t *ops, int ops_size, cell_addr_t *ref, int *ref_size);
void compile_op(ram_t *ram, cell_addr_t *pc, op_def_t *ops, int ops_size, int op, val_t args[16], op_flags_t flags, int flagsetter, int *code_ops, int *code_ops_size, int op_pos);
void compile(ram_t *ram, cell_addr_t *pc, char *str, int *code_ops, int *code_ops_size);

#endif