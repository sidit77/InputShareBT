#include <cstdlib>
#include <windows.h>
#include <cstdio>

HHOOK hHook;

bool inputDisabled = false;

LRESULT CALLBACK LowLevelKeyboardProc(int nCode, WPARAM wParam, LPARAM lParam){

    if(nCode < 0)
        return CallNextHookEx(hHook, nCode, wParam, lParam);

    auto* keyStruct = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);

    switch(wParam){
        case WM_KEYDOWN:
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

HMODULE GetCurrentModule()
{
    HMODULE hModule = NULL;
    // hModule is NULL if GetModuleHandleEx fails.
    GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS
                      | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                      (LPCTSTR)GetCurrentModule, &hModule);
    return hModule;
}

int main(){

    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, LowLevelKeyboardProc, GetCurrentModule(), 0);

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);

    return 0;
}