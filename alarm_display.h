#ifndef TIMER_DISPLAY_H
#define TIMER_DISPLAY_H

#include "hwconfig.h"
#include "SPI_LCD/lcd_api.h"
#include "OutputDriver/OutputDriver.h"
#include "app_api.h"

#ifdef UNIT_TEST
#define UNIT_TEST_STATIC
#else
#define UNIT_TEST_STATIC static
#endif

#define BUTTON_IS_PRESSED (ReadVar(SensorInput) < BUTTON_PRESSED_THRESHOLD)

#define HALF_SECOND 500 // ms
#define ONE_SECOND 1000 // ms
#define ONE_MINUTE 60 // sec
#define ONE_HOUR 60 // min
#define ONE_DAY 24 // hr

#define DAY_X 73

#define ITEMS_PER_ALARM 12

typedef const char * string;
typedef unsigned char boolean_T;

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
   AlarmOff = 0,
   RunningIgnoreInput,
   Running
} Alarm_State_E;

typedef enum
{
#define ALARM_MACRO(a,b,c,d,e,f,g,h) a,
#include "alarm_display_config.h"
#undef ALARM_MACRO
   NUMBER_OF_ITEMS
} AlarmDisplayItem_E;

typedef enum
{
#define ALARM_INIT_MACRO(n,h,t,o,m,sun,mon,tue,wed,thu,fri,sat,on,st) n,
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
   boolean_T days[END_OF_WEEK];
   boolean_T on;
   Alarm_State_E state;
} Alarm;

#endif
