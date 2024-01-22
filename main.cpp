#include "bank.h"

#include <QApplication>
#include <QMessageBox>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont("Arial", 12));
    bank w;
    w.show();
    return a.exec();
}
