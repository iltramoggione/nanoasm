#ifndef __STRING_C__
#define __STRING_C__

#include "../include/string.h"

int is_space(char *str, int pos)
{
	return str[pos]==' ' || str[pos]=='\n';
}

int is_not_space(char *str, int pos)
{
	return !is_space(str,pos) && str[pos]!='\0';
}

int trim(char *str, int pos)
{
	for(;is_space(str,pos);pos++);
	return pos;
}

int get_word(char *str, int pos)
{
	for(;is_not_space(str,pos);pos++);
	return pos;
}

int skip(char *str, int pos)
{
	return trim(str,get_word(str,pos));
}

cell_op_t get_op(op_def_t *ops, int ops_size, char *str, int pos)
{
	if(str[pos]=='\0') return OP_END;
	if(!strncmp(str+pos,"var",3)) return OP_VAR;
	for(int i=0;i<ops_size;i++)
	{
		if(!strncmp(str+pos,ops[i].name,strlen(ops[i].name))) return i;
	}
	return OP_END;
}

int get_digit(char *str, int pos)
{
	if(str[pos]>='0' && str[pos]<='9') return str[pos]-'0';
	if(str[pos]>='a' && str[pos]<='f') return str[pos]-'a';
	if(str[pos]>='A' && str[pos]<='F') return str[pos]-'A';
	return 0;
}

val_t get_val(char *str, int pos)
{
	val_t val;
	val.val=0;
	val.arg=0;
	val.lit=0;
	val.ptr=0;
	val.ref=0;
	int base=10;
	int neg=0;
	if(str[pos]=='/')
	{
		val.arg=1;
		pos++;
	}
	if(str[pos]=='\'')
	{
		val.lit=1;
		pos++;
	}
	if(str[pos]=='*')
	{
		val.ptr=1;
		pos++;
	}
	if(str[pos]=='&')
	{
		val.ref=1;
		pos++;
	}
	if(str[pos]=='-')
	{
		neg=1;
		pos++;
	}
	if(str[pos]=='c')
	{
		val.val=str[pos+1];
	}
	else
	{
		if(str[pos]=='x') base=16;
		if(str[pos]=='b') base=2;
		if(str[pos]=='o') base=8;
		if(str[pos]=='d') base=10;
		if(str[pos]=='x' || str[pos]=='b' || str[pos]=='o' || str[pos]=='d') pos++;
		for(;is_not_space(str,pos);pos++)
		{
			val.val=val.val*base+get_digit(str,pos);
		}
	}
	if(neg) val.val=-val.val;
	return val;
}

op_flags_t get_flags(char *str, int pos)
{
	op_flags_t flags;
	flags.set=0;
	flags.err=0;
	flags.ifn=0;
	flags.ifz=0;
	if(str[pos]=='f')
	{
		flags.set=1;
		pos++;
	}
	if(str[pos]=='e')
	{
		flags.err=1;
		pos++;
	}
	if(str[pos]=='<')
	{
		flags.ifn=1;
		pos++;
	}
	if(str[pos]=='=')
	{
		flags.ifz=1;
		pos++;
	}
	return flags;
}

#endif