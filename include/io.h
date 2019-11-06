#ifndef __IO_H__
#define __IO_H__

#include "globals.h"

channel_t *new_channel_t();
int channel_is_closed(channel_t *channel);
int channel_can_read(channel_t *channel);
int channel_can_write(channel_t *channel);
cell_t channel_read(channel_t *channel);
void channel_write(channel_t *channel, cell_t data);
void channel_close(channel_t *channel);
void *channel_writing(void *arg);
void *channel_reading(void *arg);
int null_writer(void *arg, uint8_t *data);
int stdout_writer(void *arg, uint8_t *data);
int stderr_writer(void *arg, uint8_t *data);
int stddebug_writer(void *arg, uint8_t *data);
thread_channel_t *new_writing_channel(int (*function)(void *arg, uint8_t *data), void *arg);
thread_channel_t *new_null_writer_channel();
thread_channel_t *new_stdout_writer_channel();
thread_channel_t *new_stderr_writer_channel();
thread_channel_t *new_stddebug_writer_channel();

#endif