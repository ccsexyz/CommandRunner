#include "MainTray.h"
#include <QApplication>

#ifndef QT_NO_SYSTEMTRAYICON

int main(int argc, char *argv[])
{
//    QCoreApplication a(argc, argv);
    QApplication a(argc, argv);
    QApplication::setQuitOnLastWindowClosed(false);
    MainTray tray;
//    QWidget widget;
//    widget.setWindowFlags(Qt::Tool| Qt::WindowStaysOnTopHint | Qt::X11BypassWindowManagerHint);
//    widget.show();
    return a.exec();
}

#else

#include <QLabel>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    QLabel *label = new QLabel("QSystemTrayIcon is not supported on this platform");
    label->setWordWrap(true);
    label->show();
    app.exec();
}

#endif
