#include <pebble.h>
static bool bt;
static Window *s_main_window;
static Window *s_roll_window;
static Window *s_send_window;
static SimpleMenuLayer *s_main_menu;
static SimpleMenuSection s_main_sections[1];
static SimpleMenuItem s_main_items[2];
static TextLayer *s_roll_header;
static TextLayer * s_rack_header;
static TextLayer *s_start_time;
static TextLayer *s_stop_time;
static TextLayer *s_total_time;
static TextLayer *s_send_header;
static Window *s_dissconected_window;
static TextLayer *s_dissconected_header;
const char *s_rack_start[50];
const char *s_rack_end[50];
static char s_roll_string[]="Racks:  | Avg:  ";
static char s_rack_string[]="Rack   ";
static char s_cur_rack[]="01";
static char s_total_racks[]="01";
void update_rack_view() {
  if(s_cur_rack[0]=='0'){
    s_rack_string[5]=s_cur_rack[1];
  } else {
    s_rack_string[5]=s_cur_rack[0];
    s_rack_string[6]=s_cur_rack[1];
  }
  text_layer_set_text(s_rack_header, s_rack_string);
}
void next_rack() {
  if(s_cur_rack[1]=='9') {
    s_cur_rack[1]='0';
    s_cur_rack[0]=s_cur_rack[0]+1;
  } else {
    s_cur_rack[1]=s_cur_rack[1]+1;
  }
  update_rack_view();
}
void prev_rack() {
  if(s_cur_rack[0]=='0' && s_cur_rack[1]=='1') {
    return;
  }
  if(s_cur_rack[1]=='0') {
    s_cur_rack[0]=s_cur_rack[0]-1;
    s_cur_rack[1]='9';
  } else {
    s_cur_rack[1]=s_cur_rack[1]-1;
  }
  update_rack_view();
}
void roll_click(Window *window) {
  window_single_click_subscribe(BUTTON_ID_DOWN, next_rack);
  window_single_click_subscribe(BUTTON_ID_UP, prev_rack);
}
void bluetooth_connection_change(bool conected) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "BT");
  bt =conected;
}
void show_roll_window() {
  window_stack_push(s_roll_window, true);
};
void send_data() {
  if(bt) {
    window_stack_push(s_send_window, true);
  } else {
    window_stack_push(s_dissconected_window, true);
  }
};
void show_dissconnect_message() {
  window_stack_push(s_dissconected_window, true);
}
void main_window_load(Window *window) {
  s_main_items[0] = (SimpleMenuItem) {
    .title="Roll!",
    .callback=show_roll_window
  };
  s_main_items[1] = (SimpleMenuItem) {
    .title="Send Data",
    .callback=send_data
  };
  s_main_sections[0] = (SimpleMenuSection) {
    .title="Rollers Log",
    .num_items = 2,
    .items = s_main_items
  };
  s_main_menu = simple_menu_layer_create(GRect(0,0,144,200),s_main_window,s_main_sections, 1, NULL);
  layer_add_child(window_get_root_layer(s_main_window), simple_menu_layer_get_layer(s_main_menu));
};
void main_window_unload(Window *window) {
  
};
void roll_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Roll Load");
  s_roll_header = text_layer_create(GRect(0,1,144,28));
  text_layer_set_background_color(s_roll_header, GColorBlack);
  text_layer_set_text_color(s_roll_header, GColorWhite);
  text_layer_set_text_alignment(s_roll_header, GTextAlignmentCenter);
  text_layer_set_font(s_roll_header, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text(s_roll_header, "Racks: 0 | Avg: 00");
  layer_add_child(window_get_root_layer(s_roll_window), text_layer_get_layer(s_roll_header));
  
  s_rack_header = text_layer_create(GRect(10, 35, 124, 18));
  if(s_cur_rack[0]=='0') {
    s_rack_string[5] = s_cur_rack[1];
  } else {
    s_rack_string[5] = s_cur_rack[0];
    s_rack_string[6] = s_cur_rack[1];
  }
  text_layer_set_text(s_rack_header, s_rack_string);
  text_layer_set_font(s_rack_header, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_background_color(s_rack_header, GColorWhite);
  text_layer_set_text_color(s_rack_header, GColorBlack);
  layer_add_child(window_get_root_layer(s_roll_window), text_layer_get_layer(s_rack_header));
  
  s_start_time = text_layer_create(GRect(10,65,144,18));
  text_layer_set_text(s_start_time, "Start Time:");
  text_layer_set_font(s_start_time, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(s_roll_window), text_layer_get_layer(s_start_time));
  
  s_stop_time = text_layer_create(GRect(10, 87, 144, 18));
  text_layer_set_font(s_stop_time, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text(s_stop_time, "End Time:");
  layer_add_child(window_get_root_layer(s_roll_window), text_layer_get_layer(s_stop_time));
  
  s_total_time = text_layer_create(GRect(10, 110, 144, 18));
  text_layer_set_text(s_total_time, "Total Time:");
  text_layer_set_font(s_total_time, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  layer_add_child(window_get_root_layer(s_roll_window), text_layer_get_layer(s_total_time));
};
void roll_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Roll Unload");
  
};
void send_window_load(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Send Load");
  s_send_header = text_layer_create(GRect(0,1,144,28));
  text_layer_set_font(s_send_header, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
  text_layer_set_text_alignment(s_send_header, GTextAlignmentCenter);
  text_layer_set_background_color(s_send_header, GColorBlack);
  text_layer_set_text_color(s_send_header, GColorWhite);
  text_layer_set_text(s_send_header, "Send Data");
  layer_add_child(window_get_root_layer(s_send_window), text_layer_get_layer(s_send_header));
};
void send_window_unload(Window *window) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "Send Unload");
};
void dissconected_window_load(Window *window) {
  s_dissconected_header = text_layer_create(GRect(0, 1, 144, 28));
  text_layer_set_background_color(s_dissconected_header, GColorBlack);
  text_layer_set_text_color(s_dissconected_header, GColorWhite);
  text_layer_set_font(s_dissconected_header, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text(s_dissconected_header, "Dissconected");
  layer_add_child(window_get_root_layer(s_dissconected_window), text_layer_get_layer(s_dissconected_header));
};
void dissconected_window_unload(Window *window) {
  
};
void init() {
  bluetooth_connection_service_subscribe(bluetooth_connection_change);
  bluetooth_connection_change(bluetooth_connection_service_peek());
  s_main_window = window_create();
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load=main_window_load,
    .unload=main_window_unload
  });
  window_stack_push(s_main_window, true);
  s_roll_window = window_create();
  window_set_window_handlers(s_roll_window, (WindowHandlers){
    .load=roll_window_load,
    .unload=roll_window_unload
  });
  window_set_click_config_provider(s_roll_window, (ClickConfigProvider) roll_click);
  s_send_window = window_create();
  window_set_window_handlers(s_send_window, (WindowHandlers){
    .load=send_window_load,
    .unload=send_window_unload
  });
  s_dissconected_window = window_create();
  window_set_window_handlers(s_dissconected_window, (WindowHandlers){
    .load=dissconected_window_load,
    .unload=dissconected_window_unload
  });
};
void deinit() {
  simple_menu_layer_destroy(s_main_menu);
  text_layer_destroy(s_send_header);
  text_layer_destroy(s_roll_header);
  text_layer_destroy(s_rack_header);
  text_layer_destroy(s_start_time);
  text_layer_destroy(s_stop_time);
  text_layer_destroy(s_total_time);
  window_destroy(s_roll_window);
  window_destroy(s_send_window);
  window_destroy(s_main_window);
  bluetooth_connection_service_unsubscribe();
};
int main(void) {
  init();
  app_event_loop();
  deinit();
}