#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <string>
#include <vector>

#include "codescompressor.h"
#include "utils.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
