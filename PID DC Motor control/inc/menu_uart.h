/*---------------------------------------------------------------------------

Copyright (c) 2016, Vaibhav Desai

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
Function:  UART based text menu user interface
Created:   16-Mar-2016
Hardware:  ATMega32U4 
---------------------------------------------------------------------------*/

#ifndef _MENU_UART_H_
#define _MENU_UART_H_

#include <stdio.h>
#include <string.h>
#include <avr/pgmspace.h>
#include "usart.h"


#define WAITING_DIALOGUE "\r\nWaiting for user input... "


typedef struct
{
   float kp;
   float kd;
   float ki;
   float err;
   float pos_ref;
   float pos_now;
   float pid_drv;
} pid_ctrl_db_typ;


/* Generic buffer */
typedef struct
{
   float *data;
   uint8_t full;
   uint8_t size;
   uint8_t ridx;
   uint8_t widx;
} buffer_typ;


extern void set_pid_params_ref(pid_ctrl_db_typ* npid);

extern const pid_ctrl_db_typ* get_pid_params_ref();

void menu_uart_prompt(void);

void handle_user_inputs(char* buf, uint8_t* len);

void print_all_pid_params(pid_ctrl_db_typ *db);


/* Buffer maintenance */
void enqueue_buffer(buffer_typ *cbuf, float c);

void reset_buffer(buffer_typ *cbuf);

bool dequeue_buffer(buffer_typ *cbuf, float* v);

void copy_buffer(buffer_typ *t, buffer_typ *s);

void print_buffer(buffer_typ *b);

#endif /* _MENU_UART_H_ */

