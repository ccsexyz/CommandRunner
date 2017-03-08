#include "maintray.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QMap>
#include <QVector>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QDebug>
#include <QFileDialog>

const QString configFilePath = "config.json";

MainTray::MainTray(QObject *parent) : QObject(parent)
{
    addAction_ = new QAction("添加可执行程序", this);
    startAllAction_ = new QAction("启动所有程序", this);
    stopAllAction_ = new QAction("停用所有程序", this);
    quitAction_ = new QAction("退出", this);
    connect(addAction_, &QAction::triggered, this, &MainTray::onAddCommand);
    connect(startAllAction_, &QAction::triggered, this, &MainTray::onStartAll);
    connect(stopAllAction_, &QAction::triggered, this, &MainTray::onStopAll);
    connect(quitAction_, &QAction::triggered, this, &MainTray::onQuit);
    menu_ = new QMenu;
    readConfigs();
    flush();
    trayIcon_ = new QSystemTrayIcon;
    trayIcon_->setContextMenu(menu_);
    trayIcon_->setIcon(QIcon(":/naive.png"));
    trayIcon_->show();
    for(auto it = commands_.cbegin(); it != commands_.cend(); ++it) {
        if(it.value()->autostart_) {
            it.value()->run();
        }
    }
}

void MainTray::readConfigs() {
    QFile file(configFilePath);
    if(!file.open(QFile::ReadOnly)) {
        qDebug() << "failed to open"<< configFilePath;
        return;
    }
    auto byteArray = file.readAll();
    QJsonParseError json_error;
    auto document = QJsonDocument::fromJson(byteArray, &json_error);
    if(!(json_error.error == QJsonParseError::NoError)) {
        qDebug() << "json parse error " << json_error.errorString();
        return;
    }
    auto var = document.toVariant();
    if(var.isNull()) {
        return;
    }
    auto map = var.toMap();
    for(auto it = map.begin(); it != map.end(); ++it) {
        auto key = it.key();
        auto value = it.value().toMap();
        auto c = std::make_shared<Command>();
        c->name_ = key;
        if(value.contains("args")) {
            c->args_ = value["args"].toString();
        }
        if(value.contains("path")) {
            c->path_ = value["path"].toString();
        }
        if(value.contains("autostart")) {
            c->autostart_ = value["autostart"].toBool();
        }
        commands_.insert(key, c);
    }
}

void MainTray::flush() {
    menu_->clear();
    menu_->addAction(addAction_);
    menu_->addAction(startAllAction_);
    menu_->addAction(stopAllAction_);
    menu_->addSeparator();
    for (auto it = commands_.begin(); it != commands_.end(); ++it) {
        QMenu *m = new QMenu(it.key());
        std::weak_ptr<Command> c = it.value();
        QAction *startAction = new QAction("启用", this);
        QAction *editAction = new QAction("编辑", this);
        QAction *stopAction = new QAction("停用", this);
        connect(startAction, &QAction::triggered, [this, c](){
            if(c.expired()) {
                return;
            }
            auto command = c.lock();
            command->run();
        });
        connect(editAction, &QAction::triggered, [this, c](){
            if(c.expired()) {
                return;
            }
            auto cl = c.lock();
            auto command = std::make_shared<Command>();
            command->args_ = cl->args_;
            command->name_ = cl->name_;
            command->autostart_ - cl->autostart_;
            command->path_ = cl->path_;
            MainWidget *widget = new MainWidget(command);
            widget->show();
            connect(widget, &MainWidget::newCommand, [this, c, command](){
                if(c.expired()) {
                    return;
                }
                auto cl = c.lock();
                cl->args_ = command->args_;
                cl->name_ = command->name_;
                cl->autostart_ = command->autostart_;
                cl->path_ = command->path_;
            });
        });
        connect(stopAction, &QAction::triggered, [this, c](){
            if(c.expired()) {
                return;
            }
            auto command = c.lock();
            command->stop();
        });
        m->addAction(startAction);
        m->addAction(editAction);
        m->addAction(stopAction);
        menu_->addMenu(m);
    }
    if(!commands_.empty()) {
        menu_->addSeparator();
    }
    menu_->addAction(quitAction_);
    QFile file(configFilePath);
    file.open(QFile::WriteOnly);
    QJsonObject object;
    for(auto it = commands_.begin(); it != commands_.end(); ++it) {
        auto key = it.key();
        auto value = it.value();
        QJsonObject cobject;
        cobject["args"] = value->args_;
        cobject["path"] = value->path_;
        cobject["autostart"] = value->autostart_;
        object[key] = cobject;
    }
    QJsonDocument document(object);
    file.write(document.toJson());
    file.close();
}

void MainTray::onAddCommand() {
    auto c = std::make_shared<Command>();
    MainWidget *widget = new MainWidget(c);
    widget->show();
    connect(widget, &MainWidget::newCommand, [this, c](){
        commands_.insert(c->name_, c);
        flush();
        if(c->autostart_) {
            c->run();
        }
    });
}

void MainTray::onStartAll() {
    for(auto it = commands_.begin(); it != commands_.end(); ++it) {
        it.value()->run();
    }
}

void MainTray::onStopAll() {
    for(auto it = commands_.begin(); it != commands_.end(); ++it) {
        it.value()->stop();
    }
}

void MainTray::onQuit() {
    QCoreApplication::quit();
}

