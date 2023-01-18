#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

private slots:

    void on_btnConnect_clicked();

private:
    Ui::Widget *ui;

         QTcpSocket * tcpSocket;

         QFile file;
         QString fileName;
         quint32 fileSize;
         quint32 RSize;
         bool isFileInfo;
};
