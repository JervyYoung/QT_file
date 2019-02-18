#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>
#include <QTcpServer>  //监听
#include <QTcpSocket>   // 通讯
#include <QFile>
#include <QTimer>

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = 0);
    ~ServerWidget();
    void sendData();

private slots:
    void on_buttonFile_clicked();

    void on_buttonSend_clicked();

private:
    Ui::ServerWidget *ui;
    QTcpServer *tcpServer;
    QTcpSocket *tcpSocket;
    QString fileName;
    qint64 fileSize;
    qint64 sendSize;//已经发送文件大小
    QFile  file;
    QTimer timer;
};

#endif // SERVERWIDGET_H
