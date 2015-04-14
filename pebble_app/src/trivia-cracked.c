#include <pebble.h>
#include "trivia-cracked.h"
#include "AppMessage.h"
#include "QuestionWindow.h"
#include "LoadingWindow.h"
#include "CategoryWindow.h"

void init(void) {
	question_window_create();
	loading_window_create();
	category_window_create();
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	loading_window_show();
	app_timer_register(5000, (AppTimerCallback) poll, NULL);
}

void deinit(void) {
	app_message_deregister_callbacks();
	question_window_destroy();
	loading_window_destroy();
	category_window_destroy();
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
