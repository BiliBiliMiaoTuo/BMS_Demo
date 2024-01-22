#include "withdraw.h"
#include "bankserver.h"
#include "options.h"
#include "ui_withdraw.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <ctime>

extern bankServer bankserver;

withdraw::withdraw(QWidget *parent) : QDialog(parent),
    ui(new Ui::withdraw)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);
    ui->deposit_moneyInput->setText("0");
    ui->deposit_moneyInput->setFocus();

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

withdraw::~withdraw()
{
    delete ui;
}

void withdraw::on_deposit_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void withdraw::on_deposit_okButton_clicked()
{
    // int result = QMessageBox::warning(this, tr(""),
    //                                   tr("Confirm withdraw") + ui->deposit_moneyInput->text() + tr("JPY?"),
    //                                   QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

    // switch (result) {
    // case QMessageBox::Yes:
        if (bankserver.Withdraw((ui->deposit_moneyInput->text()).toDouble(),
                                (ui->login_employeeInput->text()).toStdString()))
        {
            // QMessageBox::information(this, tr(""),
            //                          tr("Withdraw success") + ui->deposit_moneyInput->text() + tr("JPY！"),
            //                          QMessageBox::StandardButton::Ok);
            options *options_windows;
            options_windows = new options();
            options_windows->show();
            this->close();
        }
        else
        {
            ui->deposit_moneyInput->setText("0");
            ui->deposit_moneyInput->setFocus();
            // QMessageBox::information(this, QString(), tr("Insufficient balance!"), QMessageBox::StandardButton::Ok);
        }
        // break;
    // case QMessageBox::No:
    //     // 取消操作

    //     break;
    // default:
    //     break;
    // }


}
