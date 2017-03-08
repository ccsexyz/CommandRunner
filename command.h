#ifndef COMMAND_H
#define COMMAND_H

#include <QProcess>

class Command : public QObject
{
public:
    Command();
    ~Command() {stop();}

    void run();
    void stop();

private slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onReadyReadStdErr();
    void onReadyReadStdOut();
    void onStarted();

public:
    QString name_;
    QString path_;
    QString args_;
    bool autostart_ = true;
    bool running_ = false;
    bool destroy_ = false;

private:
    QProcess client_;
    int retry_ = 0;
};

#endif // COMMAND_H
