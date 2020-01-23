/*------------------------------------------------------------------------------------------------------------------
-- HEADER FILE:		input.h - This file contains all the key character codes that will be treated as special input.
--
-- PROGRAM:			translator
--
-- FUNCTIONS:
--
-- DATE:			Jan 20, 2020
--
-- REVISIONS:       (N/A)
--
-- DESIGNER:		Michael Yu
--
-- PROGRAMMER:		Michael Yu
--
----------------------------------------------------------------------------------------------------------------------*/

#ifndef CODES_H
#define CODES_H

#define ASCII_BACKSPACE                 0x7F // regular backspace function
#define ASCII_SPACE                     0x20 // regular backspace function
#define ASCII_LINE_KILL                 0x4B // clear everything before the line kill
#define ASCII_ABNORMAL_TERMINATION      0x0B // no translation and ends program
#define ASCII_NORMAL_TERMINATION        0x54 // translates and ends program
#define ASCII_ENTER                     0x45 // translates

#endif