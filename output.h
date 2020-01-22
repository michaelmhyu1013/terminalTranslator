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
#ifndef OUTPUT_H
#define OUTPUT_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "translator.h"

void print_from_pipe(int *pipe, char *read_buffer);

#endif