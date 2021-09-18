#include "mainwindow_test.h"
#include <QUdpSocket>

mainwindow_test::mainwindow_test(QObject *parent) : QObject(parent)
{
}

void mainwindow_test::case_void()
{
}

void mainwindow_test::case_set_visable()
{
    m.set_visable(true);
    QVERIFY(m.isVisible() == true);
}

void mainwindow_test::case_open_pics()
{
    //    QString pics_folder(R"(C:\Users\Administrator\Pictures\linercode\pics)");
    QString pics_folder(R"(C:\Users\Administrator\Pictures\)");
    m.open_pics(pics_folder);
}

void mainwindow_test::case_get_pic_page()
{
    QString p("100000.0003.0001.jpg");
    QVERIFY(m.get_pic_page(p) == 3);
    p = QString("100000.-3.0001.jpg");
    QVERIFY(m.get_pic_page(p) == -3);
    p = QString("100000.3.0001.jpg");
    QVERIFY(m.get_pic_page(p) == 3);
}

void mainwindow_test::case_udp_recv()
{
    QUdpSocket s;
    QString data = "{\"page\":10}";
    s.writeDatagram(data.toStdString().c_str(), data.size(), QHostAddress("127.0.0.1"), 12345);
}
