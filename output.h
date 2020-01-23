/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		output.h - This file provides all methods related to printing from a pipe to standard output.
--
-- PROGRAM:			translator
--
-- FUNCTIONS:
--                  void print_from_pipe(int *pipe, char *read_buffer)
--
-- DATE:			Jan 18, 2020
--
-- REVISIONS:       (N/A)
--
-- DESIGNER:		Michael Yu
--
-- PROGRAMMER:		Michael Yu
--
----------------------------------------------------------------------------------------------------------------------*/
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void print_from_pipe(int *pipe, char *read_buffer);

#endif