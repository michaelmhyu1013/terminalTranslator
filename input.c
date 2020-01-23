#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "input.h"
#include "codes.h"
#include "translator.h"

void write_from_input(int *pipe, int *pipe2, size_t buffer_size)
{
    char buffer_to_translate[BUFFERSIZE];
    int c;
    int counter = 0;
    clear_character_buffer(buffer_to_translate, BUFFERSIZE);
    
    while ((c = getchar()) && !is_terminate)
    {
        if (c == ASCII_SPACE || c == ASCII_BACKSPACE ) {
            break;
        }
        if (write(pipe[1], &c, buffer_size) < 0)
        {
            fatal("failed to write to input pipe");
        };
        switch (c)
        {
        case ASCII_ABNORMAL_TERMINATION:
            kill(0, 9);
        case ASCII_NORMAL_TERMINATION:
            buffer_to_translate[counter++] = 'T';
            write_to_translate_pipe(pipe2, buffer_to_translate, BUFFERSIZE, &counter);
            is_terminate = 1;
            return;
        case ASCII_ENTER:
            write_to_translate_pipe(pipe2, buffer_to_translate, BUFFERSIZE, &counter);
            break;
        default:
            buffer_to_translate[counter++] = c;
        }
        fflush(stdin);
    }
}

void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
{
    if (write(pipe[1], buffer, buffer_size) < 0)
    {
        fatal("failed to write to input pipe");
    };
    clear_character_buffer(buffer, BUFFERSIZE);
    *counter = 0;
}