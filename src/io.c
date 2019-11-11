#ifndef __IO_C__
#define __IO_C__

#include "../include/io.h"

enum disk_op{
	DISK_GET_OP,
	DISK_GET_ADDR_1,
	DISK_GET_ADDR_2,
	DISK_GET_ADDR_3,
	DISK_GET_ADDR_4,
	DISK_W_GET_DATA,
};

channel_t *new_channel_t()
{
	channel_t *r=malloc(sizeof(channel_t));
	r->data=0;
	r->has_data=0;
	r->closed=0;
	r->mutex=PTHREAD_MUTEX_INITIALIZER;
	r->cond=PTHREAD_COND_INITIALIZER;
	return r;
}

int channel_is_closed(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	int r=channel->closed;
	pthread_mutex_unlock(&channel->mutex);
	return r;
}

int channel_can_read(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	int r=channel->has_data && !channel->closed;
	pthread_mutex_unlock(&channel->mutex);
	return r;
}

int channel_can_write(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	int r=!channel->has_data && !channel->closed;
	pthread_mutex_unlock(&channel->mutex);
	return r;
}

cell_t channel_read(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	for(;!channel->has_data && !channel->closed;)
	{
		pthread_cond_wait(&channel->cond,&channel->mutex);
	}
	channel->has_data=0;
	cell_t r=channel->data;
	pthread_cond_broadcast(&channel->cond);
	pthread_mutex_unlock(&channel->mutex);
	return r;
}

void channel_write(channel_t *channel, cell_t data)
{
	pthread_mutex_lock(&channel->mutex);
	for(;channel->has_data && !channel->closed;)
	{
		pthread_cond_wait(&channel->cond,&channel->mutex);
	}
	channel->has_data=1;
	channel->data=data;
	pthread_cond_broadcast(&channel->cond);
	pthread_mutex_unlock(&channel->mutex);
}

void channel_close(channel_t *channel)
{
	pthread_mutex_lock(&channel->mutex);
	channel->closed=1;
	pthread_cond_broadcast(&channel->cond);
	pthread_mutex_unlock(&channel->mutex);
}

void *channel_writing(void *arg)
{
	thread_channel_t *this=(thread_channel_t*)arg;
	for(;;)
	{
		uint8_t data=channel_read(this->channel);
		if(channel_is_closed(this->channel))
		{
			return NULL;
		}
		if(this->function(this->arg,&data))
		{
			channel_close(this->channel);
			return NULL;
		}
	}
}

void *channel_reading(void *arg)
{
	thread_channel_t *this=(thread_channel_t*)arg;
	for(;;)
	{
		if(channel_is_closed(this->channel))
		{
			return NULL;
		}
		uint8_t data;
		if(this->function(this->arg,&data))
		{
			channel_close(this->channel);
			return NULL;
		}
		channel_write(this->channel,data);
	}
}

disk_t *new_disk_t(char *file)
{
	disk_t *r=malloc(sizeof(disk_t));
	r->step=0;
	r->op=0;
	r->addr=0;
	r->channel=new_channel_t();
	r->f=fopen(file,"r+b");
	return r;
}

void free_disk_t(disk_t *disk)
{
	channel_close(disk->channel);
	fclose(disk->f);
	free(disk->channel);
	free(disk);
}

int disk_writer(void *arg, uint8_t *data)
{
	disk_t *disk=(disk_t*)arg;
	switch(disk->step)
	{
		case DISK_GET_OP:
			disk->op=*data;
			break;
		case DISK_GET_ADDR_1:
		case DISK_GET_ADDR_2:
		case DISK_GET_ADDR_3:
		case DISK_GET_ADDR_4:
			disk->addr=(disk->addr<<8)+*data;
			break;
		case DISK_W_GET_DATA:
			disk->data=*data;
			break;
	}
	switch(disk->op)
	{
		case 'r':
			if(disk->step==DISK_GET_ADDR_4)
			{
				fseek(disk->f,disk->addr,SEEK_SET);
				fread(&disk->data,1,1,disk->f);
				channel_write(disk->channel,disk->data);
				disk->step=-1;
			}
			break;
		case 'w':
			if(disk->step==DISK_W_GET_DATA)
			{
				fseek(disk->f,disk->addr,SEEK_SET);
				fwrite(&disk->data,1,1,disk->f);
				disk->step=-1;
			}
			break;
		default: return 1;
	}
	disk->step++;
	return 0;
}

int disk_reader(void *arg, uint8_t *data)
{
	disk_t *disk=(disk_t*)arg;
	*data=channel_read(disk->channel);
	return 0;
}

int null_reader_writer(void *arg, uint8_t *data)
{
	return 1;
}

int stdout_writer(void *arg, uint8_t *data)
{
	printf("%c",*data);
	fflush(stdout);
	return 0;
}

int stderr_writer(void *arg, uint8_t *data)
{
	fprintf(stderr,"%c",*data);
	fflush(stderr);
	return 0;
}

int stddebug_writer(void *arg, uint8_t *data)
{
	DEBUG("%c",*data);
	fflush(STDDEBUG);
	return 0;
}

int stdin_reader(void *arg, uint8_t *data)
{
	int c;
	c=fgetc(stdin);
	if(c==EOF) return 1;
	*data=(uint8_t)c;
	return 0;
}

int file_writer(void *arg, uint8_t *data)
{
	FILE *f=arg;
	fwrite(data,1,1,f);
	return 0;
}

int file_reader(void *arg, uint8_t *data)
{
	FILE *f=arg;
	fread(data,1,1,f);
	if(feof(f)) return 1;
	return 0;
}

thread_channel_t *new_thread_channel_t(void* (*type)(void *arg), int (*function)(void *arg, uint8_t *data), void *arg)
{
	thread_channel_t *r=malloc(sizeof(thread_channel_t));
	r->channel=new_channel_t();
	r->function=function;
	r->arg=arg;
	pthread_create(&r->thread,NULL,type,r);
	return r;
}

void free_thread_channel_t(thread_channel_t *thread_channel)
{
	channel_close(thread_channel->channel);
	pthread_join(thread_channel->thread,NULL);
	free(thread_channel->channel);
	free(thread_channel);
}

thread_channel_t *new_thread_channel_t_writing(int (*function)(void *arg, uint8_t *data), void *arg)
{
	return new_thread_channel_t(channel_writing,function,arg);
}

thread_channel_t *new_thread_channel_t_reading(int (*function)(void *arg, uint8_t *data), void *arg)
{
	return new_thread_channel_t(channel_reading,function,arg);
}

thread_channel_t *new_null_writer_channel()
{
	return new_thread_channel_t_writing(null_reader_writer,NULL);
}

thread_channel_t *new_stdout_writer_channel()
{
	return new_thread_channel_t_writing(stdout_writer,NULL);
}

thread_channel_t *new_stderr_writer_channel()
{
	return new_thread_channel_t_writing(stderr_writer,NULL);
}

thread_channel_t *new_stddebug_writer_channel()
{
	return new_thread_channel_t_writing(stddebug_writer,NULL);
}

thread_channel_t *new_file_writer_channel(FILE *f)
{
	return new_thread_channel_t_writing(file_writer,f);
}

thread_channel_t *new_disk_writer_channel(disk_t *disk)
{
	return new_thread_channel_t_writing(disk_writer,disk);
}

thread_channel_t *new_null_reader_channel()
{
	return new_thread_channel_t_reading(null_reader_writer,NULL);
}

thread_channel_t *new_stdin_reader_channel()
{
	return new_thread_channel_t_reading(stdin_reader,NULL);
}

thread_channel_t *new_file_reader_channel(FILE *f)
{
	return new_thread_channel_t_reading(file_reader,f);
}

thread_channel_t *new_disk_reader_channel(disk_t *disk)
{
	return new_thread_channel_t_reading(disk_reader,disk);
}

#endif