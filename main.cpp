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

    InputHook::Initialize([&](auto args){
        if(!inputOption->isChecked()){
            if(args.pressed == KeyState::Pressed){
                uint8_t mods = 0;
                if(GetKeyState((uint32_t)VirtualKey::LShift) & 0x8000)
                    mods |= KEY_MOD_LSHIFT;
                if(GetKeyState((uint32_t)VirtualKey::LControl) & 0x8000)
                    mods |= KEY_MOD_LCTRL;
                bt_send_char(mods, keycode_windows_to_hid(args.scanCode));
            }

            return false;
        }
        return true;
    });



    auto r =  QApplication::exec();
    bt_destroy();
    return r;
}