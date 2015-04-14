#include "QuestionWindow.h"
#include "trivia-cracked.h"

Window *question_window;
TextLayer *question;
TextLayer *answer;

static void back_pop_all_handler(ClickRecognizerRef recognizer, void *context) {
	window_stack_pop_all(false);
}

void question_window_click_provider(Window *window) {
	window_single_click_subscribe(BUTTON_ID_BACK, back_pop_all_handler);
}

void question_window_set_no_crown() {
	window_set_click_config_provider(question_window, (ClickConfigProvider) question_window_click_provider);
}

void question_window_set_crown() {
	window_set_click_config_provider(question_window, NULL);
}

void question_window_create() {
	question_window = window_create();

	question = text_layer_create(GRect(0,0,144,60));
	answer = text_layer_create(GRect(0,60,144,94));

	layer_add_child(window_get_root_layer(question_window), text_layer_get_layer(question));
	layer_add_child(window_get_root_layer(question_window), text_layer_get_layer(answer));
}

void question_window_show() {
	window_stack_push(question_window, true);
}

void question_window_destroy() {
	window_destroy(question_window);
	text_layer_destroy(question);
	text_layer_destroy(answer);
}
