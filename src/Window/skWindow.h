#pragma once

#include <string>

/*
    skWindow
    handle window, process input and timer

    now only support only 1 window opening

*/
namespace sk_window {
    void Init(const std::string name, const int width, const int height);
    void ShutDown();

    void Process_event();
    bool Should_close();
    void Clear();
    void Swapbuffer();
}