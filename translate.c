#include "translate.h"

void translate_from_input(int *pipe, size_t buffer_size)
{
    int n_read, i;
    char translate_in_buffer[BUFFERSIZE];
    char translate_out_buffer[BUFFERSIZE];

    clear_character_buffer(translate_in_buffer, BUFFERSIZE);
    clear_character_buffer(translate_out_buffer, BUFFERSIZE);

    for (;;)
    {
        switch (n_read = read(pipe[0], translate_in_buffer, buffer_size))
        {
        case -1:
            fatal("translate fail");
            break;
        case 0:
            printf("pipe empty");
            break;
        default:
            for (i = 0; i < BUFFERSIZE; i++)
            {
                switch (translate_in_buffer[i])
                {
                case 'a':
                    translate_in_buffer[i] = 'z';
                    printf("%c", translate_in_buffer[i]);
                    break;
                }
                // if (write(output_pipe[1], translate_in_buffer, buffer_size) < 0)
                // {
                //     fatal("failed to write to input pipe");
                // };
                // process every character in translate_in_buffer for special processing
            }
        }
        clear_character_buffer(translate_in_buffer, BUFFERSIZE);
        fflush(stdout);
    }
}
