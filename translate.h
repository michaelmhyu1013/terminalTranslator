/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		input.h - This file provides all methods related to capturing user input from
--                              the terminal
--
-- PROGRAM:			translator
--
-- FUNCTIONS:
--                  void printGpsData(struct gps_data_t *gpsdata, bool isGPSDataValid)
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

#define ASCII_BACKSPACE                 0x58 // regular backspace function
#define ASCII_LINE_KILL                 0x4B // clear everything before the line kill
#define ASCII_ABNORMAL_TERMINATION      0x0B // no translation and ends program
#define ASCII_NORMAL_TERMINATION        0x54 // translates and ends program
#define ASCII_ENTER                     0x46 // translates

void translate_from_input(int *pipe, size_t buffer_size);

#endif