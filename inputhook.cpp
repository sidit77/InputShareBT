#include <iostream>
#include <memory>
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

    KeyState getState(WPARAM wParam){
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

    ScanCode getScanCode(KBDLLHOOKSTRUCT* keyStruct){
        auto scanCode = static_cast<ScanCode>(keyStruct->scanCode);
        if(scanCode == 0x0 || keyStruct->vkCode == VK_SNAPSHOT || keyStruct->vkCode == VK_SCROLL || keyStruct->vkCode == VK_PAUSE || keyStruct->vkCode == VK_NUMLOCK){
            scanCode = static_cast<ScanCode>(MapVirtualKey(keyStruct->vkCode, MAPVK_VK_TO_VSC_EX));
        }else{
            if(keyStruct->flags & LLKHF_EXTENDED)
                scanCode |= 0xe000;
        }
        return scanCode;
    }

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){
        if(nCode >= 0) {
            auto *keyStruct = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
            if(!keyCallback(KeyEventArgs(
                    static_cast<VirtualKey>(keyStruct->vkCode),
                    getScanCode(keyStruct),
                    getState(wParam))))
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


