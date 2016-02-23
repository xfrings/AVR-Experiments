/*-----------------------------------------------------------
- Pseudo-task scheduler

Author:    desai043
Created:   17-Feb-2016
Hardware:  ATMEGA32U4 on A-Star 32U4 Robot
           Controller LV with Raspberry Pi Bridge

           Note: LFUSE = 0xFF, HFUSE = 0xD0
           XTAL = 16MHz (CKDIV8 = 1 => SYSCLK = 16MHz)
-----------------------------------------------------------*/

#include "main.h"


/* Globals */
volatile uint64_t time_ms, green_led_toggles;
volatile uint8_t yellow_counter;
volatile bool run_htransform, run_toggle_red;
volatile button_stat_t button_a_stat;


/* Main */
int main()
{ 
   /* Init generic */
   initialize_basic();

   /* Startup */
   startup_appl();

   /* Init application specific */
   initialize_local();
   
   /* Enable interrupts */
   sei();

   /* Debug print buffers */
   //uint16_t dt;
   //char dbgbuf[50];

   /* Main loop */
   while(1)
   {
      /* Run red LED task */
      if(run_toggle_red)
      {
         task_1_toggle_red_led();
         run_toggle_red = false;
      }

      /* Run hough transform task */
      if(run_htransform)
      {
         for(int i = 0; i < 20; i++)
         {
            hough_transform((uint16_t)&red, (uint16_t)&green, (uint16_t)&blue);
         }
       
         run_htransform = false;
      }
   }
   
   return 0;
}


/* System vars re-init */
void reset_system_vars()
{
   time_ms = 0;
   yellow_counter = green_led_toggles = 0;
   run_htransform = run_toggle_red = false;
   button_a_stat = HIGH;

   /* Default shared data */
   shared_data.mod_red_led = 100;
   shared_data.mod_yelo_led = 4;
   shared_data.mod_h_trnsf = 100;
}


/* Task - Red LED */
void task_1_toggle_red_led()
{
   PORTB ^= (1 << EXT_RED);
}

/*-----------------------------------------------------------
                      HELPERS
-----------------------------------------------------------*/
void startup_appl()
{
   /* Clear all vars */
   reset_system_vars();

   /* Set port directions */
   DDRB |= ((1 << EXT_RED)|(1 << EXT_GREEN));
   DDRD |= (1 << EXT_YELLOW);

   /* Startup show */
   leds_turn_on();

   _delay_ms(1000);
   
   leds_turn_off();
}

/* Configure interrupts */
void initialize_local()
{
   /* Setup PCINTx interrupts for buttons */
   bool result = pcintx_enable_interrupt(PCINT3);

   /* Enable UART rx/tx interrupts */
   if(result)
   {
      result = usart_1_enable_interrupts();
   }

   /* Timer 0 - 1ms auto-reload */
   if(result)
   {
      result = timer_0_setup_autoreload(1);
   }

   /* Timer 1 interrupt - PWM */
   if(result)
   {
      timer_1_interrupt_enable('B');
   }

   /* Timer 1 - PWM - 100ms = 5Hz @ 50% duty cycle */
   if(result)
   {
      result = timer_1_setup_pfc_pwm(5, 50);
   }

   /* Timer 3 - 25ms auto-reload */
   if(result)
   {
      result = timer_3_setup_autoreload(25);
   }

   /* Initialize USART for communication */
   if(result)
   {
      result = usart_setup_configure(USART_DOUBLE_ASYNC);
   }

   if(!result)
   {
      throw_error(ERR_CONFIG);
   }   
}


/* LED ops */
void leds_turn_on()
{
   PORTB |= ((1 << EXT_RED)|(1 << EXT_GREEN));
   PORTD |= (1 << EXT_YELLOW); 
   PORTC |= (1 << LED_YELLOW);
   PORTD &= ~(1 << LED_GREEN);
}

void leds_turn_off()
{
   PORTB &= ~((1 << EXT_RED)|(1 << EXT_GREEN));
   PORTD &= ~(1 << EXT_YELLOW);
   PORTC &= ~(1 << LED_YELLOW);
   PORTD |= (1 << LED_GREEN);
}


/*-----------------------------------------------------------
             INTERRUPT SERVICE ROUTINES
-----------------------------------------------------------*/
/* Timer 0 compare A interrupt */
ISR(TIMER0_COMPA_vect)
{
   /* time_ms keeper */
   time_ms++;

   /* Manage experimentation timing */
   if(exp_db.running)
   {
      if(exp_db.time_to_finish > 0)
      {
         exp_db.time_to_finish--
      }
      else
      {
         exp_db.running = false;
      }
   }

   /* Red LED task release? */
   if(time_ms % shared_data.mod_red_led == 0)
   {
      /* Missed deadline? */
      if(run_toggle_red == true)
      {
         exp_db.task[TSK_REDLED].missed_deadlines++;
      }
      run_toggle_red = true;
   }

   /* Hough transform task release? */
   if(time_ms % shared_data.mod_h_trnsf == 0)
   {
      /* Missed deadline? */
      if(run_htransform == true)
      {
         exp_db.task[TSK_HTRNSF].missed_deadlines++;
      }
      run_htransform = true;
   }
}


/* Timer 1 compare B interrupt */
ISR(TIMER1_COMPB_vect)
{
   /* Green LED toggles' keeper */
   green_led_toggles++;
}


/* Timer 3 compare A interrupt */
ISR(TIMER3_COMPA_vect)
{
   /* Yellow LED task keeper */
   yellow_counter++;
   
   /* Yellow LED task */
   if(yellow_counter % shared_data.mod_yelo_led == 0)
   {
      PORTD ^= (1 << EXT_YELLOW);
   }

   /* Jitter LED task */
   if(rand() % 5 == 4)
   {
      PORTC |= (1 << LED_YELLOW);
      _delay_ms(5);
      PORTC &= ~(1 << LED_YELLOW);
   }
}


/* ISR - Pin Change Interrupt */
/* All PCINTx detections are vectored here */
ISR(PCINT0_vect)
{
   button_stat_t button_a_now;

   /* Button C */
   if(PINB & (1 << BUTTON_A))
   {
      button_a_now = HIGH;
   }
   else
   {  
      button_a_now = LOW;
   }

   /* HIGH -> LOW = Press */
   if(button_a_stat == HIGH && button_a_now == LOW)
   {
      _delay_ms(DEBOUNCE_DELAY);
      
      /* Sample again */
      if(!(PINB & (1 << BUTTON_A)))
      {
          button_a_stat = LOW;
      }
   }
   /* LOW -> HIGH = release */
   else if(button_a_stat == LOW && button_a_now == HIGH)
   {
      _delay_ms(DEBOUNCE_DELAY);
      button_a_stat = HIGH;

      /* Halt system */
      timer_0_interrupt_disable();
      timer_3_interrupt_disable();
      pcintx_disable_interrupt(PCINT3);

      /* Throw experimentation prompt */
      sei();
      menu_uart_prompt();

      /* Resume system */
      timer_0_interrupt_enable();
      timer_3_interrupt_enable();
      pcintx_enable_interrupt(PCINT3);
   }
}

