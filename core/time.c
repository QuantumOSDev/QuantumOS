/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */

#include <core/time.h>

#include <sys/cmos.h>

const unsigned char month_days[] = {
    31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31
};

unsigned long date_to_timestamp(date_t* date) {
    unsigned long unixtime = 0;

    unsigned long unixyears = date->year - 1970;
    unixtime += ((unixyears + 2) / 4) * 86400; 
    unixtime += unixyears * 86400 * 365;

    if (((date->year % 100) || (((date->year % 400) == 0))) && ((date->year % 4) == 0) && (date->month < 3)) { 
        unixtime -= 86400;
    }

    date->month--;
    while (date->month) {
        date->month--;
        unixtime += month_days[date->month] * 86400;
    }

    unixtime += (unsigned long)date->second;
    unixtime += ((unsigned long)date->minute) * 60;
    unixtime += ((unsigned long)date->hour) * 3600;
    unixtime += ((unsigned long)date->day - 1) * 86400;

    return unixtime;
}