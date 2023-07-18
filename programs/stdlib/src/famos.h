#ifndef FAMOS_H
#define FAMOS_H

#include <stddef.h>
#include <stdbool.h>


struct command_argument
{
    char argument[512];
    struct command_argument* next;
};

struct process_arguments
{
    int argc;
    char** argv;
};


void print(const char* filename);
int famos_getkey();

void* famos_malloc(size_t size);
void famos_free(void* ptr);
void famos_putchar(char c);
int famos_getkeyblock();
void famos_terminal_readline(char* out, int max, bool output_while_typing);
void famos_process_load_start(const char* filename);
struct command_argument* famos_parse_command(const char* command, int max);
void famos_process_get_arguments(struct process_arguments* arguments);
int famos_system(struct command_argument* arguments);
int famos_system_run(const char* command);
void famos_exit();

#endif