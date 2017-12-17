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

#ifndef __DS3231_H__
#define __DS3231_H__

#include <stdint.h>
#include "dst.h"


#define RTC_WADDR 0xd0
#define RTC_RADDR 0xd1


/*
 * Clock initialization
 */
 void rtc3231_init(void);

// /**
//  * Reading time from clock
//  * @time: time structure with reading data
//  */

void rtc3231_read_time(struct rtc_tm *time);

// /**
//  * Reading date from clock
//  * @date: date structure with reading data
//  */
// void rtc3231_read_date(struct rtc_date *date);

/**
 * Reading date and time from clock
 * @time datetime structure
 */
void rtc3231_read_datetime(struct rtc_tm *time);
// /**
//  * Writing new time in clock
//  * @time: time structure for writing data
//  */
// void rtc3231_write_time(struct rtc_tm *time);

// /**
//  * Writing new date in clock
//  * @date: date structure for writing data
//  */
// void rtc3231_write_date(struct rtc_date *date);


#endif
