/*
 * QuantumOS Copyright (c) 2021-2022
 *  - Solindek <solindeklive.biznes@gmail.com>
 */


#include <core/stdlib.h>
#include <core/string.h>

#include <sys/cmos.h>
#include <sys/pio.h>

const static time_zone_struct_t time_zones[TIME_ZONES_UTC_PLUS_14 + 1] = {
    [TIME_ZONES_UTC_MINUS_12]   = {
        .offset_hour = -12,
        .offset_minute = 0,
    },  
    [TIME_ZONES_UTC_MINUS_11]   = {
        .offset_hour = -11,
        .offset_minute = 0,
    },  
    [TIME_ZONES_UTC_MINUS_10]   = {
        .offset_hour = -10,
        .offset_minute = 0,
    },  
    [TIME_ZONES_UTC_MINUS_9_30] = {
        .offset_hour = -9,
        .offset_minute = -30,
    },
    [TIME_ZONES_UTC_MINUS_9]    = {
        .offset_hour = -9,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_8]    = {
        .offset_hour = -8,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_7]    = {
        .offset_hour = -7,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_6]    = {
        .offset_hour = -6,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_5]    = {
        .offset_hour = -5,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_4]    = {
        .offset_hour = -4,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_3_30] = {
        .offset_hour = -3,
        .offset_minute = -30,
    },
    [TIME_ZONES_UTC_MINUS_3]    = {
        .offset_hour = -3,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_2]    = {
        .offset_hour = -2,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_MINUS_1]    = {
        .offset_hour = -1,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_0]          = {
        .offset_hour = 0,
        .offset_minute = 0,
    },         
    [TIME_ZONES_UTC_PLUS_1]     = {
        .offset_hour = 1,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_2]     = {
        .offset_hour = 2,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_3]     = {
        .offset_hour = 3,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_3_30]  = {
        .offset_hour = 3,
        .offset_minute = 30,
    }, 
    [TIME_ZONES_UTC_PLUS_4]     = {
        .offset_hour = 4,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_4_30]  = {
        .offset_hour = 4,
        .offset_minute = 30,
    }, 
    [TIME_ZONES_UTC_PLUS_5]     = {
        .offset_hour = 5,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_5_30]  = {
        .offset_hour = 5,
        .offset_minute = 30,
    }, 
    [TIME_ZONES_UTC_PLUS_5_45]  = {
        .offset_hour = 5,
        .offset_minute = 45,
    }, 
    [TIME_ZONES_UTC_PLUS_6]     = {
        .offset_hour = 6,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_6_30]  = {
        .offset_hour = 6,
        .offset_minute = 30,
    }, 
    [TIME_ZONES_UTC_PLUS_7]     = {
        .offset_hour = 7,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_8]     = {
        .offset_hour = 8,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_8_45]  = {
        .offset_hour = 8,
        .offset_minute = 45,
    }, 
    [TIME_ZONES_UTC_PLUS_9]     = {
        .offset_hour = 9,
        .offset_minute = 0,
    },    
    [TIME_ZONES_UTC_PLUS_9_30]  = {
        .offset_hour = 9,
        .offset_minute = 30,
    }, 
    [TIME_ZONES_UTC_PLUS_10]    = {
        .offset_hour = 10,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_PLUS_10_30] = {
        .offset_hour = 10,
        .offset_minute = 30,
    },
    [TIME_ZONES_UTC_PLUS_11]    = {
        .offset_hour = 11,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_PLUS_12]    = {
        .offset_hour = 12,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_PLUS_12_45] = {
        .offset_hour = 12,
        .offset_minute = 45,
    },
    [TIME_ZONES_UTC_PLUS_13]    = {
        .offset_hour = 13,
        .offset_minute = 0,
    },   
    [TIME_ZONES_UTC_PLUS_14]    = {
        .offset_hour = 14,
        .offset_minute = 0,
    }   
};

static time_zone_t global_time_zone = TIME_ZONES_UTC_0;

BOOL get_update_in_progress_flag() 
{
    pio_outb(CMOS_ADDRESS, 0x0A);
    return pio_inb(CMOS_DATA) & 0x80;
}

unsigned char get_rtc_register(int reg) 
{
    pio_outb(CMOS_ADDRESS, reg);
    return pio_inb(CMOS_DATA);
}

void quantum_time_init(time_zone_t time_zone)
{
    global_time_zone = time_zone;
}

void change_date_as_time_zone(date_t* date)
{
    time_zone_struct_t time_zone = time_zones[global_time_zone];
    int rest = 0;

    date->minute += time_zone.offset_minute;
    date->hour   += time_zone.offset_hour;

    if (date->minute >= 60)
    {
        rest = date->minute - 60;
        date->minute = rest;
        for (int i = 0; i < (int)rest / 60; i++)
            date->hour++;
    }
    else if (date->hour >= 24)
    {
        rest = date->hour - 24;
        date->hour = rest;
        for (int i = 0; i < (int)rest / 24; i++)
            date->day++;
    }
}

void set_time_zone(time_zone_t __time_zone) 
{
    global_time_zone = __time_zone;
}

time_zone_t convert_str_to_time_zone(char* time_zone_str)
{
    if      (strcmp(time_zone_str, "UTC -12:00") == 0) return TIME_ZONES_UTC_MINUS_12;    // UTC -12:00
    else if (strcmp(time_zone_str, "UTC -11:00") == 0) return TIME_ZONES_UTC_MINUS_11;    // UTC -11:00
    else if (strcmp(time_zone_str, "UTC -10:00") == 0) return TIME_ZONES_UTC_MINUS_10;    // UTC -10:00
    else if (strcmp(time_zone_str, "UTC -09:30") == 0) return TIME_ZONES_UTC_MINUS_9_30;  // UTC -09:30
    else if (strcmp(time_zone_str, "UTC -09:00") == 0) return TIME_ZONES_UTC_MINUS_9;     // UTC -09:00
    else if (strcmp(time_zone_str, "UTC -08:00") == 0) return TIME_ZONES_UTC_MINUS_8;     // UTC -08:00
    else if (strcmp(time_zone_str, "UTC -07:00") == 0) return TIME_ZONES_UTC_MINUS_7;     // UTC -07:00
    else if (strcmp(time_zone_str, "UTC -06:00") == 0) return TIME_ZONES_UTC_MINUS_6;     // UTC -06:00
    else if (strcmp(time_zone_str, "UTC -05:00") == 0) return TIME_ZONES_UTC_MINUS_5;     // UTC -05:00
    else if (strcmp(time_zone_str, "UTC -04:00") == 0) return TIME_ZONES_UTC_MINUS_4;     // UTC -04:00
    else if (strcmp(time_zone_str, "UTC -03:30") == 0) return TIME_ZONES_UTC_MINUS_3_30;  // UTC -03:30
    else if (strcmp(time_zone_str, "UTC -03:00") == 0) return TIME_ZONES_UTC_MINUS_3;     // UTC -03:00
    else if (strcmp(time_zone_str, "UTC -02:00") == 0) return TIME_ZONES_UTC_MINUS_2;     // UTC -02:00
    else if (strcmp(time_zone_str, "UTC -01:00") == 0) return TIME_ZONES_UTC_MINUS_1;     // UTC -01:00
    else if (strcmp(time_zone_str, "UTC 0")      == 0) return TIME_ZONES_UTC_0;           // UTC 0
    else if (strcmp(time_zone_str, "UTC -01:00") == 0) return TIME_ZONES_UTC_PLUS_1;      // UTC +01:00
    else if (strcmp(time_zone_str, "UTC +02:00") == 0) return TIME_ZONES_UTC_PLUS_2;      // UTC +02:00
    else if (strcmp(time_zone_str, "UTC +03:00") == 0) return TIME_ZONES_UTC_PLUS_3;      // UTC +03:00
    else if (strcmp(time_zone_str, "UTC +03:30") == 0) return TIME_ZONES_UTC_PLUS_3_30;   // UTC +03:30
    else if (strcmp(time_zone_str, "UTC +04:00") == 0) return TIME_ZONES_UTC_PLUS_4;      // UTC +04:00
    else if (strcmp(time_zone_str, "UTC +04:30") == 0) return TIME_ZONES_UTC_PLUS_4_30;   // UTC +04:30
    else if (strcmp(time_zone_str, "UTC +05:00") == 0) return TIME_ZONES_UTC_PLUS_5;      // UTC +05:00
    else if (strcmp(time_zone_str, "UTC +05:30") == 0) return TIME_ZONES_UTC_PLUS_5_30;   // UTC +05:30
    else if (strcmp(time_zone_str, "UTC +05:45") == 0) return TIME_ZONES_UTC_PLUS_5_45;   // UTC +05:45
    else if (strcmp(time_zone_str, "UTC +06:00") == 0) return TIME_ZONES_UTC_PLUS_6;      // UTC +06:00
    else if (strcmp(time_zone_str, "UTC +06:30") == 0) return TIME_ZONES_UTC_PLUS_6_30;   // UTC +06:30
    else if (strcmp(time_zone_str, "UTC +07:00") == 0) return TIME_ZONES_UTC_PLUS_7;      // UTC +07:00
    else if (strcmp(time_zone_str, "UTC +08:00") == 0) return TIME_ZONES_UTC_PLUS_8;      // UTC +08:00
    else if (strcmp(time_zone_str, "UTC +08:45") == 0) return TIME_ZONES_UTC_PLUS_8_45;   // UTC +08:45
    else if (strcmp(time_zone_str, "UTC +09:00") == 0) return TIME_ZONES_UTC_PLUS_9;      // UTC +09:00
    else if (strcmp(time_zone_str, "UTC +09:30") == 0) return TIME_ZONES_UTC_PLUS_9_30;   // UTC +09:30
    else if (strcmp(time_zone_str, "UTC +10:00") == 0) return TIME_ZONES_UTC_PLUS_10;     // UTC +10:00
    else if (strcmp(time_zone_str, "UTC +10:30") == 0) return TIME_ZONES_UTC_PLUS_10_30;  // UTC +10:30
    else if (strcmp(time_zone_str, "UTC +11:00") == 0) return TIME_ZONES_UTC_PLUS_11;     // UTC +11:00
    else if (strcmp(time_zone_str, "UTC +12:00") == 0) return TIME_ZONES_UTC_PLUS_12;     // UTC +12:00
    else if (strcmp(time_zone_str, "UTC +12:45") == 0) return TIME_ZONES_UTC_PLUS_12_45;  // UTC +12:45
    else if (strcmp(time_zone_str, "UTC +13:00") == 0) return TIME_ZONES_UTC_PLUS_13;     // UTC +13:00
    else if (strcmp(time_zone_str, "UTC +14:00") == 0) return TIME_ZONES_UTC_PLUS_14;     // UTC +14:00
    else                                               return (time_zone_t)-1;
}

date_t get_date_cmos() 
{
    date_t date;
    while (get_update_in_progress_flag());
    date.second      = get_rtc_register(RTC_REGISTER_SECOND);
    date.minute      = get_rtc_register(RTC_REGISTER_MINUT);
    date.hour        = get_rtc_register(RTC_REGISTER_HOUR);
    date.day         = get_rtc_register(RTC_REGISTER_DAY);
    date.month       = get_rtc_register(RTC_REGISTER_MONTH);
    date.year        = get_rtc_register(RTC_REGISTER_YEAR);
    date.century     = 20; // em change in 2100 ;)

    unsigned char register_b = get_rtc_register(RTC_REGISTER_B);
    if  (!(register_b & 0x04))
{
        date.second = (date.second & 0x0F) + ((date.second / 16) * 10);
        date.minute = (date.minute & 0x0F) + ((date.minute / 16) * 10);
        date.hour = ( (date.hour & 0x0F) + (((date.hour & 0x70) / 16) * 10) ) | (date.hour & 0x80);
        date.day = (date.day & 0x0F) + ((date.day / 16) * 10);
        date.month = (date.month & 0x0F) + ((date.month / 16) * 10);
        date.year = (date.year & 0x0F) + ((date.year / 16) * 10);
    }

    change_date_as_time_zone(&date);

    return date;
}