/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: translator.c - 
--
-- PROGRAM: translator
--
-- FUNCTIONS:
--
--              int main(void)
--              void fatal(char *s)
--              void clear_character_buffer(char *buf, size_t buffer_size)
--
-- DATE: January 16
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- NOTES:
-- The program will monitor a directory that is specified in a configuration file for any type of file
-- modification activity (creation, read/write, deletion). The design uses the “inotify” kernel-level
-- utility to obtain the file system event notification. The “select” system call is used to monitor
-- the watch descriptor (returned from inotify).
--
-- Once select is triggered, the directory under watch is processed to determine the exact type of
-- file activity. Once the created/modified files have been identified, they are moved to a separate
-- archive directory. Before the archival process takes place, the system process table (/proc) is
-- searched to verify that the modifying process is currently active and running.
--
-- Note that the application once invoked, will continue to execute as a daemon.
----------------------------------------------------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "translator.h"

int is_terminate = 0;

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: write_to_translate_pipe
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
--
--
--
--
--
-- RETURNS: void.
--
-- NOTES:
-- 
----------------------------------------------------------------------------------------------------------------------*/
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

    /* ------- Open the translate pipe ----- */
    if (pipe(translate_output_pipe) < 0)
    {
        fatal("translate-output pipe call fail");
    }

    /*-------- parent fork the translate process ---------------*/
    if ((translate_pid = fork()) < 0)
    {
        fatal("translate fork fail");
    }

    /*------- PARENT BLOCK -------*/
    if (translate_pid > 0)
    {
        /*-------- parent forks the translate process ---------------*/
        if ((output_pid = fork()) < 0)
        {
            fatal("fork translate call");
        }
        /*------- parent main execution: read from stdin -------*/
        if (output_pid > 0)
        {
            close(translate_output_pipe[0]);
            close(input_pipe[0]);

            /* loops until termination */
            while (!is_terminate)
            {
                write_from_input(input_pipe, translate_output_pipe, BUFFERSIZE);
            }

            /* Lines after execute only on normal termination */

            waitpid(translate_pid, NULL, 0);

            close(translate_output_pipe[1]);
            close(input_pipe[1]);

            kill(translate_pid, SIGKILL);
            kill(output_pid, SIGKILL);

            system("/bin/stty cooked -igncr echo");
        }

        /* ------- OUTPUT BLOCK------- */
        if (output_pid == 0)
        {
            close(input_pipe[1]);
            close(translate_output_pipe[1]);

            while (!is_terminate)
            {
                print_from_pipe(input_pipe, read_buffer);
            }
            printf("output exit");
            exit(0);
        }
    }

    /*-------- TRANSLATE BLOCK ------*/
    if (translate_pid == 0)
    {
        close(input_pipe[1]);

        while (!is_terminate)
        {
            translate_from_input(translate_output_pipe, BUFFERSIZE);
        }
        exit(0);
    }
    return 0;
}


/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: write_to_translate_pipe
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
--
--
--
--
--
-- RETURNS: void.
--
-- NOTES:
-- 
----------------------------------------------------------------------------------------------------------------------*/
void fatal(char *s)
{
    perror(s);
    exit(1);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: write_to_translate_pipe
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
--
--
--
--
--
-- RETURNS: void.
--
-- NOTES:
-- 
----------------------------------------------------------------------------------------------------------------------*/
void clear_character_buffer(char *buf, size_t buffer_size)
{
    memset(buf, '\0', buffer_size * sizeof(buf[0]));
}