#include "clientwidget.h"
#include "ui_clientwidget.h"
#include <QDebug>
#include <QMessageBox>
#include <QHostAddress>
ClientWidget::ClientWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ClientWidget)
{
    ui->setupUi(this);
    isStart=true;
    tcpSocket=new QTcpSocket(this);
    connect(tcpSocket,&QTcpSocket::readyRead,[=](){
      QByteArray buf=tcpSocket->readAll();
        if(true==isStart)
        {
            isStart=false;
            fileName=QString(buf).section("##",0,0);
            fileSize=QString(buf).section("##",1,1).toInt();
            recvSize=0;

            file.setFileName(fileName);
            bool isOk=file.open(QIODevice::WriteOnly);
            if(false==isOk)
            {
                qDebug()<<"write error";
            }
        }
        else
        {
            qint64 len=file.write(buf);
            recvSize+=len;
            if(recvSize==fileSize)
            {
                file.close();
                QMessageBox::information(this,"write over","file write over");
                tcpSocket->disconnect();
                tcpSocket->close();
            }
        }
    });
}

ClientWidget::~ClientWidget()
{
    delete ui;
}

void ClientWidget::on_pushButton_clicked()
{
    QString ip=ui->lineEdit->text();
    quint16 port=ui->lineEdit_2->text().toInt();
    tcpSocket->connectToHost(QHostAddress(ip),port);
}
