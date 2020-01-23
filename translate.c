#include "translate.h"

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
                out_counter = -1;
                break;
            case ASCII_NORMAL_TERMINATION:
                is_terminate = 1;
                break;
            default:
                translate_out_buffer[out_counter] = translate_in_buffer[i];
            }
        }
    }
    if (write(pipe[1], translate_out_buffer, buffer_size) < 0)
    {
        fatal("failed to write to input pipe");
    };
    printf("\n\r");
    print_from_pipe(pipe, output_buffer);
    printf("\n\r");
    clear_character_buffer(translate_in_buffer, BUFFERSIZE);
    fflush(stdout);
}
