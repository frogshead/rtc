#include <stdint.h>
#include "dst.h"


// Original :
//https://github.com/vancegroup-mirrors/avr-libc/blob/d585f13e7db9151a2e1ad40f943eadb924777444/avr-libc/include/util/eu_dst.h
//But it has bug:
//https://savannah.nongnu.org/bugs/?44327
// So this is modified version from the original. No need for timezone info.
//
uint8_t dst(struct rtc_tm *time)
{

    uint8_t month, mday, hour, day_of_week;
   

    /* obtain the variables */

    month = time->tm_mon;        // month
    /* RTC has 1 for Monday and 7 for Sunday  */
    day_of_week = time->tm_wday == 7 ? 0 : time->tm_wday; // day of the week
    mday = time->tm_mday;    // dsy of the month
    hour = time->tm_hour;

    if ((month > 3) && (month < 10))
        return 1;

    if (month < 3)
        return 0;
    if (month > 10)
        return 0;
    
    // determine next sunday
    int ns = ((7 - time->tm_wday) + time->tm_mday);
    // first sunday
    int fs = ns % 7;
    // Last sunday
    int ls = 4 * 7 + fs > 31 ? 3 * 7 +  fs : 4 * 7 + fs;
    
    if (month == 3)
    {
        if (ls < mday)
            return 1;
        if (ls > mday)
            return 0;
        if (hour < 1)
            return 0;
        return 1;
    }
    // MARCH
    if (ls < mday)
        return 0;
    if (ls > mday)
        return 1;
    if (hour < 1)
        return 1;
    return 0;
}