#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USI_I2C_Master.h"

#include "rtc3231.h"
#include "dst.h"

#define SDA_IN (DDRB) &= ~(1 << PINB5)
#define SDA_OUT (DDRB) |= (1 << (PINB5)) 
#define SCL_IN (DDRB) &=   ~( 1 << PINB7)
#define SCL_OUT (DDRB) |= (1 << (PINB7)) 

struct rtc_tm rtc_time;


void init(void);
void timer_init(void);
void io_init(void);
uint8_t number_to_7segment(uint8_t num);

void main(void) __attribute__ ((noreturn));

void main(void)
{
  uint8_t index = 0;

  init();
  
  rtc3231_read_datetime(&rtc_time);
  rtc_time.tm_hour += dst(&rtc_time);

  
  while (1) {
  
    for (index = 0; index < 4; index++)
    {
      PORTD = number_to_7segment(rtc_time.tm_hour / 10);
      PORTB = 0x01;
      _delay_ms(5);
      PORTD = number_to_7segment(rtc_time.tm_hour % 10 );
      PORTB = 0x01 << 1;
      _delay_ms(5);
      PORTD = number_to_7segment( rtc_time.tm_min / 10 );
      PORTB = 0x01 << 2;
      _delay_ms(5);
      PORTD = number_to_7segment(rtc_time.tm_min % 10);
      //PORTD = number_to_7segment(rtc_time.tm_mon % 10);
      PORTB = 0x01 << 3;
      _delay_ms(5);
      
    }
  }
}

void init(void)
{
  io_init();
  timer_init();
  i2c_init();

}

void io_init(void)
{
  DDRB = 0x0f;
  DDRD = 0xff;
  PORTB = 0x01;
  PORTD = 0xfe;
}

void timer_init(void){
  TCCR1B |= (1 << WGM12); // configure timer1 for CTC mode
  TIMSK |= (1 << OCIE1A); // enable the CTC interrupt
  
  TCCR1B |= ((1 << CS10) | (1 << CS12)); // start the timer at 1MHz/1024 = 976,5625
  
  OCR1A   = 977; // set the CTC compare value
  
  sei(); // enable global interrupts
}
uint8_t  number_to_7segment(uint8_t num){
  switch (num)
  {
    case 0:
    return 0b11000000;
    case 1:
    return 0b11111001;
    case 2:
    return 0b10100100;
    case 3:
    return 0b10110000;
    case 4:
    return 0b10011001;
    case 5:
    return 0b10010010;
    case 6:
    return 0b10000010;
    case 7:
    return 0b11111000;
    case 8:
    return 0b10000000;
    case 9:
    return 0b10010000;
    
    default:
    return 0x00;
  }
}


ISR(TIMER1_COMPA_vect ){
  rtc3231_read_datetime(&rtc_time);
  rtc_time.tm_isdst = dst(&rtc_time);
  if(rtc_time.tm_isdst == 1 ){
    rtc_time.tm_hour++;
  }
  
}