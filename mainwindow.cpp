#include "mainwindow.h"
#include "lib/cximage/include/ximage.h"
#include "ui_mainwindow.h"
#include <QCloseEvent>
#include <QDir>
#include <QFile>
#include <QImageReader>
#include <QJsonDocument>
#include <QListWidgetItem>
#include <QNetworkDatagram>
#include <QSize>
#include <QVBoxLayout>
#include <QVarLengthArray>
#include <QtDebug>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , MAGIC_FIELD("ZK")
    , ui(new Ui::MainWindow)
    , LISTEN_PORT(12345)
    , SERVER_PORT(54321)
{
    ui->setupUi(this);
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setIconSize(QSize(300, 400));
    ui->listWidget->setSpacing(10);
    ui->listWidget->setLineWidth(2);
    ui->listWidget->setResizeMode(QListWidget::Adjust);
    ui->listWidget->setMovement(QListWidget::Static);
    //初始化接收udp
    m_recv_socket = new QUdpSocket(this);
    m_recv_socket->bind(QHostAddress::LocalHost, LISTEN_PORT);
    connect(m_recv_socket, &QUdpSocket::readyRead, this, &MainWindow::read_udp_data);
    //初始化发送udp
    m_send_socket = new QUdpSocket(this);

    connect(ui->listWidget, &QListWidget::itemDoubleClicked, this, &MainWindow::on_dbclicked_item);
    hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_visable(bool is_visable)
{
    setVisible(is_visable);
}

void MainWindow::open_pics(PicsDir dir_path)
{
    ui->listWidget->clear();
    QDir dir(dir_path);
    dir.setFilter(QDir::Files);
    QStringList filter;
    filter << "*.tif";
    filter << "*.jpg";
    filter << "*.png";
    filter << "*.tif";
    auto file_list = dir.entryInfoList(filter);
    foreach (auto file, file_list) {
        if (file.fileName().endsWith("tif")) {
            CxImage img;
            if (!img.Load(file.absoluteFilePath().toStdWString().c_str())) {
                qDebug() << "load error:" + file.fileName();
                continue;
            }
            img.SetCodecOption(5);
            uint8_t* img_buf = nullptr;
            int img_size = 0;
            img.Encode(img_buf, img_size, CXIMAGE_FORMAT_TIF);
            QPixmap p;
            if (!p.loadFromData(img_buf, img_size)) {
                qDebug() << "tif to pixmap failed!";
            }
            img.FreeMemory(img_buf);
            img.Clear();
            QListWidgetItem* item = new QListWidgetItem;
            item->setIcon(p);
            item->setText(file.fileName());
            ui->listWidget->addItem(item);
#if 0
            TIFF* tif = TIFFOpen(file.absoluteFilePath().toStdString().c_str(), "r");
            if (!tif) {
                qDebug() << "tif open error!";
                continue;
            }
            uint32_t t_width, t_height;
            if (TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &t_width) != 1) {
                qDebug() << "get width error!";
                continue;
            }
            if (TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &t_height) != 1) {
                qDebug() << "get height error!";
                continue;
            }
            qDebug() << "here";
            QImage img(t_width, t_height, QImage::Format_ARGB32);
            QVarLengthArray<quint16, 1024> src(t_width);
            for (uint32_t y = 0; y < t_height; ++y) {
                TIFFReadScanline(tif, src.data(), y, 0);
                quint8* dst = (quint8*)img.scanLine(y);
                for (uint32_t x = 0; x < t_width; ++x) {
                    dst[x] = src[x] >> 8;
                }
            }
            QListWidgetItem* img_item = new QListWidgetItem;
            QPixmap p = QPixmap::fromImage(img);
            img_item->setIcon(QIcon(p));
            img_item->setText(file.fileName());
            ui->listWidget->addItem(img_item);
#endif
        } else {
            QListWidgetItem* img = new QListWidgetItem;
            QImageReader img_reader(file.absoluteFilePath());
            auto img_content = img_reader.read();
            QPixmap p = QPixmap::fromImage(img_content);
            img->setIcon(QIcon(p));
            img->setText(file.fileName());
            ui->listWidget->addItem(img);
        }
    }
}

MainWindow::Page MainWindow::get_pic_page(const FileName& file_name)
{
    //文件名为 任务号.页码.序号.扩展名
    //这个函数应该放到通用功能类中
    auto parts = file_name.split('.');
    return parts.size() == 4 ? parts[1].toInt() : -1;
}

void MainWindow::read_udp_data()
{
    qDebug() << "read";
    while (m_recv_socket->hasPendingDatagrams()) {
        auto data = m_recv_socket->receiveDatagram().data();
        process_recv_data(data);
    }
}

void MainWindow::on_dbclicked_item(QListWidgetItem* item)
{
    //发给服务端的page消息为{"page":1}ZK
    qDebug() << item->text();
    auto page = get_pic_page(item->text());
    QJsonObject obj;
    obj.insert("page", page);
    QJsonDocument doc;
    doc.setObject(obj);
    auto data = doc.toJson(QJsonDocument::Compact);
    data.append(MAGIC_FIELD.toUtf8());
    qDebug() << data;
    m_send_socket->writeDatagram(data, data.size(), QHostAddress::LocalHost, SERVER_PORT);
}

void MainWindow::process_recv_data(RecvData& data)
{
    //服务端指令目前为{"pic_dir":"..."}ZK
    if (data.endsWith(MAGIC_FIELD.toUtf8())) {
        data.chop(2); //去掉magic标记
        //路径中可能有中文，需要处理一下
        auto temp_str = QString::fromLocal8Bit(data);
        QJsonParseError err;
        auto json = QJsonDocument::fromJson(temp_str.toUtf8(), &err);
        if (err.error == QJsonParseError::NoError) {
            auto jobj = json.object();
            auto it = jobj.find("pic_dir");
            if (it != jobj.end()) {
                qDebug() << it.value();
                auto dir_parse = it.value().toString().replace(R"(\\)", R"(\)");
                open_pics(dir_parse);
                show();
            }
        } else {
            qDebug() << err.errorString();
            qDebug() << data;
        }
    } else {
        qDebug() << data;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    hide();
    event->ignore();
}
