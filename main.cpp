#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QCoreApplication::setOrganizationName(QLatin1String("maple"));
    QCoreApplication::setOrganizationDomain(QLatin1String("maple.tw"));
    QCoreApplication::setApplicationName(QLatin1String("Visioneering"));


    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    
    return a.exec();
}
