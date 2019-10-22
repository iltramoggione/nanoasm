#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdio.h>
#include <stdint.h>

#define debug 0

#define RAM_SIZE 256
#define RAM_PC (RAM_SIZE - 1)
#define RAM_FLAGS (RAM_SIZE - 2)
#define RAM_PC_HLT (RAM_SIZE - 3)
#define OP_SIZE 3
#define OP_ARG_1 1
#define OP_ARG_2 2


typedef uint8_t cell_t;
typedef uint8_t cell_val_t;
typedef uint8_t cell_addr_t;
typedef uint8_t cell_op_t;
typedef cell_t ram_t[RAM_SIZE];

typedef union{
	cell_t data;
	struct{
		uint8_t err:1;
		uint8_t zero:1;
		uint8_t neg:1;
	};
} flags_t;

typedef union{
	cell_op_t data;
	struct{
		uint8_t ifz:1;
		uint8_t ifn:1;
		uint8_t err:1;
		uint8_t set:1;
		uint8_t val:1;
		uint8_t ptr:1;
		uint8_t op:2;
	};
} op_t;

typedef struct{
	cell_val_t val;
	uint8_t lit:1;
	uint8_t ptr:1;
	uint8_t ref:1;
	uint8_t pos:1;
} val_t;

typedef struct{
	uint8_t ifz:1;
	uint8_t ifn:1;
	uint8_t err:1;
	uint8_t set:1;
} op_flags_t;

enum{
	op_mov,
	op_add,
	op_nor,
	op_and
};

enum{
	if_no,
	if_eq,
	if_lt,
	if_le,
};

typedef struct{
	char name[16];
	uint8_t nargs;
	uint8_t nops;
	uint8_t flagsetter;
	cell_op_t ops[8];
	char args[64];
} op_def_t;

#endif