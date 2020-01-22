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
#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "input.h"
#include "output.h"
#include "translate.h"

#define TOTAL_PROCESSES 3
#define BUFFERSIZE 256

void fatal(char *);
void clear_character_buffer(char *buf, size_t buffer_size);

#endif