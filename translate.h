/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		translat.h - This file provides all related to the translation of an input buffer received from a
--                               pipe in response to the input of special characters, defined in codes.h.
--
-- PROGRAM:			translator
--
-- FUNCTIONS:
--                  void translate_from_input(int *pipe, size_t buffer_size)
--                  void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer)
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
#ifndef TRANSLATE_H
#define TRANSLATE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "translator.h"
#include "output.h"
#include "codes.h"

void translate_from_input(int *pipe, size_t buffer_size);
void print_translation_pipe(int *pipe, char *output_buffer, char *read_buffer);

#endif