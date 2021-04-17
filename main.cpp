#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <iostream>
#include <bitset>
#include "inputhook.h"
#include "bluetooth.h"
#include "hid.h"

int main(int argc, char *argv[]){
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    //Window window;
    //window.show();

    QMenu trayMenu;
    auto* inputOption = trayMenu.addAction("Input Enabled");
    inputOption->setCheckable(true);
    inputOption->setChecked(false);
    trayMenu.addAction("Quit", QApplication::instance(), &QApplication::quit);

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(":/resources/icon.png"));
    trayIcon.setToolTip("Snip Snap Program");
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();


    inputOption->setChecked(false);
    bt_init();

    //uint8_t modifiers = 0;

    auto modifiers = HIDModifierKeys::None;
    bool canSwap = true;
    std::vector<HIDKeyCode> pressedKeys;


    InputHook::Initialize([&](auto args){
        bool fresh = true;
        auto mod = getModifiers(args.key);
        if(mod != HIDModifierKeys::None){
            auto old = modifiers;
            if(args.state == KeyState::Pressed)
                modifiers += mod;
            if(args.state == KeyState::Released)
                modifiers -= mod;
            if(modifiers == old)
                fresh = false;
        } else {
            auto key = getHidKeycode(args.scanCode);
            if(key != 0x0){
                auto sv = std::find(pressedKeys.begin(), pressedKeys.end(), key);
                if(args.state == KeyState::Pressed){
                    if(sv == pressedKeys.end())
                        pressedKeys.push_back(key);
                    else
                        fresh = false;
                }
                if(args.state == KeyState::Released)
                    pressedKeys.erase(sv);
            }

        }
        if(modifiers & (HIDModifierKeys::LCtrl | HIDModifierKeys::RCtrl)){
            if(canSwap){
                std::cout << "Swaping" << std::endl;
                inputOption->setChecked(!inputOption->isChecked());
                canSwap = false;
            }
        } else {
            canSwap = true;
        }

        //TODO handle key press -> switch -> key release

        //std::cout << std::bitset<8>(static_cast<uint8_t>(modifiers)) << std::endl;
        if(!inputOption->isChecked()){
            HIDKeys keys{};
            for(int i = 0; i < std::min(pressedKeys.size(), {6}); i++){
                keys[i] = pressedKeys[pressedKeys.size() - std::min(pressedKeys.size(), {6}) + i];
            }
            if(fresh){
                std::cout << std::bitset<8>(static_cast<uint8_t>(modifiers));
                for(auto k : keys){
                    std::cout << " " << std::hex << +k;
                }
                std::cout <<std::endl;
                bt_send_char(modifiers, keys);
            }


            //if(args.state == KeyState::Pressed){
            //    printf("Keycode: 0x%x Scancode: Ox%x Translate: 0x%x\n", (uint32_t)args.key, args.scanCode, getHidKeycode(args.scanCode));
            //    //bt_send_char(static_cast<uint8_t>(modifiers), getHidKeycode(args.scanCode));
            //}

            return false;
        }
        return true;
    });



    auto r =  QApplication::exec();
    bt_destroy();
    return r;
}