#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/select.h>
#include <pthread.h>
#include <time.h>

#define debug 0

#define RAM_SIZE 256
#define RAM_PC (RAM_SIZE - 1)
#define RAM_FLAGS (RAM_SIZE - 2)
#define RAM_PORT0 (RAM_SIZE - 4)
#define RAM_PORT1 (RAM_SIZE - 3)
#define RAM_PORT2 (RAM_SIZE - 4)
#define RAM_PORT3 (RAM_SIZE - 3)
#define RAM_PORT0_BIT 7
#define RAM_PORT1_BIT 6
#define RAM_PORT2_BIT 5
#define RAM_PORT3_BIT 4
#define RAM_UNUSED_BIT 3
#define RAM_NEG_BIT 2
#define RAM_ZERO_BIT 1
#define RAM_ERR_BIT 0
#define RAM_PC_HLT (RAM_SIZE - 4)
#define OP_SIZE 3
#define OP_ARG_1 1
#define OP_ARG_2 2

#define PRINTF_HEX "%02hhx"
#define PRINTF_INT "%3hhu"
#define PRINTF_BITS "%hhu"

#define OP_END 255
#define OP_VAR 254

//#define STDDEBUG stdout
#define STDDEBUG stderr
#define DEBUG_PRINT 0
#define DEBUG(...) do{if(DEBUG_PRINT){fprintf(STDDEBUG,__VA_ARGS__);}}while(0)

typedef uint8_t cell_t;
typedef uint8_t cell_val_t;
typedef uint8_t cell_addr_t;
typedef uint8_t cell_op_t;

typedef struct{
	cell_t data;
	int has_data;
	int closed;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} channel_t;

typedef struct{
	cell_t data[RAM_SIZE];
	channel_t *port[4];
} ram_t;

typedef union{
	cell_t data;
	struct{
		uint8_t err:1;
		uint8_t zero:1;
		uint8_t neg:1;
		uint8_t unused:1;
		uint8_t port3:1;
		uint8_t port2:1;
		uint8_t port1:1;
		uint8_t port0:1;
	};
} flags_t;

typedef union{
	cell_op_t data;
	struct{
		uint8_t ifz:1;
		uint8_t ifn:1;
		uint8_t err:1;
		uint8_t set:1;
		uint8_t arg:2;
		uint8_t op:2;
	};
} op_t;

typedef struct{
	cell_val_t val;
	uint8_t arg:1;
	uint8_t lit:1;
	uint8_t ptr:1;
	uint8_t ref:1;
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
	arg_cell_cell,
	arg_cell_lit,
	arg_ptr_cell,
	arg_cell_ptr,
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
	uint8_t args_size;
	uint8_t ops_size;
	uint8_t flagsetter;
	int ops[8];
	val_t args[16];
} op_def_t;

typedef struct{
	pthread_t thread;
	channel_t *channel;
	int (*function)(void *arg, uint8_t *data);
	void *arg;
} thread_channel_t;

typedef struct{
	int step;
	int op;
	uint32_t addr;
	uint8_t data;
	channel_t *channel;
	FILE *f;
} disk_t;

#define max(a,b) ((a) > (b) ? (a) : (b))

#endif