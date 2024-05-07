#include "mainwindow.h"
#include "icons.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    a.setWindowIcon(QIcon(Icons::appIcon));

    w.show();
    return a.exec();
}
