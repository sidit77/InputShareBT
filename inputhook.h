#pragma once

#include <windows.h>

class InputHook {
public:
    InputHook() = delete;
    static void Initialize();
    static void Destroy();
    static void SetInputState(bool enable);
    static bool GetInputState();
};

