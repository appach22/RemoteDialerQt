#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
#ifdef Q_OS_MAC
    a.installEventFilter(&w);
#endif
    w.show();
    
    return a.exec();
}
