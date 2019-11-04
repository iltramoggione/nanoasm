#ifndef __STRING_H__
#define __STRING_H__

#include "globals.h"

int is_space(char *str, int pos);
int is_not_space(char *str, int pos);
int trim(char *str, int pos);
int get_word(char *str, int pos);
int skip(char *str, int pos);
cell_op_t get_op(op_def_t *ops, int ops_size, char *str, int pos);
int get_digit(char *str, int pos);
val_t get_val(char *str, int pos);
op_flags_t get_flags(char *str, int pos);

#endif