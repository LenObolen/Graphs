#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    //StartDialog startDialog;
    //startDialog.show();
    return a.exec();
}
