/**************************************************************************
/// brief The class description
/// author ruslan@robo-team.com
/// date 9.11.2020
/// details The class details
**************************************************************************/
#include "SettingsWindow.h"
#include "GithubIcon.h"
#include <QApplication>
#include <QMenu>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setQuitOnLastWindowClosed(false);
    GithubIcon tray;
    QObject::connect(&tray, &GithubIcon::onExit, &a, &QApplication::quit, Qt::QueuedConnection);
    tray.show();

    return a.exec();
}
