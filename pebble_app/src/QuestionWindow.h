#ifndef QUESTION_WINDOW_H
#define QUESTION_WINDOW_H

#include <pebble.h>
void question_window_show();
void question_window_create();
void question_window_destroy();
void question_window_set_no_crown();
void question_window_click_provider(Window *window);
void back_pop_all_handler(ClickRecognizerRef recognizer, void *context);
#endif
