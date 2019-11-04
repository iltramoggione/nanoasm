#ifndef __EXECUTE_H__
#define __EXECUTE_H__

#include "globals.h"
#include "flags.h"

void execute(ram_t *ram, int max_op, char *code, int *code_ops, int code_ops_size);

#endif