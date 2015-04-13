#include "AppMessage.h"
#include "QuestionWindow.h"
#include "LoadingWindow.h"
#include "trivia-cracked.h"

void poll(void *data) {
	DictionaryIterator *iter;

	app_message_outbox_begin(&iter);

	dict_write_cstring(iter, REFRESH, "");
	app_message_outbox_send();
	
	app_timer_register(5000, (AppTimerCallback) poll, NULL);
}

void question_window_show_timer(void *data) {
	((bool) data) ? APP_LOG(APP_LOG_LEVEL_DEBUG, "CROWN") : question_window_set_no_crown();
	question_window_show();
}

// Called when a message is received from PebbleKitJS
void in_received_handler(DictionaryIterator *received, void *context) {
Tuple *tuple;
isCrown = false;
if (!loading_window_showing())
	window_stack_pop(false);

	tuple = dict_find(received, CROWN);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Recieved Status: %d", (int)tuple->value->uint32);
		isCrown = true;
	}

	tuple = dict_find(received, ANSWER);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %s", tuple->value->cstring); 
		text_layer_set_text(answer, tuple->value->cstring);
		if (window_stack_get_top_window() == loading_window) {
			app_timer_register(500, question_window_show_timer, (void*) isCrown);
		}
	}
	
	tuple = dict_find(received, QUESTION);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
		text_layer_set_text(question, tuple->value->cstring);
	}
}

// Called when an incoming message from PebbleKitJS is dropped
void in_dropped_handler(AppMessageResult reason, void *context) {	
#ifdef DEBUG
	APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Dropped!");
#endif
}

// Called when PebbleKitJS does not acknowledge receipt of a message
void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
#ifdef DEBUG
	APP_LOG(APP_LOG_LEVEL_DEBUG, "AppMessage Failed!");
#endif
}
