#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "USI_I2C_Master.h"
//#include "rtc.h"

#define DS3231_I2C_ADDR_READ 0xd1
#define DS3231_I2C_ADDR_WRITE 0xd0
#define SDA_IN (DDRB) &= ~(1 << PINB5)
#define SDA_OUT (DDRB) |= (1 << (PINB5)) 
#define SCL_IN (DDRB) &=   ~( 1 << PINB7)
#define SCL_OUT (DDRB) |= (1 << (PINB7)) 

// Global variables
static uint8_t seconds = 0; 
static uint8_t minutes = 24; 
static uint8_t hours  = 9;
static uint8_t  error_flag = 0;



void init(void);
void timer_init(void);
void io_init(void);
uint8_t number_to_7segment(uint8_t num);
void main(void) __attribute__ ((noreturn));

void main(void)
{
  uint8_t index = 0;

  init();
  //_delay_ms(500);
  error_flag =  i2c_start(DS3231_I2C_ADDR_WRITE); // start with device address containing read bit
  i2c_write(0x00);
  i2c_rep_start(DS3231_I2C_ADDR_READ);
  seconds = i2c_read(0); //read one byte with ack  
  minutes = i2c_read(0); //read one byte with ack  
 hours = i2c_read(1); //read one byte with nack  
  i2c_stop();

 seconds = ((seconds & 0xF0) >> 4) * 10 + (seconds & 0x0F);
 uint8_t tenth_hours = (hours & 0xF0) >> 4  ;
 hours = hours & 0x0F;
 uint8_t tenth_minutes =  (minutes & 0xF0) >> 4; 
  minutes = minutes & 0x0F;
hours = ((tenth_hours * 10) + hours);
minutes=  ((tenth_minutes * 10) + minutes);

  while (1) {
    
    
    
    //Try to show something on 7segment
    for (index = 0; index < 4; index++)
    {
      PORTD = number_to_7segment(hours / 10);
      PORTB = 0x01;
      _delay_ms(5);
      PORTD = number_to_7segment(hours % 10 );
      PORTB = 0x01 << 1;
      _delay_ms(5);
      PORTD = number_to_7segment( minutes / 10 );
      PORTB = 0x01 << 2;
      _delay_ms(5);
      PORTD = number_to_7segment(minutes % 10);
      PORTB = 0x01 << 3;
      _delay_ms(5);
    
    }
    
    if (seconds > 59)
    {
      seconds = 0;
      minutes++;
    }
    if(minutes >  59){
      minutes = 0;
      hours++;
    }
    if(hours > 23){
      hours = 0;
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
  seconds++; // add one second
  }