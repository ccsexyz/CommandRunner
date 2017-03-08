#include "command.h"
#include <QTimer>
#include <QDebug>

Command::Command()
    : client_(this)
{
    connect(&client_, &QProcess::started, this, &Command::onStarted);
    connect(&client_, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, &Command::onFinished);
    connect(&client_, &QProcess::readyReadStandardError, this, &Command::onReadyReadStdErr);
    connect(&client_, &QProcess::readyReadStandardOutput, this, &Command::onReadyReadStdOut);
}

void Command::run() {
    destroy_ = false;
    if (client_.state() != QProcess::ProcessState::NotRunning) {
        return;
    }
    client_.start(path_, args_.split(" "));
}

void Command::stop() {
    if(destroy_ == false && client_.state() == QProcess::ProcessState::NotRunning) {
        return;
    }
    destroy_ = true;
    client_.kill();
}

void Command::onFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    running_ = false;
    if(destroy_ == false) {
        QTimer::singleShot(1000, [this]{
            // have memory problem
            // but I dont care
            retry_++;
            if (retry_ > 10) {
                destroy_ = true;
                return;
            }
            run();
        });
    }
}

void Command::onStarted() {
    retry_ = 0;
    running_ = true;
    qDebug() << name_ << "started!";
}

void Command::onReadyReadStdErr() {
    qDebug() << name_ << client_.readAllStandardError();
}

void Command::onReadyReadStdOut() {
    qDebug() << name_ << client_.readAllStandardOutput();
}
