#include "deposit.h"
#include "bankserver.h"
#include "options.h"
#include "ui_deposit.h"
#include <QDebug>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <ctime>

extern bankServer bankserver;

deposit::deposit(QWidget *parent) : QDialog(parent),
    ui(new Ui::deposit)
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

deposit::~deposit()
{
    delete ui;
}

void deposit::on_deposit_okButton_clicked()
{
    // int ret = QMessageBox::warning(this, tr("Warnning"),
    //                                tr("Are you sure deposit") + ui->deposit_moneyInput->text() + tr("JPY?"),
    //                                QMessageBox::Yes | QMessageBox::No);

    // if (ret == QMessageBox::Yes) {
        bankserver.Deposit((ui->deposit_moneyInput->text()).toDouble(),
                           (ui->login_employeeInput->text()).toStdString());
        // QMessageBox::information(this, tr("Deposit Success"),
        //                          tr("Success deposit") + ui->deposit_moneyInput->text() + tr("JPY！"),
        //                          QMessageBox::Ok);

        options *options_windows;
        options_windows = new options();
        options_windows->show();
        this->close();
    // }
}

void deposit::on_deposit_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}
