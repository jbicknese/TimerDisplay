#include "hwconfig.h"
#include "alarm_display.h"
#include "lcd_api.h"
#include "buttons.h"
#include "app_api.h"

static UInt16_T TIME_TO_IGNORE_BUTTON = 2000; // ms
static UInt16_T RUN_TIME_LIMIT = 5000; // ms
static UInt16_T BUTTON_PRESSED_THRESHOLD = 1000; // counts

static TimeOfDay clock = {3, 3, 0, PM, FRIDAY};

static Alarm alarms[] =
{
#define ALARM_INIT_MACRO(n,h,t,o,m,sun,mon,tue,wed,thu,fri,sat,on,state) \
   {{h, t, o, m, END_OF_WEEK}, {sun,mon,tue,wed,thu,fri,sat}, on, state},
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

static string day_strings[2][END_OF_WEEK] =
{
   {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"},
   {"S","M","T","W","T","F","S"}
};

static string meridiem_indicators[] =
{
   "am",
   "pm"
};

static SInt16_T selected_index = 0;

static boolean_T run_clock(void)
{
   static UInt16_T current_ms = 0;
   static UInt16_T current_second = 0;
   static UInt16_T current_minute;
   static UInt16_T current_hour;
   static boolean_T colon_flash = FALSE;

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

static void render(boolean_T colon_flash)
{
   UInt8_T i;

   ClearScreen();
   DrawVerticalLine(40, 0, 1, 32);
   DrawBox(101, 1, 2, 2);
   DrawBox(101, 5, 2, 2);
   DrawBox(101, 18, 2, 2);
   DrawBox(101, 22, 2, 2);

   for(i = NUMBER_OF_ITEMS; i--;)
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
         string s_on = "Alarm %d";
         string s_off = "Alarm %d OFF";
         boolean_T alarm_is_active = (alarms[items[i].control].state != AlarmOff);
         if(alarm_is_active && colon_flash)
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
         if(items[i].type == Weekday && items[i].control == NUMBER_OF_ITEMS) // Clock day
         {
            PrintF(items[i].x_pos, items[i].y_pos, "%s", day_strings[0][*items[i].data]);
         }
         else if(items[i].type == Weekday) // Alarm day
         {
            // Use the index to determine which day we're printing
            PrintF(items[i].x_pos, items[i].y_pos, *items[i].data ? "%s" : "",
               day_strings[1][(i - Alarm1_Sunday) % ITEMS_PER_ALARM]);
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

   // for debugging
   //PrintF(0,21,"%d", ReadVar(SensorInput));
   //PrintF(40,21,"%d", alarms[0].state);
}

static void execute_state_machine(Alarm_E i, boolean_T is_active, boolean_T was_active)
{
   static UInt16_T timer = 0;

   switch(alarms[i].state)
   {
   default:
   case AlarmOff:
      if(is_active && !was_active) // rising edge
      {
         alarms[i].state = RunningIgnoreInput;
         timer = 0;
      }
      break;

   case RunningIgnoreInput:
      timer += GetTaskRate();

      if(timer >= TIME_TO_IGNORE_BUTTON || !BUTTON_IS_PRESSED)
      {
         alarms[i].state = Running;
         timer = 0;
      }
      else if(!is_active)
      {
         // This should never happen
         alarms[i].state = AlarmOff;
      }
      break;

   case Running:
      timer += GetTaskRate();

      if(timer >= RUN_TIME_LIMIT || BUTTON_IS_PRESSED)
      {
         alarms[i].state = AlarmOff;
      }
      break;
   }
}

static void check_alarms(void)
{
   // TODO Initialize this so the size of this array always matches the size of the alarms array
   static boolean_T was_alarm_active[] = {FALSE, FALSE};
   boolean_T should_command_output = FALSE;
   Alarm_E i;

   for(i = 0; i < NUMBER_OF_ALARMS; i++)
   {
      boolean_T alarm_is_active =
         alarms[i].on &&
         alarms[i].days[clock.day] == TRUE &&
         alarms[i].time.am_pm        == clock.am_pm &&
         alarms[i].time.hours        == clock.hours &&
         alarms[i].time.minutes_tens == clock.minutes_tens &&
         alarms[i].time.minutes_ones == clock.minutes_ones;

      execute_state_machine(i, alarm_is_active, was_alarm_active[i]);
      should_command_output |= (alarms[i].state != AlarmOff);

      // Remember for next time
      was_alarm_active[i] = alarm_is_active;
   }

   SetCommand(OUT_1, should_command_output);
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
   boolean_T colon_flash = run_clock();

   check_alarms();

   process_button_inputs();

   render(colon_flash);
}
