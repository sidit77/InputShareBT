#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <iostream>
#include "inputhook.h"
#include "bluetooth.h"
#include "hid.h"

#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

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

    uint8_t modifiers = 0;

    InputHook::Initialize([&](auto args){
        if(args.key == VirtualKey::LShift   && args.state == KeyState::Pressed)
            modifiers |= KEY_MOD_LSHIFT;
        if(args.key == VirtualKey::LControl && args.state == KeyState::Pressed)
            modifiers |= KEY_MOD_LCTRL;
        if(args.key == VirtualKey::LShift && args.state == KeyState::Released)
            modifiers &= ~KEY_MOD_LSHIFT;
        if(args.key == VirtualKey::LControl && args.state == KeyState::Released)
            modifiers &= ~KEY_MOD_LCTRL;
        if(!inputOption->isChecked()){
            if(args.state == KeyState::Pressed){
                printf("Keycode: 0x%x Scancode: Ox%x Translate: 0x%x\n", (uint32_t)args.key, args.scanCode, getHidKeycode(args.scanCode));
                bt_send_char(modifiers, getHidKeycode(args.scanCode));
            }

            return false;
        }
        return true;
    });



    auto r =  QApplication::exec();
    bt_destroy();
    return r;
}