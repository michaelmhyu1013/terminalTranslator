/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: input.c
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
--          This file contains all the functions that are related with the retrieval of user input data. It provides 
--          functionality to read from standard input and handles certain special cases prior to writing to the output
--           and translation pipes.
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
--              pipe:           pipe that will be written to for all user input. used for general echo outpput
--              pipe2:          pipe that will be written to for translation
--              buffer_size:    represents the buffer size of the two buffers
--
-- RETURNS: void.
--
-- NOTES:
--              Call this function to read from standard input and store the data into the pipes that will be read
--              by the output and translate processes. The output pipe will capture all characters, whereas the translate
--              process will store data from the pipe for later processing.
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
--              pipe:           pipe that will be written to
--              buffer:         buffer that stores the data to be written to the pipe
--              buffer_size:    size of the buffer that will be read from
--              counter:        counter that stores the current index of the buffer. used to reset the counter upon
--                              user input of the enter key 'E'
--
-- RETURNS: void.
--
-- NOTES:           Call this function to write a buffer to the translation pipe. This translation pipe is used by the
--                  child translate process that will convert user input based on the special guidelines denoted in
--                  translator.h.
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