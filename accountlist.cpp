#include "accountlist.h"
#include "bank.h"
#include "bankserver.h"
#include "options.h"
#include "ui_accountlist.h"
#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <stdio.h>
#include <ctime>

extern bankServer bankserver;

accountlist::accountlist(QWidget *parent) : QDialog(parent),
    ui(new Ui::accountlist)
{
    time_t tTime = time(nullptr);

    char szTime[100] = {'\0'};

    tm result;
    tm *pTm = localtime_r(&tTime, &result);
    if(pTm)
    {
        //pTm = localtime(&time_t_time);
        pTm->tm_year += 1900;
        pTm->tm_mon += 1;

        snprintf(szTime, sizeof(szTime), "%04d-%02d-%02d",
                 pTm->tm_year,
                 pTm->tm_mon,
                 pTm->tm_mday);

        string strTime = szTime;
    }
    ui->setupUi(this);

    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    SetDefault();

    ui->dateEdit->setDate(QDate::fromString(QString::fromStdString(szTime), QString::fromStdString("yyyy-MM-dd")));

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

accountlist::~accountlist()
{
    delete ui;
}

void accountlist::on_accountlist_backButton_clicked()
{
    bank *bank_windows;
    bank_windows = new bank();
    bank_windows->show();
    this->close();
}

void accountlist::on_accountlist_searchEdit_textChanged(const QString &arg1)
{
    string strSearchString = arg1.toStdString();
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Account"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Address"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Saving Type"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Interest"));
    model->setHorizontalHeaderItem(5, new QStandardItem("Report Loss"));
    model->setHorizontalHeaderItem(6, new QStandardItem("Staff"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 80);
    ui->accountlist_tableView->setColumnWidth(4, 80);
    ui->accountlist_tableView->setColumnWidth(5, 160);
    ui->accountlist_tableView->setColumnWidth(6, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfAccounts = bankserver.GetNumberOfAccounts();

    int iItemsFinded = -1;
    for (int i = 0; i < iNumberOfAccounts; i++)
    {
        int iTmpType = bankserver.GetType(i);
        bool bLost = bankserver.GetLost(i);
        string strTypeString, strInterestString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "One Year";
            strInterestString = "1.98";
            break;
        case 1:
            strTypeString = "Three Years";
            strInterestString = "2.25";
            break;
        case 2:
            strTypeString = "Five Years";
            strInterestString = "3.5";
            break;
        }

        string strLostString;
        if (bLost)
        {
            time_t tTime = bankserver.GetLostTime(i);

            char szTime[100] = {'\0'};

            tm result;
            tm *pTm = localtime_r(&tTime, &result);
            if(pTm)
            {
                //pTm = localtime(&time_t_time);
                pTm->tm_year += 1900;
                pTm->tm_mon += 1;

                snprintf(szTime, sizeof(szTime), "%04dYear%02dMonth%02dDay %02d:%02d:%02d",
                         pTm->tm_year,
                         pTm->tm_mon,
                         pTm->tm_mday,
                         pTm->tm_hour,
                         pTm->tm_min,
                         pTm->tm_sec);

                strLostString = szTime;
            }

        }
        else
        {
            strLostString = "否";
        }

        if (bankserver.GetNumber(i).find(strSearchString) != string::npos ||
            bankserver.GetName(i).find(strSearchString) != string::npos ||
            bankserver.GetAddress(i).find(strSearchString) != string::npos ||
            strTypeString.find(strSearchString) != string::npos ||
            strInterestString.find(strSearchString) != string::npos ||
            strLostString.find(strSearchString) != string::npos ||
            bankserver.GetOperator(i).find(strSearchString) != string::npos)
        {
            iItemsFinded++;
            model->setItem(iItemsFinded, 0, new QStandardItem(QString::fromStdString(bankserver.GetNumber(i))));
            model->setItem(iItemsFinded, 1, new QStandardItem(QString::fromStdString(bankserver.GetName(i))));
            model->setItem(iItemsFinded, 2, new QStandardItem(QString::fromStdString(bankserver.GetAddress(i))));
            model->setItem(iItemsFinded, 3, new QStandardItem(QString::fromStdString(strTypeString)));
            model->setItem(iItemsFinded, 4, new QStandardItem(QString::fromStdString(strInterestString)));
            model->setItem(iItemsFinded, 5, new QStandardItem(QString::fromStdString(strLostString)));
            model->setItem(iItemsFinded, 6, new QStandardItem(QString::fromStdString(bankserver.GetOperator(i))));
        }
    }
    ui->label_2->setNum(iItemsFinded + 1);
}

void accountlist::SetDefault()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Account"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Name"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Address"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Saving Tyes"));
    model->setHorizontalHeaderItem(4, new QStandardItem("Interest"));
    model->setHorizontalHeaderItem(5, new QStandardItem("Report Loss"));
    model->setHorizontalHeaderItem(6, new QStandardItem("Staff"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 70);
    ui->accountlist_tableView->setColumnWidth(4, 70);
    ui->accountlist_tableView->setColumnWidth(5, 190);
    ui->accountlist_tableView->setColumnWidth(6, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfAccounts = bankserver.GetNumberOfAccounts();
    ui->label_2->setNum(iNumberOfAccounts);

    for (int i = 0; i < iNumberOfAccounts; i++)
    {
        int iTmpType = bankserver.GetType(i);
        bool bLost = bankserver.GetLost(i);
        string strTypeString, strInterestString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "One Year";
            strInterestString = "1.98";
            break;
        case 1:
            strTypeString = "Three Years";
            strInterestString = "2.25";
            break;
        case 2:
            strTypeString = "Five Years";
            strInterestString = "3.5";
            break;
        }

        string strLostString;
        if (bLost)
        {
            time_t tTime = bankserver.GetLostTime(i);

            char szTime[100] = {'\0'};

            tm result;
            tm *pTm = localtime_r(&tTime, &result);
            if(pTm)
            {
                //pTm = localtime(&time_t_time);
                pTm->tm_year += 1900;
                pTm->tm_mon += 1;

                snprintf(szTime, sizeof(szTime), "%04dYear%02dMonth%02dDay %02d:%02d:%02d",
                         pTm->tm_year,
                         pTm->tm_mon,
                         pTm->tm_mday,
                         pTm->tm_hour,
                         pTm->tm_min,
                         pTm->tm_sec);

                strLostString = szTime;
            }

        }
        else
        {
            strLostString = "No";
        }

        model->setItem(i, 0, new QStandardItem(QString::fromStdString(bankserver.GetNumber(i))));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(bankserver.GetName(i))));
        model->setItem(i, 2, new QStandardItem(QString::fromStdString(bankserver.GetAddress(i))));
        model->setItem(i, 3, new QStandardItem(QString::fromStdString(strTypeString)));
        model->setItem(i, 4, new QStandardItem(QString::fromStdString(strInterestString)));
        model->setItem(i, 5, new QStandardItem(QString::fromStdString(strLostString)));
        model->setItem(i, 6, new QStandardItem(QString::fromStdString(bankserver.GetOperator(i))));
    }
}

void accountlist::on_dateEdit_dateChanged(const QDate &date)
{
    time_t tTime = QDateTime(date, QTime(23, 59, 59)).addDays(1).toSecsSinceEpoch() - 1;



    ui->label_6->setText(QString::number(bankserver.GetPrepare(tTime)));
}

void accountlist::on_accountlist_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Output"),
                                                    "",
                                                    tr("*.html"));
    if (!fileName.isNull())
    {
        bankserver.ExportAccountFile(fileName.toStdString());
        // QMessageBox::information(this, tr("Message"), tr("Output Success!"), QMessageBox::Ok);

    }
}

