#pragma once

#include <windows.h>
#include <functional>

typedef std::function<void(char)> KeyCallback;

class InputHook {
public:
    InputHook() = delete;
    static void Initialize(KeyCallback keyCallback);
    static void SetInputState(bool enable);
    static bool GetInputState();
};

