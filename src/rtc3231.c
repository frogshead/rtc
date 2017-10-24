/* DS3231 Real Time Clock AVR Lirary
 *
 * Copyright (C) 2016-2017 Sergey Denisov.
 * Written by Sergey Denisov aka LittleBuster (DenisovS21@gmail.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public Licence
 * as published by the Free Software Foundation; either version 3
 * of the Licence, or (at your option) any later version.
 *
 * Original library written by Adafruit Industries. MIT license.
 */

#include "rtc3231.h"
#include "USI_I2C_Master.h"



static unsigned char bcd (unsigned char data)
{
	unsigned char bc;

	bc = ((((data & (1 << 6)) | (data & (1 << 5)) | (data & (1 << 4)))*0x0A) >> 4)
	+ ((data & (1 << 3))|(data & (1 << 2))|(data & (1 << 1))|(data & 0x01));

  return bc;
}

static unsigned char bin(unsigned char dec)
{
	char bcd;
	char n, dig, num, count;

	num = dec;
	count = 0;
	bcd = 0;

	for (n = 0; n < 4; n++) {
		dig = num % 10;
		num = num / 10;
		bcd = (dig << count) | bcd;
		count += 4;
	}
	return bcd;
}


void rtc3231_init(void)
{
	
	i2c_start(RTC_WADDR);
	i2c_write(0x0E);
	i2c_write(0x20);
	i2c_write(0x08);
	i2c_stop();
}

void rtc3231_read_time(struct rtc_tm *time)
{
	i2c_init(); // Why this is needed?
	i2c_start(RTC_WADDR);
	i2c_write(0x00);
	i2c_rep_start(RTC_RADDR);

	time->tm_sec = bcd(i2c_read(0));
	time->tm_min = bcd(i2c_read(0));
	time->tm_hour = bcd(i2c_read(1));
	i2c_stop();
}

// void rtc3231_read_date(struct rtc_date *date)
// {
	
// 	i2c_start(RTC_WADDR);
// 	i2c_write(0x00);
// 	i2c_stop();

	
// 	i2c_start(RTC_RADDR);
// 	bcd(i2c_read(0));
// 	bcd(i2c_read(0));
// 	bcd(i2c_read(0));

// 	date->wday = bcd(i2c_read(0));
// 	date->day = bcd(i2c_read(0));
// 	date->month = bcd(i2c_read(0));
// 	date->year = bcd(i2c_read(1));
// 	i2c_stop();
// }

void rtc3231_read_datetime(struct rtc_tm *time)
{
	i2c_init();
	i2c_start(RTC_WADDR);
	i2c_write(0x00);
	i2c_stop();

	
	i2c_start(RTC_RADDR);
	time->tm_sec = bcd(i2c_read(0));
	time->tm_min = bcd(i2c_read(0));
	time->tm_hour = bcd(i2c_read(0));

	time->tm_wday = bcd(i2c_read(0)); //Day of the week 1-7
	time->tm_mday = bcd(i2c_read(0)); // Day of the month 01-31
	time->tm_mon = bcd(i2c_read(0));  //Month 01-12
	time->tm_year = bcd(i2c_read(1)); // Year 00-99
	
	i2c_stop();
}

void rtc3231_write_time(struct rtc_tm *time)
{
    
    i2c_start(RTC_WADDR);
    i2c_write(0x00);
    i2c_write(bin(time->tm_sec));
	i2c_write(bin(time->tm_min));
    i2c_write(bin(time->tm_hour));
    i2c_stop();
}

// void rtc3231_write_date(struct rtc_date *date)
// {

//     i2c_start(RTC_WADDR);
//     i2c_write(0x03);
//     i2c_write(bin(date->wday));
//     i2c_write(bin(date->day));
// 	i2c_write(bin(date->month));
// 	i2c_write(bin(date->year));
//     i2c_stop();
// }
