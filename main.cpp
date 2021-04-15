#include <QtWidgets/QApplication>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QMenu>
#include <iostream>
#include "inputhook.h"
#include "bluetooth.h"

int main(int argc, char *argv[]){
    Q_INIT_RESOURCE(resources);

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("QtProject");
    QCoreApplication::setApplicationName("Application Example");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);

    //Window window;
    //window.show();

    //bt_init();

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

    InputHook::Initialize([&](auto args){
        if(args.key == VirtualKey::KeyF && args.pressed == KeyState::Pressed)
            std::cout << "F Pressed" << std::endl;

        return inputOption->isChecked();
    });





    auto r =  QApplication::exec();
    //bt_destroy();
    return r;
}