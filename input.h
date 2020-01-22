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
#ifndef INPUT_H
#define INPUT_H

#include "translator.h"

void write_from_input(int *pipe, int *pipe2, size_t buffer_size);

#endif