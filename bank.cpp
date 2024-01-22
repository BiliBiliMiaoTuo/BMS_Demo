#include "bank.h"
#include "accountlist.h"
#include "bankserver.h"
#include "createaccount.h"
#include "login.h"
#include "ui_bank.h"
#include <QDebug>
#include <QDialog>
#include <QMessageBox>
#include <QString>
#include <ctime>

//extern bankServer bankserver;
bankServer bankserver;

bank::bank(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::bank)
{
    ui->setupUi(this);

    // setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    // setFixedSize(this->width(), this->height());

}

bank::~bank()
{
    delete ui;
}

void bank::on_loginButton_clicked()
{
    login *login_windows = new login();
    login_windows->show();
    this->hide();
}

void bank::on_createAccountButton_clicked()
{
    createaccount *createaccount_windows = new createaccount();
    createaccount_windows->show();
    this->hide();
}

void bank::on_exitButton_clicked()
{

    showNormal();
    // int result = QMessageBox::critical(this, tr("Warnning"), tr("Are you sure Exit?"), QMessageBox::No | QMessageBox::Yes);
    // if (result == QMessageBox::Yes)
    // {
        this->close();  // 关闭当前窗口
    // }
}

void bank::on_printAccountButton_clicked()
{
    accountlist *accountlist_windows = new accountlist();
    accountlist_windows->show();
    this->hide();
}
