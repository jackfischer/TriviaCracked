#include "CategoryWindow.h"
#include "AppMessage.h"

Window *category_window;
MenuLayer *category_menu;

static uint16_t category_menu_get_num_sections(MenuLayer *layer, void *data) {
	return 1;
}

static uint16_t category_menu_get_num_rows(MenuLayer *layer, uint16_t section_index, void *data) {
	return num_categories;
}

static int16_t category_menu_get_header_height(MenuLayer *menu_layer, uint16_t section_index, void *data) {
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

static void category_menu_draw_header(GContext *ctx, const Layer *cell_layer, uint16_t section_index, void *data) {
	menu_cell_basic_header_draw(ctx, cell_layer, "Select the Category!");
}

static void category_menu_draw_row(GContext *ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data) {
	menu_cell_title_draw(ctx, cell_layer, categories[cell_index->row]);
}

static void category_menu_select(MenuLayer *layer, MenuIndex *index, void *data) {
	request(index->row);
}

void category_window_create() {
	category_window = window_create();
	category_menu = menu_layer_create(GRect(0,0,144,154));
	menu_layer_set_callbacks(category_menu, NULL, (MenuLayerCallbacks) {
		.get_num_sections = category_menu_get_num_sections,
		.get_num_rows = category_menu_get_num_rows,
		.get_header_height = category_menu_get_header_height,
		.draw_header = category_menu_draw_header,
		.draw_row = category_menu_draw_row,
		.select_click = category_menu_select
	});
	menu_layer_set_click_config_onto_window(category_menu, category_window);
	layer_add_child(window_get_root_layer(category_window), menu_layer_get_layer(category_menu));
}

void category_window_destroy() {
	window_destroy(category_window);
	menu_layer_destroy(category_menu);
}

void category_window_show() {
	menu_layer_reload_data(category_menu);
	window_stack_push(category_window, true);
}
