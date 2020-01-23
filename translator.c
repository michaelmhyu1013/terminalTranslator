/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: translator.c - An application that reads user input from the terminal and echoes back the contents
--                              to the terminal window. It allows the user to translate the input following a certain
--                              set of rules for special characters.
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
-- This program constantly monitors user input from the terminal and echoes the exact input back to the console. All 
-- built-in keyboard commands are disabled using the command:
--
--                  system("/bin/stty raw igncr -echo")
--
-- Special characters are handled differently following a certain set of guidelines, listed below:
--                  'E'     - command to perform translation of user input based on following special characters
--                  'a'     - character will be converted to 'z' upon translation
--                  'X'     - character that will represent the BACKSPACE key upon translation
--                  'K'     - character that will represent the LINE-KILL key upon translation
--                  'T'     - character that will represent NORMAL TERMINATION of the program. The user input will be
--                              translated prior to termination
--                  'CTRL+k' - character that will represent ABNORMAL TERMINATION of the program. Application will
--                              immediately exit and no echo or translation of user text will occur 
--
-- The application utilizies a simple fan architecture in constructing the three processes. The parent process is responsible
-- for handling user input, and the two child processes are responsible for translation and terminal output. Upon any 
-- error that occurs from reading or writing from the pipe, the program will automatically terminate and notify the user of
-- the error that occured.
--
-- The application utilizes two pipes, one for transferring data from standard input to the output process. The second pipe
-- is used to transfer translated data from the translation process to the output process.
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
-- FUNCTION: main
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: int main(void)
--
-- RETURNS: int
--              0 upon successful termination of the program
--
-- NOTES:   Acts as the driver of the program and is responsible for the creation of the processes and pipes that will
--          be used in the program. Upon any unsuccessful pipe or fork call, the application will exit. All processes
--          are controlled by a boolean value that will only be toggled upon normal termination ('T'). The 'T' will
--          be piped appropriately to all processes to successfully terminate the loops in each process.
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
-- FUNCTION: fatal
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void fatal(char *s)
--              s: represents the string that will be printed upon a failed read or write function
--
-- RETURNS: void.
--
-- NOTES:   General utility function for logging errors related to read or write functions for pipes.    
--
----------------------------------------------------------------------------------------------------------------------*/
void fatal(char *s)
{
    perror(s);
    exit(1);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: clear_character_buffer
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void clear_character_buffer(char *buf, size_t buffer_size)
--              buf:            the buffer that will be memset and have all its values set to null character
--              buffer_size:    represents the size of the buffer tht will be memset
--
-- RETURNS: void.
--
-- NOTES:   General utility function that will null out all values for a character buffer. Used to flush a buffer
--          after using it in a read or write from a pipe.
--      
----------------------------------------------------------------------------------------------------------------------*/
void clear_character_buffer(char *buf, size_t buffer_size)
{
    memset(buf, '\0', buffer_size * sizeof(buf[0]));
}