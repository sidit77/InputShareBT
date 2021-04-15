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

    bt_init();

    InputHook::Initialize([](char c){
        std::cout << c << std::endl;
        bt_send_char(c);
    });



    QMenu trayMenu;
    auto* inputOption = trayMenu.addAction("Input Enabled", &InputHook::SetInputState);
    inputOption->setCheckable(true);
    inputOption->setChecked(InputHook::GetInputState());
    trayMenu.addAction("Quit", QApplication::instance(), &QApplication::quit);

    QSystemTrayIcon trayIcon;
    trayIcon.setIcon(QIcon(":/resources/icon.png"));
    trayIcon.setToolTip("Snip Snap Program");
    trayIcon.setContextMenu(&trayMenu);
    trayIcon.show();

    auto r =  QApplication::exec();
    bt_destroy();
    return r;
}