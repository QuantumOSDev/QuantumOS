#ifndef CMOS_H
#define CMOS_H

#include <core/stdlib.h>

#define CMOS_ADDRESS 0x70
#define CMOS_DATA    0x71

#define RTC_REGISTER_SECOND  0x00
#define RTC_REGISTER_MINUT   0x02
#define RTC_REGISTER_HOUR    0x04
#define RTC_REGISTER_DAY     0x07
#define RTC_REGISTER_MONTH   0x08
#define RTC_REGISTER_YEAR    0x09
#define RTC_REGISTER_CENTURY NULL

#define RTC_REGISTER_MEMORY_LOW  0x30
#define RTC_REGISTER_MEMORY_HIGH 0x31

#define RTC_REGISTER_B       0x0B

typedef struct __date_t {
    int milisecond;
    int second;
    int minute;
    int hour;
    int day;
    int month;
    int year;
    int century;
} date_t;

typedef enum __time_zone_t {
    TIME_ZONES_UTC_MINUS_12,   // UTC -12:00  
    TIME_ZONES_UTC_MINUS_11,   // UTC -11:00
    TIME_ZONES_UTC_MINUS_10,   // UTC -10:00
    TIME_ZONES_UTC_MINUS_9_30, // UTC -09:30
    TIME_ZONES_UTC_MINUS_9,    // UTC -09:00
    TIME_ZONES_UTC_MINUS_8,    // UTC -08:00
    TIME_ZONES_UTC_MINUS_7,    // UTC -07:00
    TIME_ZONES_UTC_MINUS_6,    // UTC -06:00
    TIME_ZONES_UTC_MINUS_5,    // UTC -05:00
    TIME_ZONES_UTC_MINUS_4,    // UTC -04:00
    TIME_ZONES_UTC_MINUS_3_30, // UTC -03:30
    TIME_ZONES_UTC_MINUS_3,    // UTC -03:00
    TIME_ZONES_UTC_MINUS_2,    // UTC -02:00
    TIME_ZONES_UTC_MINUS_1,    // UTC -01:00
    TIME_ZONES_UTC_0,          // UTC -00:00
    TIME_ZONES_UTC_PLUS_1,     // UTC +01:00
    TIME_ZONES_UTC_PLUS_2,     // UTC +02:00
    TIME_ZONES_UTC_PLUS_3,     // UTC +03:00
    TIME_ZONES_UTC_PLUS_3_30,  // UTC +03:30
    TIME_ZONES_UTC_PLUS_4,     // UTC +04:00
    TIME_ZONES_UTC_PLUS_4_30,  // UTC +04:30
    TIME_ZONES_UTC_PLUS_5,     // UTC +05:00
    TIME_ZONES_UTC_PLUS_5_30,  // UTC +05:30
    TIME_ZONES_UTC_PLUS_5_45,  // UTC +05:45
    TIME_ZONES_UTC_PLUS_6,     // UTC +06:00
    TIME_ZONES_UTC_PLUS_6_30,  // UTC +06:30
    TIME_ZONES_UTC_PLUS_7,     // UTC +07:00
    TIME_ZONES_UTC_PLUS_8,     // UTC +08:00
    TIME_ZONES_UTC_PLUS_8_45,  // UTC +08:45
    TIME_ZONES_UTC_PLUS_9,     // UTC +09:00
    TIME_ZONES_UTC_PLUS_9_30,  // UTC +09:30
    TIME_ZONES_UTC_PLUS_10,    // UTC +10:00
    TIME_ZONES_UTC_PLUS_10_30, // UTC +10:30
    TIME_ZONES_UTC_PLUS_11,    // UTC +11:00
    TIME_ZONES_UTC_PLUS_12,    // UTC +12:00
    TIME_ZONES_UTC_PLUS_12_45, // UTC +12:45
    TIME_ZONES_UTC_PLUS_13,    // UTC +13:00
    TIME_ZONES_UTC_PLUS_14,    // UTC +14:00
} time_zone_t;

typedef struct __time_zone_struct_t {
    int offset_hour;
    int offset_minute;
} time_zone_struct_t;

void quantum_time_init(); 

BOOL get_update_in_progress_flag();

unsigned char get_rtc_register(int reg);

date_t get_date_cmos();

#endif