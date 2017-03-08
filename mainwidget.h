#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QCloseEvent>
#include <QCheckBox>
#include "command.h"
#include <QComboBox>
#include <QTextEdit>
#include <QPushButton>
#include <QLabel>
#include <memory>
#include <QPlainTextEdit>
#include <QLineEdit>

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MainWidget(std::shared_ptr<Command> c);
signals:
    void newCommand();
private slots:
    void onSaveBtn();
    void onCancelBtn();
    void onFileBtn();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
    QGridLayout *layout_;
    QPushButton *fileBtn_;
    QLabel *pathLabel_;
    QLabel *nameLabel_;
    QLineEdit *nameLineEdit_;
    QLabel *argsLabel_;
    QPlainTextEdit *argsTextEdit_;
    QLabel *autostartLabel_;
    QCheckBox *autostartCheckBox_;
    QPushButton *saveBtn_;
    QPushButton *cancelBtn_;
    std::shared_ptr<Command> command_;
};

#endif // MAINWIDGET_H
