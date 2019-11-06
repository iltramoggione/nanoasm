#ifndef __WRITE_RAM_H__
#define __WRITE_RAM_H__

#include "globals.h"
#include "show.h"
#include "io.h"


int port_is_input(int port);
uint8_t read_ram_raw(ram_t *ram, cell_addr_t addr);
void write_ram_raw(ram_t *ram, cell_addr_t addr, cell_val_t data);
uint8_t read_ram(ram_t *ram, cell_addr_t addr);
void write_ram(ram_t *ram, cell_addr_t addr, cell_val_t data);
uint8_t set_op(ram_t *ram, cell_addr_t addr, uint8_t op, cell_val_t arg1, cell_val_t arg2, uint8_t arg, uint8_t set, uint8_t err, uint8_t ifn, uint8_t ifz);
uint8_t set_val(ram_t *ram, cell_addr_t addr, cell_val_t val);

#endif