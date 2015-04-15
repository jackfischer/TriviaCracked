#ifndef CATEGORY_WINDOW_H
#define CATEGORY_WINDOW_H

#include <pebble.h>

extern char categories[6][15];
extern int num_categories;

bool category_window_showing();
void category_window_show();
void category_window_create();
void category_window_destroy();
void category_window_click_provider(void *context);

#endif
