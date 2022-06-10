#ifndef TIMER_DISPLAY_H
#define TIMER_DISPLAY_H

#include "hwconfig.h"
#include "SPI_LCD/lcd_api.h"
#include "OutputDriver/OutputDriver.h"

#define ONE_SECOND 1000 // ms
#define ONE_MINUTE 60 // sec
#define ONE_HOUR 60 // min
#define ONE_DAY 24 // hr

#define DAY_X 73

#define ITEMS_PER_ALARM 12

typedef const char * String;

typedef enum
{
   SUNDAY = 0,
   MONDAY,
   TUESDAY,
   WEDNESDAY,
   THURSDAY,
   FRIDAY,
   SATURDAY,
   END_OF_WEEK
} Day;

typedef enum
{
   AM = 0,
   PM
} AM_PM;

typedef enum
{
   OnOff,
   Weekday,
   AmPm,
   Hour,
   Minute
} ItemType;

typedef enum
{
#define ALARM_MACRO(a,b,c,d,e,f,g,h) a,
#include "alarm_display_config.h"
#undef ALARM_MACRO
   NUMBER_OF_ITEMS
} AlarmDisplayItem_E;

typedef enum
{
#define ALARM_INIT_MACRO(n,h,t,o,m,sun,mon,tue,wed,thu,fri,sat,on,out) n,
#include "alarm_display_config.h"
#undef ALARM_INIT_MACRO
   NUMBER_OF_ALARMS
} Alarm_E;

typedef struct
{
   ItemType type;
   UInt8_T states;
   UInt8_T x_pos;
   UInt8_T y_pos;
   enum LCD_Font_E font;
   UInt8_T *data;
   AlarmDisplayItem_E control;
} AlarmDisplayItem_T;

typedef struct
{
   UInt8_T hours;
   UInt8_T minutes_tens;
   UInt8_T minutes_ones;
   AM_PM am_pm;
   Day day;
} TimeOfDay;

typedef struct
{
   TimeOfDay time;
   UInt8_T days[END_OF_WEEK];
   UInt8_T on;
   enum Output_Device_E output_device;
} Alarm;

#endif
