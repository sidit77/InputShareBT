#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <iostream>
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

    bt_init();

    //uint8_t modifiers = 0;

    auto modifiers = HIDModifierKeys::None;

    InputHook::Initialize([&](auto args){
        auto mod = getModifiers(args.key);
        if(mod != HIDModifierKeys::None){
            if(args.state == KeyState::Pressed)
                modifiers += mod;
            if(args.state == KeyState::Released)
                modifiers -= mod;
        } else {

        }
        if(!inputOption->isChecked()){
            if(args.state == KeyState::Pressed){
                printf("Keycode: 0x%x Scancode: Ox%x Translate: 0x%x\n", (uint32_t)args.key, args.scanCode, getHidKeycode(args.scanCode));
                bt_send_char(static_cast<uint8_t>(modifiers), getHidKeycode(args.scanCode));
            }

            return false;
        }
        return true;
    });



    auto r =  QApplication::exec();
    bt_destroy();
    return r;
}