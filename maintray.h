#ifndef MAINTRAY_H
#define MAINTRAY_H

#include <QWidget>
#include <QAction>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QList>
#include <QDebug>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <memory>
#include "mainwidget.h"

class MainTray : public QObject
{
public:
    explicit MainTray(QObject *parent = nullptr);
    void flush();
private:
    void readConfigs();
private slots:
    void onAddCommand();
    void onStartAll();
    void onStopAll();
    void onQuit();
private:
    QAction *addAction_;
    QAction *startAllAction_;
    QAction *stopAllAction_;
    QAction *quitAction_;
    QMenu *menu_;
    QSystemTrayIcon *trayIcon_;
    QMap<QString, std::shared_ptr<Command>> commands_;
};

#endif // MAINTRAY_H
