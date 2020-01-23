/*------------------------------------------------------------------------------------------------------------------
-- SOURCE FILE: input.c - 
--
-- PROGRAM: translator
--
-- FUNCTIONS:
--
--          void translate_from_input(int *pipe, size_t buffer_size)
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

#include "translate.h"

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
void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer)
{
    printf("\n\r");
    print_from_pipe(pipe, output_buffer);
    printf("\n\r");
    clear_character_buffer(read_buffer, BUFFERSIZE);
    fflush(stdout);
}
