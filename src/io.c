#ifndef __IO_C__
#define __IO_C__

#include "../include/io.h"

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

int null_reader_writer(void *arg, uint8_t *data)
{
	return 1;
}

int stdout_writer(void *arg, uint8_t *data)
{
	printf("%c",*data);
	return 0;
}

int stderr_writer(void *arg, uint8_t *data)
{
	fprintf(stderr,"%c",*data);
	return 0;
}

int stddebug_writer(void *arg, uint8_t *data)
{
	fprintf(STDDEBUG,"%c",*data);
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
	fwrite(data,1,1,(FILE*)arg);
	return 0;
}

int file_reader(void *arg, uint8_t *data)
{
	fread(data,1,1,(FILE*)arg);
	if(feof((FILE*)arg)) return 1;
	return 0;
}

thread_channel_t *new_channel(void* (*type)(void *arg), int (*function)(void *arg, uint8_t *data), void *arg)
{
	thread_channel_t *r=malloc(sizeof(thread_channel_t));
	r->channel=new_channel_t();
	r->function=function;
	r->arg=arg;
	pthread_create(&r->thread,NULL,type,r);
	return r;
}

thread_channel_t *new_channel_writing(int (*function)(void *arg, uint8_t *data), void *arg)
{
	return new_channel(channel_writing,function,arg);
}

thread_channel_t *new_channel_reading(int (*function)(void *arg, uint8_t *data), void *arg)
{
	return new_channel(channel_reading,function,arg);
}

thread_channel_t *new_null_writer_channel()
{
	return new_channel_writing(null_reader_writer,NULL);
}

thread_channel_t *new_stdout_writer_channel()
{
	return new_channel_writing(stdout_writer,NULL);
}

thread_channel_t *new_stderr_writer_channel()
{
	return new_channel_writing(stderr_writer,NULL);
}

thread_channel_t *new_stddebug_writer_channel()
{
	return new_channel_writing(stddebug_writer,NULL);
}

thread_channel_t *new_file_writer_channel(FILE *f)
{
	return new_channel_writing(file_writer,f);
}

thread_channel_t *new_null_reader_channel()
{
	return new_channel_reading(null_reader_writer,NULL);
}

thread_channel_t *new_stdin_reader_channel()
{
	return new_channel_reading(stdin_reader,NULL);
}

thread_channel_t *new_file_reader_channel(FILE *f)
{
	return new_channel_reading(file_reader,f);
}

#endif