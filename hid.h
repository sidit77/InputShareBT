#pragma once

#include <cstdint>
#include <type_traits>
#include "keys.h"

uint8_t getHidKeycode(uint16_t scanCode);

enum class HIDModifierKeys : uint8_t {
    None    = 0x00,
    LCtrl   = 0x01,
    LShift  = 0x02,
    LAlt    = 0x04,
    LMeta   = 0x08,
    RCtrl   = 0x10,
    RShift  = 0x20,
    RAlt    = 0x40,
    RMeta   = 0x80
};

inline HIDModifierKeys operator | (HIDModifierKeys lhs, HIDModifierKeys rhs)
{
    using T = std::underlying_type_t<HIDModifierKeys>;
    return static_cast<HIDModifierKeys>(static_cast<T>(lhs) | static_cast<T>(rhs));
}

inline HIDModifierKeys& operator += (HIDModifierKeys& lhs, HIDModifierKeys rhs)
{
    using T = std::underlying_type_t<HIDModifierKeys>;
    lhs = static_cast<HIDModifierKeys>(static_cast<T>(lhs) | static_cast<T>(rhs));
    return lhs;
}

inline HIDModifierKeys& operator -= (HIDModifierKeys& lhs, HIDModifierKeys rhs)
{
    using T = std::underlying_type_t<HIDModifierKeys>;
    lhs = static_cast<HIDModifierKeys>(static_cast<T>(lhs) & ~static_cast<T>(rhs));
    return lhs;
}

HIDModifierKeys getModifiers(VirtualKey key);