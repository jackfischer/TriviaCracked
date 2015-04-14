#ifndef CATEGORY_WINDOW_H
#define CATEGORY_WINDOW_H

#include <pebble.h>

extern char **categories;
extern int num_categories;

void category_window_show();
void category_window_create();
void category_window_destroy();

#endif
