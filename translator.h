/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		translator.h - This file provides all methods related to capturing user input from
--                              the terminal
--
-- PROGRAM:			translator
--
-- FUNCTIONS:
--                  int main(void)
--                  void fatal(char *)
--                  void clear_character_buffer(char *buf, size_t buffer_size)
--
-- DATE:			Jan 16, 2020
--
-- REVISIONS:       (N/A)
--
-- DESIGNER:		Michael Yu
--
-- PROGRAMMER:		Michael Yu
--
----------------------------------------------------------------------------------------------------------------------*/
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "input.h"
#include "output.h"
#include "translate.h"

/*              Defines                      */
#define TOTAL_PROCESSES 3
#define BUFFERSIZE 256

/*           Function Prototypes             */
int main (void);
void fatal(char *);
void clear_character_buffer(char *buf, size_t buffer_size);

/*            Globals                         */
extern int is_terminate;

#endif