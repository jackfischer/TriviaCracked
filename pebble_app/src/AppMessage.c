#include "AppMessage.h"
#include "QuestionWindow.h"
#include "LoadingWindow.h"
#include "CategoryWindow.h"
#include "trivia-cracked.h"

bool isCrown;
char **categories;
int num_categories = 0;

void poll(void *data) {
	DictionaryIterator *iter;

	app_message_outbox_begin(&iter);

	dict_write_cstring(iter, REFRESH, "");
	app_message_outbox_send();
	
	app_timer_register(5000, (AppTimerCallback) poll, NULL);
}

void request(int i) {
	DictionaryIterator *iter;

	app_message_outbox_begin(&iter);

	dict_write_uint8(iter, REQUEST, i);
	app_message_outbox_send();
}

void question_window_show_timer(void *data) {
	((bool) data) ? question_window_set_crown() : question_window_set_no_crown();
	question_window_show();
}

// Called when a message is received from PebbleKitJS
void in_received_handler(DictionaryIterator *received, void *context) {
Tuple *tuple;
isCrown = false;
if  (!loading_window_showing() && !category_window_showing())
	window_stack_pop(false);

	tuple = dict_find(received, NUM_CATEGORIES);
	if (tuple) {
		for (int i = 0; i < num_categories; i++)
			free(categories[i]);
		num_categories = tuple->value->uint32;
	}

	tuple = dict_find(received, ANSWER);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Answer: %s", tuple->value->cstring); 
		text_layer_set_text(answer, tuple->value->cstring);
		app_timer_register(500, question_window_show_timer, (void*) &isCrown);
	}
	
	tuple = dict_find(received, QUESTION);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Received Message: %s", tuple->value->cstring);
		text_layer_set_text(question, tuple->value->cstring);
	}

	tuple = dict_find(received, CROWN);
	if(tuple) {
		APP_LOG(APP_LOG_LEVEL_DEBUG, "Recieved Status: %d", (int)tuple->value->uint32);
		isCrown = true;
	}

	tuple = dict_find(received, CATEGORY);
	if (tuple && isCrown) {
		int position = 0;
		int words_so_far = 0;
		char *long_string = (char*) tuple->value->cstring;
		APP_LOG(APP_LOG_LEVEL_DEBUG, "categories long: %s" , long_string);
		categories = (char**) realloc(categories, sizeof(char*) * num_categories);
		for (unsigned i = 0; i < strlen(long_string); i++) {
			if (long_string[i] == '|') {
				char *word = (char*) malloc(sizeof(char) * (i-position+1));
				//memcpy(word, &long_string[position], i - position);
				//word[i-position] = '\0';
				strncpy(word, &long_string[position], i-position);

				categories[words_so_far++] = word;
				APP_LOG(APP_LOG_LEVEL_DEBUG, "Word: %s", word);
				position = i + 1;
			}
		}
		category_window_show();
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
