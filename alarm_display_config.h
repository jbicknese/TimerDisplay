
#ifdef ALARM_INIT_MACRO
//               Name,   Time,                   On,    Output driver
ALARM_INIT_MACRO(Alarm1, 11, 0, 0, AM, SATURDAY, TRUE,  OUT_1)
ALARM_INIT_MACRO(Alarm2,  6, 3, 0, AM, SUNDAY,   FALSE, OUT_2)
#endif

#ifdef ALARM_MACRO
//          Name,             Type,    States,  X Pos, Y Pos, Font,       Data,                         Control
ALARM_MACRO(Clock_day,        Weekday,      7,      8,    21, LARGE_FONT, &clock.day,                   NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_AM_PM,      AmPm,         2,     10,     9, LARGE_FONT, &clock.am_pm,                 NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_hours,      Hour,        12,      0,     0, LARGE_FONT, &clock.hours,                 NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_mins_tens,  Minute,       6,     21,     0, LARGE_FONT, &clock.minutes_tens,          NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_mins_ones,  Minute,      10,     29,     0, LARGE_FONT, &clock.minutes_ones,          NUMBER_OF_ITEMS)

ALARM_MACRO(Alarm1_on,        OnOff,        2,     43,     0, SMALL_FONT, &alarms[0].on,                Alarm1)
ALARM_MACRO(Alarm1_day,       Weekday,      7,    100,     8, SMALL_FONT, &alarms[0].time.day,          Alarm1_on)
ALARM_MACRO(Alarm1_AM_PM,     AmPm,         2,    115,     0, SMALL_FONT, &alarms[0].time.am_pm,        Alarm1_on)
ALARM_MACRO(Alarm1_hours,     Hour,        12,     88,     0, SMALL_FONT, &alarms[0].time.hours,        Alarm1_on)
ALARM_MACRO(Alarm1_mins_tens, Minute,       6,    103,     0, SMALL_FONT, &alarms[0].time.minutes_tens, Alarm1_on)
ALARM_MACRO(Alarm1_mins_ones, Minute,      10,    109,     0, SMALL_FONT, &alarms[0].time.minutes_ones, Alarm1_on)

ALARM_MACRO(Alarm2_on,        OnOff,        2,     43,    17, SMALL_FONT, &alarms[1].on,                Alarm2)
ALARM_MACRO(Alarm2_day,       Weekday,      7,    100,    25, SMALL_FONT, &alarms[1].time.day,          Alarm2_on)
ALARM_MACRO(Alarm2_AM_PM,     AmPm,         2,    115,    17, SMALL_FONT, &alarms[1].time.am_pm,        Alarm2_on)
ALARM_MACRO(Alarm2_hours,     Hour,        12,     88,    17, SMALL_FONT, &alarms[1].time.hours,        Alarm2_on)
ALARM_MACRO(Alarm2_mins_tens, Minute,       6,    103,    17, SMALL_FONT, &alarms[1].time.minutes_tens, Alarm2_on)
ALARM_MACRO(Alarm2_mins_ones, Minute,      10,    109,    17, SMALL_FONT, &alarms[1].time.minutes_ones, Alarm2_on)
#endif
