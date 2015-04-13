#include "LoadingWindow.h"
#include "trivia-cracked.h"

Window *loading_window;
TextLayer *loading;

void loading_window_create() {
	loading_window = window_create();
	loading = text_layer_create(GRect(0,60,144,94));

	text_layer_set_text_alignment(loading, GTextAlignmentCenter);

	text_layer_set_text(loading, "Loading...");

	layer_add_child(window_get_root_layer(loading_window), text_layer_get_layer(loading));
}

void loading_window_show() {
	window_stack_push(loading_window, true);
}

void loading_window_destroy() {
	window_destroy(loading_window);
	text_layer_destroy(loading);
}

bool loading_window_showing() {
	return (window_stack_get_top_window() == loading_window);
}
