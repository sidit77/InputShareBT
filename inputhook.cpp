#include <iostream>
#include "inputhook.h"

namespace {
    HHOOK hHook;
    bool inputDisabled = false;

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

        if(nCode < 0)
            return CallNextHookEx(hHook, nCode, wParam, lParam);

        auto* keyStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

        switch(wParam){
            case WM_KEYDOWN:
                std::cout << "Key pressed" << std::endl;
                if(keyStruct->vkCode == VK_ESCAPE)
                    inputDisabled = !inputDisabled;
                break;
            case WM_KEYUP:
                break;
            case WM_SYSKEYDOWN:
                break;
            case WM_SYSKEYUP:
                break;
            default:
                printf("Error!\n");
        }

        return inputDisabled ? 1 : CallNextHookEx(hHook, nCode, wParam, lParam);
    }
}

void InputHook::Initialize() {
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetModuleHandle(nullptr), 0);;
}

void InputHook::Destroy() {
    UnhookWindowsHookEx(hHook);
    std::cout << "Unhooked" << std::endl;
}

void InputHook::SetInputState(bool enable) {
    inputDisabled = !enable;
}

bool InputHook::GetInputState() {
    return !inputDisabled;
}

