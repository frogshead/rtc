
#ifndef __DST_H__
#define __DST_H__


struct rtc_tm {
    int tm_sec;         /* seconds,  range 0 to 59          */
    int tm_min;         /* minutes, range 0 to 59           */
    int tm_hour;        /* hours, range 0 to 23             */
    int tm_mday;        /* day of the month, range 1 to 31  */
    int tm_mon;         /* month, range 1 to 12             */
    int tm_year;        /* The number of years since 1900   */
    int tm_wday;        /* day of the week, range 1 to 7     7 == Sunday*/
    int tm_yday;        /* day in the year, range 0 to 365  */
    int tm_isdst;       /* daylight saving time             */
};



uint8_t dst(struct rtc_tm * time);

#endif