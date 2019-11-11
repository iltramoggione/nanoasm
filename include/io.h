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
disk_t *new_disk_t(char *file);
void free_disk_t(disk_t *disk);
int disk_writer(void *arg, uint8_t *data);
int disk_reader(void *arg, uint8_t *data);
int null_reader_writer(void *arg, uint8_t *data);
int stdout_writer(void *arg, uint8_t *data);
int stderr_writer(void *arg, uint8_t *data);
int stddebug_writer(void *arg, uint8_t *data);
int stdin_reader(void *arg, uint8_t *data);
int file_writer(void *arg, uint8_t *data);
int file_reader(void *arg, uint8_t *data);
thread_channel_t *new_thread_channel_t(void* (*type)(void *arg), int (*function)(void *arg, uint8_t *data), void *arg);
void free_thread_channel_t(thread_channel_t *thread_channel);
thread_channel_t *new_thread_channel_t_writing(int (*function)(void *arg, uint8_t *data), void *arg);
thread_channel_t *new_thread_channel_t_reading(int (*function)(void *arg, uint8_t *data), void *arg);
thread_channel_t *new_null_writer_channel();
thread_channel_t *new_stdout_writer_channel();
thread_channel_t *new_stderr_writer_channel();
thread_channel_t *new_stddebug_writer_channel();
thread_channel_t *new_file_writer_channel(FILE *f);
thread_channel_t *new_disk_writer_channel(disk_t *disk);
thread_channel_t *new_null_reader_channel();
thread_channel_t *new_stdin_reader_channel();
thread_channel_t *new_file_reader_channel(FILE *f);
thread_channel_t *new_disk_reader_channel(disk_t *disk);

#endif