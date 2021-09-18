#ifndef MAINWINDOW_TEST_H
#define MAINWINDOW_TEST_H
#include "mainwindow.h"

#include <QObject>
#include <QTest>

class mainwindow_test : public QObject
{
    Q_OBJECT
public:
    explicit mainwindow_test(QObject *parent = nullptr);

signals:
private slots:
    void case_void();
    void case_set_visable();
    void case_open_pics();
    void case_get_pic_page();
    void case_udp_recv();

private:
    MainWindow m;
};

#endif // MAINWINDOW_TEST_H
