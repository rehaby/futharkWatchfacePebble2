#include <pebble.h>

Window *window;
TextLayer *text_layer;

static GFont futhark_font;

static char *timeStr;
static int timeStrSize = 50;

#define TWENETY 20
#define THIRTY 21
#define FOURTY 22
#define FIVTY 23

static char* numToRunes[]= {"", 
                           "in", "tfiy", "driy", "fioriy", "fim", "siks", "siau", "ata", "niu", "tiu", 
                           "ililfu","tolf", "dritan", "fiortan", "fimtan", "sikstan", "siautan", "atian", "nitian", "tutuku",
                           "driy;tiki",
                           "fioriy;tiki",
                           "fim;tiki"};

void UpdateTimeDate(struct tm *tick_time)
{  
  int hours = 0;
  if(tick_time->tm_hour > 12)
  {
    hours = (tick_time->tm_hour - 12);
  }
  else if(tick_time->tm_hour == 0)
  {
    hours = 12;
  }
  else
  {
    hours = tick_time->tm_hour;
  }
  int mins =0;
  int minsTens = 0;
  
  if(tick_time->tm_min < 21)
  {
    mins = tick_time->tm_min;
  }
  else if(tick_time->tm_min > 20 && tick_time->tm_min < 30)
  {
    minsTens = TWENETY;
    mins = tick_time->tm_min - 20;
  }
  else if(tick_time->tm_min > 29 && tick_time->tm_min < 40)
  {
    minsTens = THIRTY;
    mins = tick_time->tm_min - 30;
  }
  else if(tick_time->tm_min > 39 && tick_time->tm_min < 50)
  {
    minsTens = FOURTY;
    mins = tick_time->tm_min - 40;
  }
  else if(tick_time->tm_min > 49 && tick_time->tm_min < 60)
  {
    minsTens = FIVTY;
    mins = tick_time->tm_min - 50;
  }
  
  if(minsTens == 0)
  {
   snprintf(timeStr, timeStrSize, ";%s; ;%s;", numToRunes[hours], numToRunes[mins] ); 
  }
  else if(mins == 0)
  {
    snprintf(timeStr, timeStrSize, ";%s; ;%s;", numToRunes[hours], numToRunes[minsTens] );
  }
  else if( minsTens == 0 && mins == 0 )
  {
    snprintf(timeStr, timeStrSize, ";%s;", numToRunes[hours] );
  }
  else
  {
    snprintf(timeStr, timeStrSize, ";%s; ;%s; ;ok; ;%s;", numToRunes[hours], numToRunes[mins], numToRunes[minsTens] );  
  }

  text_layer_set_text( text_layer, timeStr);
}

static void handle_bluetooth_state(bool connected)
{
  if(connected == false)
  {
    vibes_double_pulse();
  }
}

static void handle_minute_tick(struct tm *tick_time, TimeUnits units_changed) 
{
  UpdateTimeDate(tick_time);
}

void handle_init(void) 
{
	// Create a window and text layer
	window = window_create();
  window_set_fullscreen(window,true);
  
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_frame(window_layer);
  
	text_layer = text_layer_create(bounds);
  
  futhark_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_FUTHARK_48));
	
	// Set the text, font, and text alignment
	text_layer_set_text(text_layer, "hi;ik;im;bibbli");
	text_layer_set_font(text_layer, futhark_font);
	text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  text_layer_set_background_color(text_layer, GColorBlack);
  text_layer_set_text_color(text_layer, GColorWhite);
	
	// Add the text layer to the window
	layer_add_child(window_layer, text_layer_get_layer(text_layer));

	// Push the window
	window_stack_push(window, true);
  
  timeStr = malloc(timeStrSize);
  tick_timer_service_subscribe(MINUTE_UNIT, handle_minute_tick);
  bluetooth_connection_service_subscribe(handle_bluetooth_state);
  
	
	// App Logging!
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Just pushed a window!");
}

void handle_deinit(void) 
{
  free(timeStr);
	// Destroy the text layer
	text_layer_destroy(text_layer);
	fonts_unload_custom_font(futhark_font);
	// Destroy the window
	window_destroy(window);
}

int main(void) 
{
	handle_init();
	app_event_loop();
	handle_deinit();
}
