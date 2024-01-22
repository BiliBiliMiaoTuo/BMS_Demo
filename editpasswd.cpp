#include "editpasswd.h"
#include "bankserver.h"
#include "options.h"
#include "ui_editpasswd.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <ctime>

extern bankServer bankserver;

editpasswd::editpasswd(QWidget *parent) : QDialog(parent),
    ui(new Ui::editpasswd)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

editpasswd::~editpasswd()
{
    delete ui;
}

void editpasswd::on_login_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void editpasswd::on_login_okButton_clicked()
{
    // int ret = QMessageBox::warning(this, tr("Warning"), tr("Are you sure reset password？"), QMessageBox::Yes | QMessageBox::No);

    // if (ret == QMessageBox::Yes) {
        if (ui->login_passwdInput->text() != ui->login_confirmPasswdInput->text())
        {
            ui->login_passwdInput->setText(QString::fromStdString(""));
            ui->login_confirmPasswdInput->setText(QString::fromStdString(""));
            // QMessageBox::critical(this, tr("Errow"), tr("Please recheck your entry！"), QMessageBox::Ok);

            return;
        }
        if (!bankserver.PasswdEdit(ui->login_passwdInput_old->text().toStdString(),
                                   ui->login_passwdInput->text().toStdString()))
        {
            ui->login_passwdInput->setText(QString::fromStdString(""));
            ui->login_confirmPasswdInput->setText(QString::fromStdString(""));
            ui->login_passwdInput_old->setText(QString::fromStdString(""));
            // QMessageBox::critical(this, tr("Errow"), tr("Password Wrong！"), QMessageBox::Ok);

            return;
        }
        // QMessageBox::information(this, tr(""), tr("Reset Success！"), QMessageBox::Ok);

        options *options_windows;
        options_windows = new options();
        options_windows->show();
        this->close();
        delete this;
    // }
}

void editpasswd::on_login_passwdInput_textEdited(const QString &arg1)
{
    if (arg1 != ui->login_confirmPasswdInput->text())
    {
        ui->login_repeatPasswdState->setStyleSheet("color:red;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("× Password Wrong"));
    }
    else
    {
        ui->login_repeatPasswdState->setStyleSheet("color:green;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("√"));
    }
}

void editpasswd::on_login_confirmPasswdInput_textEdited(const QString &arg1)
{
    if (arg1 != ui->login_passwdInput->text())
    {
        ui->login_repeatPasswdState->setStyleSheet("color:red;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("× Repassword Wrong"));
    }
    else
    {
        ui->login_repeatPasswdState->setStyleSheet("color:green;");
        ui->login_repeatPasswdState->setText(QString::fromStdString("√"));
    }
}
