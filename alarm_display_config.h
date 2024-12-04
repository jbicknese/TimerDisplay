
#ifdef ALARM_INIT_MACRO
//               Name,   Time,         Sun,  Mon,  Tue,  Wed,  Thu,  Fri,  Sat,  On,    State
ALARM_INIT_MACRO(Alarm1, 11, 0, 0, AM, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE,  AlarmOff)
ALARM_INIT_MACRO(Alarm2,  6, 3, 0, AM, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, TRUE, FALSE, AlarmOff)
#endif

#ifdef ALARM_MACRO
//          Name,             Type,    States,  X Pos, Y Pos, Font,       Data,                              Control
ALARM_MACRO(Clock_hours,      Hour,        12,      0,     0, LARGE_FONT, &clock.hours,                      NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_mins_tens,  Minute,       6,     21,     0, LARGE_FONT, &clock.minutes_tens,               NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_mins_ones,  Minute,      10,     29,     0, LARGE_FONT, &clock.minutes_ones,               NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_AM_PM,      AmPm,         2,     10,     9, LARGE_FONT, &clock.am_pm,                      NUMBER_OF_ITEMS)
ALARM_MACRO(Clock_day,        Weekday,      7,      8,    21, LARGE_FONT, &clock.day,                        NUMBER_OF_ITEMS)

ALARM_MACRO(Alarm1_on,        OnOff,        2,     43,     0, SMALL_FONT, &alarms[Alarm1].on,                Alarm1)
ALARM_MACRO(Alarm1_hours,     Hour,        12,     88,     0, SMALL_FONT, &alarms[Alarm1].time.hours,        Alarm1_on)
ALARM_MACRO(Alarm1_mins_tens, Minute,       6,    103,     0, SMALL_FONT, &alarms[Alarm1].time.minutes_tens, Alarm1_on)
ALARM_MACRO(Alarm1_mins_ones, Minute,      10,    109,     0, SMALL_FONT, &alarms[Alarm1].time.minutes_ones, Alarm1_on)
ALARM_MACRO(Alarm1_AM_PM,     AmPm,         2,    115,     0, SMALL_FONT, &alarms[Alarm1].time.am_pm,        Alarm1_on)
ALARM_MACRO(Alarm1_Sunday,    Weekday,      2,  DAY_X,     8, SMALL_FONT, &alarms[Alarm1].days[SUNDAY],      Alarm1_on)
ALARM_MACRO(Alarm1_Monday,    Weekday,      2,  DAY_X+8,   8, SMALL_FONT, &alarms[Alarm1].days[MONDAY],      Alarm1_on)
ALARM_MACRO(Alarm1_Tuesday,   Weekday,      2,  DAY_X+16,  8, SMALL_FONT, &alarms[Alarm1].days[TUESDAY],     Alarm1_on)
ALARM_MACRO(Alarm1_Wednesday, Weekday,      2,  DAY_X+24,  8, SMALL_FONT, &alarms[Alarm1].days[WEDNESDAY],   Alarm1_on)
ALARM_MACRO(Alarm1_Thursday,  Weekday,      2,  DAY_X+32,  8, SMALL_FONT, &alarms[Alarm1].days[THURSDAY],    Alarm1_on)
ALARM_MACRO(Alarm1_Friday,    Weekday,      2,  DAY_X+40,  8, SMALL_FONT, &alarms[Alarm1].days[FRIDAY],      Alarm1_on)
ALARM_MACRO(Alarm1_Saturday,  Weekday,      2,  DAY_X+48,  8, SMALL_FONT, &alarms[Alarm1].days[SATURDAY],    Alarm1_on)

ALARM_MACRO(Alarm2_on,        OnOff,        2,     43,    17, SMALL_FONT, &alarms[Alarm2].on,                Alarm2)
ALARM_MACRO(Alarm2_hours,     Hour,        12,     88,    17, SMALL_FONT, &alarms[Alarm2].time.hours,        Alarm2_on)
ALARM_MACRO(Alarm2_mins_tens, Minute,       6,    103,    17, SMALL_FONT, &alarms[Alarm2].time.minutes_tens, Alarm2_on)
ALARM_MACRO(Alarm2_mins_ones, Minute,      10,    109,    17, SMALL_FONT, &alarms[Alarm2].time.minutes_ones, Alarm2_on)
ALARM_MACRO(Alarm2_AM_PM,     AmPm,         2,    115,    17, SMALL_FONT, &alarms[Alarm2].time.am_pm,        Alarm2_on)
ALARM_MACRO(Alarm2_Sunday,    Weekday,      2,  DAY_X,    25, SMALL_FONT, &alarms[Alarm2].days[SUNDAY],      Alarm2_on)
ALARM_MACRO(Alarm2_Monday,    Weekday,      2,  DAY_X+8,  25, SMALL_FONT, &alarms[Alarm2].days[MONDAY],      Alarm2_on)
ALARM_MACRO(Alarm2_Tuesday,   Weekday,      2,  DAY_X+16, 25, SMALL_FONT, &alarms[Alarm2].days[TUESDAY],     Alarm2_on)
ALARM_MACRO(Alarm2_Wednesday, Weekday,      2,  DAY_X+24, 25, SMALL_FONT, &alarms[Alarm2].days[WEDNESDAY],   Alarm2_on)
ALARM_MACRO(Alarm2_Thursday,  Weekday,      2,  DAY_X+32, 25, SMALL_FONT, &alarms[Alarm2].days[THURSDAY],    Alarm2_on)
ALARM_MACRO(Alarm2_Friday,    Weekday,      2,  DAY_X+40, 25, SMALL_FONT, &alarms[Alarm2].days[FRIDAY],      Alarm2_on)
ALARM_MACRO(Alarm2_Saturday,  Weekday,      2,  DAY_X+48, 25, SMALL_FONT, &alarms[Alarm2].days[SATURDAY],    Alarm2_on)


#endif
