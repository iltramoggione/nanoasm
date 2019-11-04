#ifndef __NANOASM_H__
#define __NANOASM_H__

#include "globals.h"
#include "compile.h"
#include "execute.h"
#include "io.h"

void compile_execute(char *code, int max_op, channel_t **port);
int main(int argc, char **argv);

#endif