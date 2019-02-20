#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QFile>

namespace Ui {
class ClientWidget;
}

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ClientWidget(QWidget *parent = 0);
    ~ClientWidget();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ClientWidget *ui;
    QTcpSocket *tcpSocket;
    QString fileName;
    qint64 fileSize;
    qint64 recvSize;//已经发送文件大小
    QFile  file;
    bool isStart;
};

#endif // CLIENTWIDGET_H
