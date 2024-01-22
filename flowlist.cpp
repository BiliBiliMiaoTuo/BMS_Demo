#include "flowlist.h"
#include "bank.h"
#include "bankserver.h"
#include "options.h"
#include "ui_flowlist.h"
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QTableView>
#include <stdio.h>
#include <ctime>

extern bankServer bankserver;

flowlist::flowlist(QWidget *parent) : QDialog(parent),
    ui(new Ui::flowlist)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::WindowCloseButtonHint | Qt::MSWindowsFixedSizeDialogHint);

    SetDefault();
    ui->label_4->setText(QString::number(bankserver.GetTotal()));

    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);
    setWindowFlags(windowFlags() & ~Qt::CustomizeWindowHint);
    setWindowFlags(windowFlags() & ~Qt::WindowCloseButtonHint);
    setFixedSize(this->width(), this->height());
}

flowlist::~flowlist()
{
    delete ui;
}

void flowlist::on_accountlist_backButton_clicked()
{
    options *options_windows;
    options_windows = new options();
    options_windows->show();
    this->close();
}

void flowlist::on_accountlist_searchEdit_textChanged(const QString &arg1)
{
    string strSearchString = arg1.toStdString();
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Type"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Money"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Staff"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfFlows = bankserver.GetNumberOfFlows();

    int iItemsFinded = -1;
    for (int i = 0; i < iNumberOfFlows; i++)
    {
        int iTmpType = bankserver.GetOperationType(i);
        time_t tTime = bankserver.GetTime(i);

        string strTypeString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "Deposit";
            break;
        case 1:
            strTypeString = "Withdraw";
            break;
        }

        string strTime;
        char szTime[100] = {'\0'};

        tm result;
        tm *pTm = localtime_r(&tTime, &result);
        if(pTm)
        {
            //pTm = localtime(&time_t_time);
            pTm->tm_year += 1900;
            pTm->tm_mon += 1;

            snprintf(szTime, sizeof(szTime), "%04dYear%02dMonth%02dDay %02d:%02d:%02d",
                     result.tm_year,
                     result.tm_mon,
                     result.tm_mday,
                     result.tm_hour,
                     result.tm_min,
                     result.tm_sec);

            strTime = szTime;
        }


        if (bankserver.GetMoney(i) == arg1.toDouble() ||
            strTypeString.find(strSearchString) != string::npos ||
            strTime.find(strSearchString) != string::npos ||
            bankserver.GetOperator(i).find(strSearchString) != string::npos)
        {
            iItemsFinded++;
            model->setItem(iItemsFinded, 0, new QStandardItem(QString::fromStdString(strTime)));
            model->setItem(iItemsFinded, 1, new QStandardItem(QString::fromStdString(strTypeString)));
            model->setItem(iItemsFinded, 2, new QStandardItem(QString::number(bankserver.GetMoney(i))));
            model->setItem(iItemsFinded, 3, new QStandardItem(QString::fromStdString(bankserver.GetFlowOperator(i))));
        }
    }
    ui->label_2->setNum(iItemsFinded + 1);
}

void flowlist::SetDefault()
{
    QStandardItemModel *model = new QStandardItemModel(this);
    ui->accountlist_tableView->setModel(model);

    model->setHorizontalHeaderItem(0, new QStandardItem("Time"));
    model->setHorizontalHeaderItem(1, new QStandardItem("Type"));
    model->setHorizontalHeaderItem(2, new QStandardItem("Money"));
    model->setHorizontalHeaderItem(3, new QStandardItem("Staff"));

    ui->accountlist_tableView->setColumnWidth(0, 200);
    ui->accountlist_tableView->setColumnWidth(1, 150);
    ui->accountlist_tableView->setColumnWidth(2, 200);
    ui->accountlist_tableView->setColumnWidth(3, 80);

    //ui->accountlist_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->accountlist_tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->accountlist_tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    int iNumberOfFlows = bankserver.GetNumberOfFlows();

    for (int i = 0; i < iNumberOfFlows; i++)
    {
        int iTmpType = bankserver.GetOperationType(i);
        time_t tTime = bankserver.GetTime(i);
        //qDebug()<<("Type=")<<(iTmpType);
        string strTypeString;
        switch (iTmpType)
        {
        case 0:
            strTypeString = "Deposit";
            break;
        case 1:
            strTypeString = "Withdraw";
            break;
        }
        //qDebug()<<("String=")<<(QString::fromStdString(strTypeString));

        string strTime;
        char szTime[100] = {'\0'};

        tm result;
        tm *pTm = localtime_r(&tTime, &result);
        if(pTm)
        {
            pTm->tm_year += 1900;
            pTm->tm_mon += 1;

            snprintf(szTime, sizeof(szTime), "%04dYear%02dMonth%02dDay %02d:%02d:%02d",
                     result.tm_year,
                     result.tm_mon,
                     result.tm_mday,
                     result.tm_hour,
                     result.tm_min,
                     result.tm_sec);

            strTime = szTime;
            //qDebug()<<"here";
        }

        model->setItem(i, 0, new QStandardItem(QString::fromStdString(strTime)));
        model->setItem(i, 1, new QStandardItem(QString::fromStdString(strTypeString)));
        model->setItem(i, 2, new QStandardItem(QString::number(bankserver.GetMoney(i))));
        model->setItem(i, 3, new QStandardItem(QString::fromStdString(bankserver.GetFlowOperator(i))));
    }
    ui->label_2->setNum(iNumberOfFlows);
}

void flowlist::on_accountlist_exportButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Output"),
                                                    "",
                                                    tr("HTML (*.html)"));
    if (!fileName.isNull())
    {
        //fileName是文件名
        bankserver.ExportFlowFile(fileName.toStdString());
        // QMessageBox::information(this, tr("Information"), tr("Output Success!"), QMessageBox::Ok);


    }
}

