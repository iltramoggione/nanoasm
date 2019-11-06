#ifndef __WRITE_RAM_C__
#define __WRITE_RAM_C__

#include "../include/write_ram.h"

int port_is_input(int port)
{
	return port&2;
}

uint8_t read_ram_raw(ram_t *ram, cell_addr_t addr)
{
	return ram->data[addr];
}

void write_ram_raw(ram_t *ram, cell_addr_t addr, cell_val_t data)
{
	ram->data[addr]=data;
}

uint8_t read_ram(ram_t *ram, cell_addr_t addr)
{
	int port_bit[4]={
		RAM_PORT0_BIT,
		RAM_PORT1_BIT,
		RAM_PORT2_BIT,
		RAM_PORT3_BIT
	};
	if(addr==RAM_FLAGS)
	{
		for(int i=0;i<4;i++)
		{
			if((port_is_input(i) && channel_can_read(ram->port[i])) || (!port_is_input(i) && channel_can_write(ram->port[i])))
			{
				ram->data[RAM_FLAGS]|=1<<port_bit[i];
			}
			else
			{
				ram->data[RAM_FLAGS]&=~(1<<port_bit[i]);
			}
		}
		return ram->data[RAM_FLAGS];
	}
	if(addr==RAM_PORT0)
	{
		return channel_read(ram->port[0]);
	}
	if(addr==RAM_PORT1)
	{
		return channel_read(ram->port[1]);
	}
	return read_ram_raw(ram,addr);
}

void write_ram(ram_t *ram, cell_addr_t addr, cell_val_t data)
{
	if(addr==RAM_PORT2)
	{
		channel_write(ram->port[2],data);
		return;
	}
	if(addr==RAM_PORT3)
	{
		channel_write(ram->port[3],data);
		return;
	}
	write_ram_raw(ram,addr,data);
}

uint8_t set_op(ram_t *ram, cell_addr_t addr, uint8_t op, cell_val_t arg1, cell_val_t arg2, uint8_t arg, uint8_t set, uint8_t err, uint8_t ifn, uint8_t ifz)
{
	op_t opr;
	opr.op=op;
	opr.arg=arg;
	opr.set=set;
	opr.err=err;
	opr.ifn=ifn;
	opr.ifz=ifz;
	write_ram_raw(ram,addr,opr.data);
	write_ram_raw(ram,addr+1,arg1);
	write_ram_raw(ram,addr+2,arg2);
	show_op(ram,addr);
	return addr+OP_SIZE;
}

uint8_t set_val(ram_t *ram, cell_addr_t addr, cell_val_t val)
{
	write_ram_raw(ram,addr,val);
	show_cell(ram,addr);
	return addr+1;
}

#endif