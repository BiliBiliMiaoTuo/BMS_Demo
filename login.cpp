#include "login.h"
#include "bank.h"
#include "bankserver.h"
#include "options.h"
#include "ui_login.h"
#include <QDebug>
#include <QMessageBox>
#include <stdio.h>
#include <ctime>

extern bankServer bankserver;

login::login(QWidget *parent) : QDialog(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

login::~login()
{
    delete ui;
}

void login::on_login_okButton_clicked()
{
    if (!bankserver.Login(ui->login_accountInput->text().toStdString(),
                          ui->login_passwdInput->text().toStdString()))
    {
        ui->login_passwdInput->setText(QString::fromStdString(""));
        // QMessageBox::critical(this, tr("Errow"), tr("Password Wrong！"), QMessageBox::Ok);

    }
    else if (bankserver.GetLost())
    {
        // if (!QMessageBox::critical(this, tr(""),
        //                            tr("The account has been reported lost!\nDo you want to cancel the loss report?"),
        //                            QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No))
        // {
            bankserver.RidLost();
            options *options_windows;
            options_windows = new options();
            options_windows->show();
            this->close();
        // }
    }
    else
    {
        options *options_windows;
        options_windows = new options();
        options_windows->show();
        this->close();
    }
}

void login::on_login_backButton_clicked()
{
    bank *bank_windows;
    bank_windows = new bank();
    bank_windows->show();
    this->close();
}

void login::on_login_accountInput_textChanged(const QString &arg1)
{
    if (arg1.toStdString() == "")
    {
        ui->login_accountStateLabel->setText(QString::fromStdString(""));
    }
    if (!bankserver.AccountExist(arg1.toStdString()))
    {
        ui->login_accountStateLabel->setStyleSheet("color:red;");
        ui->login_accountStateLabel->setText(QString::fromStdString("× Account does not exist!"));
    }
    else
    {
        ui->login_accountStateLabel->setStyleSheet("color:green;");
        ui->login_accountStateLabel->setText(QString::fromStdString("√"));
    }
}
