#include <pebble.h>

#define NUM_ITEMS_MAX 6

Window *questionWindow;	
Window *loadingWindow;
static TextLayer *question;
static TextLayer *answer;
static TextLayer *loading;

char *words[6];
int numCategories;
bool isCrown;
	
// Key values for AppMessage Dictionary
enum {
	CROWN = 0,
	QUESTION = 1,
	ANSWER = 2,
	CATEGORY = 3,
	NUM_CATEGORIES = 4,
	REFRESH = 5
};

void timer_callback(void *data) {
	DictionaryIterator *iter;

	app_message_outbox_begin(&iter);

	dict_write_cstring(iter, REFRESH, "");
	app_message_outbox_send();
	
	app_timer_register(10000, (AppTimerCallback) timer_callback, NULL);
}

// Called when a message is received from PebbleKitJS
static void in_received_handler(DictionaryIterator *received, void *context) {
	Tuple *tuple;
	isCrown = false;

	tuple = dict_find(received, CROWN);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Recieved Status: %d", (int)tuple->value->uint32);
		isCrown = true;
	}

	tuple = dict_find(received, ANSWER);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Status: %s", tuple->value->cstring); 
		text_layer_set_text(answer, tuple->value->cstring);
		if (window_stack_get_top_window() == loadingWindow)
			window_stack_push(questionWindow, true);
	}
	
	tuple = dict_find(received, CATEGORY);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
		if (isCrown) {
			int starting_point = 0;
			char long_string[60];
			memcpy(long_string, tuple->value->cstring, tuple->length);
			for (int i = 0; i < (int)strlen(long_string); i++) {
				if (long_string[i] == '|') {
					char word[20];
					strncpy(word, &long_string[starting_point], i - starting_point);
					word[i - starting_point] = '\0';
					APP_LOG(APP_LOG_LEVEL_DEBUG, "Word parsed: %s", word);
					starting_point = i + 1;
				}
			}
		}
	}

	tuple = dict_find(received, QUESTION);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
		text_layer_set_text(question, tuple->value->cstring);
	}
}

// Called when an incoming message from PebbleKitJS is dropped
static void in_dropped_handler(AppMessageResult reason, void *context) {	
}

// Called when PebbleKitJS does not acknowledge receipt of a message
static void out_failed_handler(DictionaryIterator *failed, AppMessageResult reason, void *context) {
}

void init(void) {
	questionWindow = window_create();
	loadingWindow = window_create();
	
	// Register AppMessage handlers
	app_message_register_inbox_received(in_received_handler); 
	app_message_register_inbox_dropped(in_dropped_handler); 
	app_message_register_outbox_failed(out_failed_handler);
		
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());

	question = text_layer_create(GRect(0,0,144,60));
	answer = text_layer_create(GRect(0,60,144,94));
	loading = text_layer_create(GRect(0,60,144,94));

	layer_add_child(window_get_root_layer(questionWindow), text_layer_get_layer(question));
	layer_add_child(window_get_root_layer(questionWindow), text_layer_get_layer(answer));

	layer_add_child(window_get_root_layer(loadingWindow), text_layer_get_layer(loading));

	text_layer_set_text_alignment(loading, GTextAlignmentCenter);

	text_layer_set_text(question, "This is a question");
	text_layer_set_text(answer, "This is an answer");
	text_layer_set_text(loading, "Loading...");

	window_stack_push(loadingWindow, true);
	app_timer_register(10000, (AppTimerCallback) timer_callback, NULL);
}

void deinit(void) {
	app_message_deregister_callbacks();
	window_destroy(questionWindow);
	window_destroy(loadingWindow);
	text_layer_destroy(question);
	text_layer_destroy(answer);
}

int main( void ) {
	init();
	app_event_loop();
	deinit();
}
