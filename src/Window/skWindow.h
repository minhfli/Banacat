#pragma once

#include <string>

/*
    skWindow
    handle window, process input and timer

    now only support only 1 window opening

*/

void sk_window_init(const std::string name, const int width, const int height);
void sk_window_quit();

void sk_window_process_event();
bool sk_window_should_close();
void sk_window_clear();
void sk_window_swapbuffer();


void sk_keystate_get();
void sk_keystate_process();
