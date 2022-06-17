#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setStyleSheet("background-image:url(:Fond3.png)");
    w.show();
    //w.showFullScreen();

    return a.exec();
}

