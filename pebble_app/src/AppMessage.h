#ifndef APP_MESSAGE_H
#define APP_MESSAGE_H

#include <pebble.h>

extern TextLayer *answer;
extern TextLayer *question;
extern Window *loading_window;

// Appmessage keys
enum {
	CROWN = 0,
	QUESTION = 1,
	ANSWER = 2,
	CATEGORY = 3,
	NUM_CATEGORIES = 4,
	REFRESH = 5,
	REQUEST = 6
};

void poll(void *data);
void question_window_load_timer(void *data);
void in_received_handler(DictionaryIterator *received, void *context);
void in_dropped_handler(AppMessageResult reason, void *context);
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context);
void request(int i);
#endif
