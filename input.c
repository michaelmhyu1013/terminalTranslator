#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "input.h"

void write_from_input(int *pipe, int *pipe2, size_t buffer_size)
{
    char c;
    while ((c = getchar()))
    {
        switch (c)
        {
        default:
            if (write(pipe[1], &c, buffer_size) < 0)
            {
                fatal("failed to write to input pipe");
            };
            if (write(pipe2[1], &c, buffer_size) < 0)
            {
                fatal("failed to write to input pipe");
            };
        }
    }
}