#pragma once

#include <windows.h>
#include <functional>
#include "keys.h"

struct KeyEventArgs {
    VirtualKey key;
    ScanCode scanCode;
    KeyState state;
    KeyEventArgs(VirtualKey key, ScanCode scanCode, KeyState state) : key(key), state(state), scanCode(scanCode) {}
};

typedef std::function<bool(KeyEventArgs)> KeyCallback;

class InputHook {
public:
    InputHook() = delete;
    static void Initialize(KeyCallback keyCallback);
};

