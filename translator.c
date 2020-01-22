#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "translator.h"

//stty sane [ctrl+j]                    - command line command to restore keyboard
//kill(0,9) is for abnormal termination
//killpid(pid) is for normal termination

int main(void)
{
    system("/bin/stty raw igncr -echo");
    char read_buffer[BUFFERSIZE];
    int input_pipe[2], translate_output_pipe[2];
    pid_t translate_pid, output_pid;

    // Null the buffer
    clear_character_buffer(read_buffer, BUFFERSIZE);

    /*-- Open the input pipe ----*/
    if (pipe(input_pipe) < 0)
    {
        fatal("Parent pipe call fail");
    }

    /* ------- translate process creates pipe to communicate with output process ----- */
    if (pipe(translate_output_pipe) < 0)
    {
        fatal("translate-output pipe call fail");
    }

    /*-------- fork the translate process ---------------*/
    if ((translate_pid = fork()) < 0)
    {
        fatal("translate fork fail");
    }

    /*------- parent forks the output process -------*/
    if (translate_pid > 0)
    {
        /*-------- fork the translate process ---------------*/
        if ((output_pid = fork()) < 0)
        {
            fatal("fork translate call");
        }
        /*------- parent input process execution -------*/
        if (output_pid > 0)
        {
            close(translate_output_pipe[0]);
            close(input_pipe[0]);
            write_from_input(input_pipe, translate_output_pipe, BUFFERSIZE);
        }
        /* ------- output process execution ------- */
        if (output_pid == 0)
        {
            close(input_pipe[1]);
            close(translate_output_pipe[1]);

            for (;;)
            {
                print_from_pipe(input_pipe, read_buffer);
            }
        }
    }
    /*-------- translate process execution ------*/
    if (translate_pid == 0)
    {
        printf("translate process\n");
        close(input_pipe[1]);

        for (;;)
        {
            translate_from_input(translate_output_pipe, BUFFERSIZE);
        }
    }

    printf("Completion of main\n");
    // system("/bin/stty cooked echo");
    return 0;
}

void fatal(char *s)
{
    perror(s); /* print error msg and die */
    exit(1);
}

void clear_character_buffer(char *buf, size_t buffer_size)
{
    memset(buf, '\0', buffer_size * sizeof(buf[0]));
}