/*---------------------------------------------------------------------------
  
Copyright (c) 2016, Noname

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

-----------------------------------------------------------------------------
Function:  Hough transform on a minimal image
Created:   23-Feb-2016
Hardware:  ATMega32U4 
---------------------------------------------------------------------------*/

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <avr/pgmspace.h>


#define __Height 6
#define __Width 6

static const uint8_t red[10][10] PROGMEM = {
  { 21,21,20,16,13,13,15,17,19,19} ,
  { 9,21,27,22,16,17,18,17,19,19} ,
  { 12,16,15,12,15,23,22,16,17,19} ,
  { 12,7,6,12,19,17,16,17,17,17} ,
  { 15,4,6,18,19,4,4,17,16,17} ,
  { 53,20,0,2,10,8,9,16,15,16} ,
  { 66,46,18,4,5,14,15,12,13,15} ,
  { 28,61,71,41,9,4,9,7,10,13} ,
  { 0,14,46,66,53,21,0,0,13,11} ,
  { 0,0,2,29,50,47,28,16,0,10}
};

static const uint8_t blue[10][10] PROGMEM = {
  { 40,41,40,36,34,34,37,39,41,41} ,
  { 28,40,47,42,37,38,40,39,41,41} ,
  { 35,39,39,36,40,46,46,40,41,40} ,
  { 39,34,34,40,45,44,41,43,41,41} ,
  { 47,37,37,50,49,34,33,44,41,41} ,
  { 89,56,33,36,43,40,39,45,42,41} ,
  { 108,86,58,42,42,49,49,44,42,41} ,
  { 74,104,115,83,50,42,44,40,42,41} ,
  { 40,61,92,109,95,61,38,31,46,43} ,
  { 49,42,49,75,93,89,68,54,33,43}
};

static const uint8_t green[10][10] PROGMEM = {
  { 80,78,75,69,63,61,61,62,64,65} ,
  { 68,80,84,75,68,65,64,63,65,65} ,
  { 77,80,77,72,71,77,72,66,65,67} ,
  { 84,77,74,79,80,74,71,70,67,67} ,
  { 94,82,81,91,87,68,65,74,71,69} ,
  { 137,104,78,81,84,79,75,79,72,72} ,
  { 158,137,107,89,86,91,87,82,76,74} ,
  { 124,155,164,131,96,87,86,81,80,78} ,
  { 92,113,142,160,145,110,83,76,89,82} ,
  { 100,93,101,125,144,139,117,101,79,86}
};

#endif /* _IMAGE_H_ */
