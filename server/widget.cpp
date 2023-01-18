#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QFileInfo>
#include <QFileDialog>
#include <QDebug>
#include <QTime>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    tcpServer = new QTcpServer(this);
    tcpSocket = new QTcpSocket(this);

    tcpServer->listen(QHostAddress::Any, 8888);

    connect(tcpServer, &QTcpServer::newConnection, [=](){
        tcpSocket = tcpServer->nextPendingConnection();

        QString ip = tcpSocket->peerAddress().toString();
        int port = tcpSocket->peerPort();

                 QString str = QString("[%1:%2] connected").arg(ip).arg(port);
        ui->textEdit->setText(str);

        connect(tcpSocket, &QTcpSocket::readyRead, [=](){

            QByteArray buf = tcpSocket->readAll();

            if (QString(buf) == "file done")
            {
                ui->textEdit->append("File sent successfully");
                file.close();

                tcpSocket->disconnectFromHost();
                tcpSocket->close();
            } else {
                ui->ErrorLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + ("Failed to send file"));
            }
        });
    });

    connect(&timer, &QTimer::timeout, [=](){

        timer.stop();

        sendFileData();
    });
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_btnSelect_clicked()
{

    QString filePath = QFileDialog::getOpenFileName();
    if (!filePath.isEmpty())
    {
        fileName.clear();
        fileSize = 0;
        sendSize = 0;

        QFileInfo info(filePath);
                 fileName = info.fileName ();
                 fileSize = info.size ();

        file.setFileName(filePath);

        if (file.open(QFile::ReadOnly))
        {
            ui->textEdit->append(("File path: ")+(filePath));
        }
    }
}

void Widget::on_btnSend_clicked()
{
         ui-> textEdit->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + (" Sending file.."));

    QString fileInfo = QString("%1|%2").arg(fileName).arg(fileSize);

    int len = tcpSocket->write(fileInfo.toUtf8());
    if (len > 0)
    {
        timer.start(20);
    }
    else
    {
        ui->ErrorLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + (" Failed to send file information "));
        file.close();
    }
}

void Widget::sendFileData()
{
    qint64 len = 0;
    do
    {
        char buf[1024 * 4] = {0};

        len = file.read(buf, sizeof(buf));

        len = tcpSocket->write(buf, len);

        sendSize += len;
    }while(len > 0);
}
