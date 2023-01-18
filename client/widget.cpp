#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QStringList>
#include <QTime>

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->lineEdit_IP->setText("127.0.0.1");
    ui->lineEdit_Port->setText("8888");
    isFileInfo = true;
         ui-> progressBar-> setValue (0);

    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead, [=](){

        QByteArray buf = tcpSocket->readAll();

                 if (isFileInfo == true)
        {
            isFileInfo = false;
            RSize = 0;

            QStringList list = QString(buf).split('|');
                         fileName = list.at (0);
                         fileSize = QString (list.at(1)).toInt();


            ui->progressBar->setRange(0, fileSize/1024);
            ui->progressBar->setValue(0);

            file.setFileName(fileName);

            if (!file.open(QFile::WriteOnly))
                      {
                        ui->ErrorLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + (" Cannot open the file "));
                        isFileInfo = true;
                      }
        }
                 else
        {

            int len = file.write(buf);

            RSize += len;

            ui->progressBar->setValue(RSize/1024);

            if (RSize == fileSize)
            {
                ui-> textEdit-> append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + (" File recieved"));

                file.close();

                isFileInfo = true;
                RSize = 0;
            }
        }
    });

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_btnConnect_clicked()
{

    QString ip = ui->lineEdit_IP->text();
    int port = ui->lineEdit_Port->text().toInt();

    tcpSocket->connectToHost(QHostAddress(ip), port);
    if(!tcpSocket->waitForConnected(2000))
    {
       ui->ErrorLog->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss") + (" Failed connection "));
    }
else

    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
    {
       ui->btnConnect->setEnabled(false);
    }
}
