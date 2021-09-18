#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QListWidget>
#include <QMainWindow>
#include <QScrollArea>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    using PicsDir = QString;
    using Page = int;
    using FileName = QString;
    using RecvData = QByteArray;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void set_visable(bool is_visable);
    void open_pics(PicsDir dir);
    Page get_pic_page(const FileName& file_name);
public slots:
    void read_udp_data();
    void on_dbclicked_item(QListWidgetItem* item);

private:
    void process_recv_data(RecvData& data);

protected:
    void closeEvent(QCloseEvent* event) override;

private:
    const QString MAGIC_FIELD;
    QUdpSocket* m_recv_socket;
    QUdpSocket* m_send_socket;
    Ui::MainWindow *ui;
    const int LISTEN_PORT;
    const int SERVER_PORT;
};
#endif // MAINWINDOW_H
