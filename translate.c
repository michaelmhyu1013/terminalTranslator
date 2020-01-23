/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: translate.c
--
-- PROGRAM: translator
--
-- FUNCTIONS:
--
--          void translate_from_input(int *pipe, size_t buffer_size)
--          void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer)
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
--          This file contains all functionality related to the translation of the characters inputed by the user. Note that
--          translation of input only occurs when two characters are detected by the input: 'E' or 'T'.
----------------------------------------------------------------------------------------------------------------------*/

#include "translate.h"

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: translate_from_input
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: translate_from_input(int *pipe, size_t buffer_size)
--
--              pipe:               pipe that will be read for data
--              buffer_size:        maximum buffer size that will be read from the pipe
--
-- RETURNS: void.
--
-- NOTES:   This function reads data from user input and stores it into a temporary buffer. The buffer is then
--          iterated over to translate all characters within the buffer as defined by the guidelines in 
--          translator.h.
-- 
----------------------------------------------------------------------------------------------------------------------*/
void translate_from_input(int *pipe, size_t buffer_size)
{
    int n_read, i, out_counter;
    char translate_in_buffer[BUFFERSIZE];
    char translate_out_buffer[BUFFERSIZE];
    char output_buffer[BUFFERSIZE];

    out_counter = 0;

    clear_character_buffer(translate_in_buffer, BUFFERSIZE);
    clear_character_buffer(translate_out_buffer, BUFFERSIZE);

    switch (n_read = read(pipe[0], translate_in_buffer, buffer_size))
    {
    case -1:
        fatal("translate fail");
        break;
    case 0:
        printf("pipe empty");
        break;
    default:
        for (i = 0; i < BUFFERSIZE; i++, out_counter++)
        {
            switch (translate_in_buffer[i])
            {
            case 'a':
                translate_out_buffer[i] = 'z';
                break;
            case 'X':
                out_counter = out_counter < 1 ? 0 : out_counter - 2;
                break;
            case ASCII_LINE_KILL:
                clear_character_buffer(translate_out_buffer, BUFFERSIZE);
                fflush(stdout);
                out_counter = -1; // Set to beginning of string; -1 for increment from loop to 0
                break;
            case ASCII_NORMAL_TERMINATION:
                is_terminate = 1; // terminate loop in main thread for translate process
                break;
            default:
                translate_out_buffer[out_counter] = translate_in_buffer[i];
            }
        }
    }
    if (write(pipe[1], translate_out_buffer, buffer_size) < 0)
    {
        fatal("failed to write to input pipe");
        exit(1);
    };
    print_translation_pipe(pipe, output_buffer, translate_in_buffer);
}

/*------------------------------------------------------------------------------------------------------------------
-- FUNCTION: print_translation_pipe
--
-- DATE: January 17, 2020
--
-- REVISIONS: N/A
--
-- DESIGNER: Michael Yu
--
-- PROGRAMMER: Michael Yu
--
-- INTERFACE: void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer)
--
--              pipe:           pipe that will be read and printed to the console with proper formatting
--              output_buffer:  buffer that will be used to store the data read from the pipe
--              read_buffer:    buffer that was read into pipe and will be cleared
--
--
-- RETURNS: void.
--
-- NOTES:   Call this function to format and print the data stored in the translation pipe to the console. This is done
--          by piping the data to the write descriptor of the translation pipe. 
-- 
----------------------------------------------------------------------------------------------------------------------*/
void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer)
{
    printf("\n\r");
    print_from_pipe(pipe, output_buffer);
    printf("\n\r");
    clear_character_buffer(read_buffer, BUFFERSIZE);
    fflush(stdout);
}
