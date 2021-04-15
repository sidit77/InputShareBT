#include <iostream>
#include <memory>
#include <utility>
#include "inputhook.h"

namespace {

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);

    class HookWrapper {
    public:
        HHOOK keyboard;
        HookWrapper(){
            HINSTANCE hinstance = GetModuleHandle(nullptr);
            keyboard =  SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, hinstance, 0);
            std::cout << "Created Windows Hooks!" << std::endl;
        }
        ~HookWrapper(){
            UnhookWindowsHookEx(keyboard);
            std::cout << "Released Windows Hooks!" << std::endl;
        }
    };

    std::unique_ptr<HookWrapper> instance;
    KeyCallback keyCallback;

    KeyState isPressed(WPARAM wParam){
        switch (wParam) {
            case WM_KEYDOWN:
            case WM_SYSKEYDOWN:
                return KeyState::Pressed;
            case WM_KEYUP:
            case WM_SYSKEYUP:
                return KeyState::Released;
            default:
                throw std::invalid_argument("Illegal Event!");
        }
    }

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
        if(nCode >= 0) {
            auto *keyStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            if(!keyCallback(KeyEventArgs(static_cast<VirtualKey>(keyStruct->vkCode),  isPressed(wParam))))
                return 1;
        }
        return CallNextHookEx(instance->keyboard, nCode, wParam, lParam);
    }
}

void InputHook::Initialize(KeyCallback kc) {
    if(!instance)
        instance = std::make_unique<HookWrapper>();
    keyCallback = std::move(kc);
}


