#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QFile>
#include <QTimer>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void sendFileData();

private slots:
    void on_btnSelect_clicked();

    void on_btnSend_clicked();

private:
    Ui::Widget *ui;

         QTcpServer * tcpServer;
         QTcpSocket * tcpSocket;

         QFile file;
         QString fileName;
         int fileSize;
         int sendSize;

         QTimer timer;
};
