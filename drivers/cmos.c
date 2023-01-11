#include <drivers/cmos.h>
#include <sys/pio.h>

int get_update_in_progress_flag() {
    pio_outb(CMOS_ADDRESS, 0x0A);
    return pio_inb(CMOS_DATA) & 0x80;
}

unsigned char get_rtc_register(int reg) {
    pio_outb(CMOS_ADDRESS, reg);
    return pio_inb(CMOS_DATA);
}

date_t get_date_cmos() {
    date_t date;
    while (get_update_in_progress_flag());
    date.second  = get_rtc_register(RTC_REGISTER_SECOND);
    date.minute  = get_rtc_register(RTC_REGISTER_MINUTE);
    date.hour    = get_rtc_register(RTC_REGISTER_HOUR);
    date.day     = get_rtc_register(RTC_REGISTER_DAY);
    date.month   = get_rtc_register(RTC_REGISTER_MONTH);
    date.year    = get_rtc_register(RTC_REGISTER_YEAR);
    date.century = 20;

    unsigned char register_b = get_rtc_register(RTC_REGISTER_B);
    if  (!(register_b & 0x04)) {
        date.second = (date.second & 0x0F) + ((date.second / 16) * 10);
        date.minute = (date.minute & 0x0F) + ((date.minute / 16) * 10);
        date.hour = ( (date.hour & 0x0F) + (((date.hour & 0x70) / 16) * 10) ) | (date.hour & 0x80);
        date.day = (date.day & 0x0F) + ((date.day / 16) * 10);
        date.month = (date.month & 0x0F) + ((date.month / 16) * 10);
        date.year = (date.year & 0x0F) + ((date.year / 16) * 10);
    }

    return date;
}