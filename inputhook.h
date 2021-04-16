#pragma once

#include <windows.h>
#include <functional>
#include "keys.h"

struct KeyEventArgs {
    VirtualKey key;
    uint16_t scanCode;
    KeyState state;
    KeyEventArgs(VirtualKey key, uint16_t scanCode, KeyState state) : key(key), state(state), scanCode(scanCode) {}
};

typedef std::function<bool(KeyEventArgs)> KeyCallback;

class InputHook {
public:
    InputHook() = delete;
    static void Initialize(KeyCallback keyCallback);
};

