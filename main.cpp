#include "mainwindow.h"
#include "mainwindow_test.h"

#include <QApplication>
#include <QDebug>
#include <QTest>

#if QT_NO_DEBUG
int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    return a.exec();
}
#else
QTEST_MAIN(mainwindow_test);
#endif
