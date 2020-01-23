
#include "output.h"
#include "translator.h"

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