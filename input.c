/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: input.c - 
--
-- PROGRAM: translator
--
-- FUNCTIONS:
--
--          void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
--          void write_from_input(int *pipe, int *pipe2, size_t buffer_size)
--
-- DATE: January 16
--
-- REVISIONS: (Date and Description)
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
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "input.h"
#include "codes.h"
#include "translator.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: write_from_input
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void write_from_input(int *pipe, int *pipe2, size_t buffer_size)
--
--
--
--
-- RETURNS: void.
--
-- NOTES:
-- 
----------------------------------------------------------------------------------------------------------------------*/
void write_from_input(int *pipe, int *pipe2, size_t buffer_size)
{
    char buffer_to_translate[BUFFERSIZE];
    int c;
    int counter = 0;
    clear_character_buffer(buffer_to_translate, BUFFERSIZE);

    while ((c = getchar()) && !is_terminate)
    {
        if (c == ASCII_SPACE || c == ASCII_BACKSPACE || c == ASCII_DEL)
        {
            continue;
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
            buffer_to_translate[counter++] = 'T'; // write T to translate pipe for proper process kill
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
void write_to_translate_pipe(int *pipe, char *buffer, size_t buffer_size, int *counter)
{
    if (write(pipe[1], buffer, buffer_size) < 0)
    {
        fatal("failed to write to input pipe");
    };
    clear_character_buffer(buffer, BUFFERSIZE);
    *counter = 0;
}