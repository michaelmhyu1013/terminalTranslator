/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: output.c 
--
-- PROGRAM: translator
--
-- FUNCTIONS:
--
--              void print_from_pipe(int *pipe, char *read_buffer)
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
--                          This file contains all the functions related to interaction with outputting to the terminal
--                          window. This mainly encompasses print statements that will be used to display the desired
--                          output to the terminal.
----------------------------------------------------------------------------------------------------------------------*/

#include "output.h"
#include "translator.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_from_pipe
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void print_from_pipe(int *pipe, char *read_buffer)
--
--                  pipe:           specifies the pipe that will be read from for output
--                  read_buffer:    specifies the buffer that will store the data read from the pipe 
--
--
-- RETURNS: void.
--
-- NOTES:   Call this function to access the read description of the desired pipe. The contents of the pipe will be read
--          to the specified buffer and subsequently printed to the console. This is a blocking call and will only return
--          once something is read from the pipe, or an error occured.
-- 
----------------------------------------------------------------------------------------------------------------------*/
void print_from_pipe(int *pipe, char *read_buffer)
{
    int n_read;

    switch (n_read = read(pipe[0], read_buffer, BUFFERSIZE))
    {
    case -1:
        break;
    case 0:
        printf("pipe empty");
        break;
    default:
        printf("%s", read_buffer);
        clear_character_buffer(read_buffer, BUFFERSIZE);
        fflush(stdout);
    }
}