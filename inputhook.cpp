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
    bool inputDisabled;

    LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

        if(nCode >= 0) {

            auto *keyStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

            switch (wParam) {
                case WM_KEYDOWN:
                    std::cout << (char)MapVirtualKey(keyStruct->vkCode, MAPVK_VK_TO_CHAR) << std::endl;
                    if (keyStruct->vkCode == VK_ESCAPE)
                        inputDisabled = !inputDisabled;
                    break;
                case WM_KEYUP:
                    break;
                case WM_SYSKEYDOWN:
                    break;
                case WM_SYSKEYUP:
                    break;
                default:
                    std::cout << "Error: Unknown Event" << std::endl;
            }

            if(inputDisabled)
                return 1;
        }
        return CallNextHookEx(instance->keyboard, nCode, wParam, lParam);
    }
}

void InputHook::Initialize() {
    if(!instance)
        instance = std::make_unique<HookWrapper>();
    inputDisabled = false;
}

void InputHook::SetInputState(bool enable) {
    inputDisabled = !enable;
}

bool InputHook::GetInputState() {
    return !inputDisabled;
}

