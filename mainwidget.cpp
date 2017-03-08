#include "mainwidget.h"
#include <QFileDialog>
#include <QMessageBox>

MainWidget::MainWidget(std::shared_ptr<Command> c)
    : QWidget(nullptr), command_(c)
{
    fileBtn_ = new QPushButton("选择可执行程序");
    pathLabel_ = new QLabel(c->path_);
    nameLabel_ = new QLabel("别名");
    nameLineEdit_ = new QLineEdit(c->name_);
    argsLabel_ = new QLabel("命令行参数");
    argsTextEdit_ = new QPlainTextEdit;
    argsTextEdit_->setPlainText(c->args_);
    autostartLabel_ = new QLabel("自启动");
    autostartCheckBox_ = new QCheckBox;
    autostartCheckBox_->setChecked(c->autostart_);
    saveBtn_ = new QPushButton("保存");
    cancelBtn_ = new QPushButton("取消");
    layout_ = new QGridLayout;
    layout_->addWidget(fileBtn_, 0, 0);
    layout_->addWidget(pathLabel_, 0, 1);
    layout_->addWidget(nameLabel_, 1, 0);
    layout_->addWidget(nameLineEdit_, 1, 1);
    layout_->addWidget(argsLabel_, 2, 0);
    layout_->addWidget(argsTextEdit_, 2, 1);
    layout_->addWidget(autostartLabel_, 3, 0);
    layout_->addWidget(autostartCheckBox_, 3, 1);
    layout_->addWidget(saveBtn_, 4, 0);
    layout_->addWidget(cancelBtn_, 4, 1);
    setLayout(layout_);
    connect(saveBtn_, &QPushButton::clicked, this, &MainWidget::onSaveBtn);
    connect(cancelBtn_, &QPushButton::clicked, this, &MainWidget::onCancelBtn);
    connect(fileBtn_, &QPushButton::clicked, this, &MainWidget::onFileBtn);
}

void MainWidget::onSaveBtn() {
    auto name = nameLineEdit_->text();
    if (name.length() == 0) {
        QMessageBox::warning(nullptr, "错误", "必须填写名称");
        return;
    }
    if(command_->path_.length() == 0) {
        QMessageBox::warning(nullptr, "错误", "必须选择可执行文件");
        return;
    }
    command_->name_ = name;
    command_->args_ = argsTextEdit_->toPlainText();
    command_->autostart_ = autostartCheckBox_->isChecked();
    emit newCommand();
    destroy();
}

void MainWidget::onFileBtn() {
    QString filePath = QFileDialog::getOpenFileName(nullptr, "选择可执行程序");
    if (filePath.length() == 0) {
        return;
    }
    command_->path_ = filePath;
    pathLabel_->setText(filePath);
    update();
}

void MainWidget::onCancelBtn() {
    destroy();
}

void MainWidget::closeEvent(QCloseEvent *event) {
    event->accept();
    deleteLater();
}
