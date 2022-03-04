#include "hwconfig.h"
#include "alarm_display.h"
#include "lcd_api.h"
#include "buttons.h"
#include "app_api.h"

#define ONE_SECOND 1000 // ms
#define ONE_MINUTE 60 // sec
#define ONE_HOUR 60 // min
#define ONE_DAY 24 // hr

//static TimeOfDay clock = {11, 5, 9, PM, SATURDAY};
static TimeOfDay clock = {3, 3, 0, PM, FRIDAY};

static Alarm alarms[] =
{
#define ALARM_INIT_MACRO(n,h,t,o,m,d,on,out) {{h, t, o, m, d}, on, out},
#include "alarm_display_config.h"
#undef ALARM_INIT_MACRO
};

static AlarmDisplayItem_T items[] =
{
   // Filling data with placeholders until runtime
#define ALARM_MACRO(n,t,s,x,y,f,d,c) {t, s, x, y, f, 0, c},
#include "alarm_display_config.h"
#undef ALARM_MACRO
};

static String days[] =
{
   "Sun",
   "Mon",
   "Tue",
   "Wed",
   "Thu",
   "Fri",
   "Sat"
};

static String meridiem_indicators[] =
{
   "am",
   "pm"
};

static SInt16_T selected_index = 0;

static UInt8_T run_clock(void)
{
   static UInt16_T current_ms = 0;
   static UInt16_T current_second = 0;
   static UInt16_T current_minute;
   static UInt16_T current_hour;
   static UInt8_T colon_flash = 0;

   // Convert display -> military
   current_minute = (10 * clock.minutes_tens) + clock.minutes_ones;
   current_hour = (12 * clock.am_pm) + (clock.hours % 12);

   current_ms += GetTaskRate();
   if(current_ms >= ONE_SECOND)
   {
      current_ms = 0;
      current_second++;
      colon_flash = !colon_flash;

      if(current_second >= ONE_MINUTE)
      {
         current_second = 0;
         current_minute++;

         if(current_minute >= ONE_HOUR)
         {
            current_minute = 0;
            current_hour++;

            if(current_hour >= ONE_DAY)
            {
               current_hour = 0;
               clock.day++;

               if(clock.day >= END_OF_WEEK)
               {
                  clock.day = SUNDAY;
               }
            }
         }
      }
   }

   // Convert military -> display
   clock.minutes_ones = current_minute % 10;
   clock.minutes_tens = current_minute / 10;
   clock.hours = current_hour % 12;
   clock.am_pm = (AM_PM)(current_hour / 12);

   return colon_flash;
}

static void move_selection(SInt8_T dir)
{
   do
   {
      selected_index += dir;
      if(selected_index >= NUMBER_OF_ITEMS)
      {
         selected_index = 0;
      }
      else if(selected_index < 0)
      {
         selected_index = NUMBER_OF_ITEMS - 1;
      }
   } while(items[selected_index].type != OnOff &&
      items[selected_index].control != NUMBER_OF_ITEMS &&
      !*items[items[selected_index].control].data); // Skip items whose alarm is OFF
}

static void process_button_inputs(void)
{
   static Button_State_T prev_next_button_state = BUTTON_NOT_PRESSED;
   static Button_State_T prev_back_button_state = BUTTON_NOT_PRESSED;
   static Button_State_T prev_select_button_state = BUTTON_NOT_PRESSED;
   Button_State_T next_button_state = GetButtonState(NEXT_BUTTON);
   Button_State_T back_button_state = GetButtonState(BACK_BUTTON);
   Button_State_T select_button_state = GetButtonState(SELECT_BUTTON);

   // Move selection through the display items based on navigation button inputs
   if(prev_next_button_state == BUTTON_NOT_PRESSED &&
          (next_button_state & BUTTON_PRESSED))
   {
      move_selection(1);
   }
   else if(prev_back_button_state == BUTTON_NOT_PRESSED &&
               (back_button_state & BUTTON_PRESSED))
   {
      move_selection(-1);
   }
   else if(prev_select_button_state == BUTTON_NOT_PRESSED &&
               (select_button_state & BUTTON_PRESSED))
   {
      (*items[selected_index].data)++;
      if(*items[selected_index].data >= items[selected_index].states)
      {
         *items[selected_index].data = 0;
      }
   }

   // Remember for next time
   prev_next_button_state = next_button_state;
   prev_back_button_state = back_button_state;
   prev_select_button_state = select_button_state;
}

static void render(Alarm_E active_alarm, UInt8_T colon_flash)
{
   UInt8_T i;

   ClearScreen();
   DrawVerticalLine(40, 0, 1, 32);
   DrawBox(101, 1, 2, 2);
   DrawBox(101, 5, 2, 2);
   DrawBox(101, 18, 2, 2);
   DrawBox(101, 22, 2, 2);

   for(i = 0; i < NUMBER_OF_ITEMS; i++)
   {
      SetFont(items[i].font);
      if(i == selected_index)
      {
         SetDisplayAttributes(LCD_HIGHLIGHT);
      }
      else
      {
         SetDisplayAttributes(LCD_NO_ATTRIBUTES);
      }

      if(items[i].type == OnOff)
      {
         String s_on = "Alarm %d";
         String s_off = "Alarm %d OFF";
         if(items[i].control == active_alarm && colon_flash)
         {
            SetDisplayAttributes(LCD_HIGHLIGHT);
         }
         PrintF(items[i].x_pos, items[i].y_pos,
            *items[i].data ? s_on : s_off, items[i].control + 1);
      }
      // If this item is always displayed, or if this item's alarm is ON
      else if(items[i].control == NUMBER_OF_ITEMS ||
         (items[i].control != NUMBER_OF_ITEMS && *items[items[i].control].data))
      {
         if(items[i].type == Weekday)
         {
            PrintF(items[i].x_pos, items[i].y_pos, "%s", days[*items[i].data]);
         }
         else if(items[i].type == AmPm)
         {
            PrintF(items[i].x_pos, items[i].y_pos, "%s", meridiem_indicators[*items[i].data]);
         }
         else if(items[i].type == Hour)
         {
            // Print 12 instead of 0 for the hour
            PrintF(items[i].x_pos, items[i].y_pos, "%2d",
               (*items[i].data == 0) ? 12 : *items[i].data);
         }
         else
         {
            PrintF(items[i].x_pos, items[i].y_pos, "%d", *items[i].data);
         }
      }
   }
   SetDisplayAttributes(LCD_NO_ATTRIBUTES);

   {
      // Clear unwanted highlight from clock area
      const UInt8_T blank[] = {8, 4, 0x00, 0x00, 0x00, 0x00};
      DrawBitmap(17, 4, blank);
   }

   if(colon_flash)
   {
      DrawBox(18, 4, 2, 2);
      DrawBox(18, 8, 2, 2);
   }
}

static Alarm_E check_alarms(void)
{
   UInt8_T i;
   Alarm_E active_alarm = NUMBER_OF_ALARMS;

   for(i = 0; i < NUMBER_OF_ALARMS; i++)
   {
      // Solenoids warm but not dangerously when powered for 1 minute
      if(alarms[i].on &&
         alarms[i].time.day          == clock.day &&
         alarms[i].time.am_pm        == clock.am_pm &&
         alarms[i].time.hours        == clock.hours &&
         alarms[i].time.minutes_tens == clock.minutes_tens &&
         alarms[i].time.minutes_ones == clock.minutes_ones)
      {
         SetCommand(alarms[i].output_device, TRUE);
         active_alarm = i;
      }
      else
      {
         SetCommand(alarms[i].output_device, FALSE);
      }
   }

   return active_alarm;
}

void Alarm_Display_Init(void)
{
   // Plug in non-constant values at runtime
#define ALARM_MACRO(n,t,s,x,y,f,d,c) items[n].data = (UInt8_T*)d;
#include "alarm_display_config.h"
#undef ALARM_MACRO

   WriteVar(LO_Backlight, TRUE, DATA_GOODDATA);
}

void Alarm_Display_Task(void)
{
   UInt8_T colon_flash = run_clock();
   Alarm_E active_alarm = check_alarms();
   process_button_inputs();
   render(active_alarm, colon_flash);
}
