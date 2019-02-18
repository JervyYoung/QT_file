#include "serverwidget.h"
#include "ui_serverwidget.h"
#include <QFileDialog>
#include <QDebug>
#include <QFileInfo>
ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    tcpServer=new QTcpServer(this);
    tcpServer->listen(QHostAddress::Any,8888);

    ui->buttonFile->setEnabled(false);
    ui->buttonSend->setEnabled(false);
    //如果客户和服务端连接会触发newConnection()
    connect(tcpServer,&QTcpServer::newConnection,[=](){
        tcpSocket=tcpServer->nextPendingConnection();
        QString ip=tcpSocket->peerAddress().toString();
        quint16 port=tcpSocket->peerPort();
        QString str=QString("[%1:%2]succeed con").arg(ip).arg(port);
        ui->textEdit->setText(str);
        ui->buttonFile->setEnabled(true);


    });

    connect(&timer,&QTimer::timeout,[=](){
      timer.stop();
      sendData();

    });

}

ServerWidget::~ServerWidget()
{
    delete ui;
}

void ServerWidget::on_buttonFile_clicked()
{
   QString filePath= QFileDialog::getOpenFileName(this,"open","../") ;
   if(false==filePath.isEmpty())
   {
       fileName.clear();
       fileSize=0;
       QFileInfo info(filePath);
       fileName=info.fileName();
       fileSize=info.size();
       sendSize=0;

       file.setFileName(filePath);

       bool isOk=file.open(QIODevice::ReadOnly);
       if(false==isOk)
       {
           qDebug()<<"open fail";
       }
       ui->textEdit->append(filePath);
       ui->buttonFile->setEnabled(false);
       ui->buttonSend->setEnabled(true);
   }
   else
   {
       qDebug()<<"path is wrong";
   }


}

void ServerWidget::on_buttonSend_clicked()
{
     QString head=QString("%1##%2").arg(fileName).arg(fileSize);
     qint64 len=tcpSocket->write(head.toUtf8());
     if(len>0)
     {
         timer.start(20);
     }
     else
     {
         qDebug()<<"head send fail";
         file.close();
         ui->buttonFile->setEnabled(true);
         ui->buttonSend->setEnabled(false);
     }
}

void ServerWidget::sendData(){
    qint64 len=0;
    do
    {
        char buf[4*1024]={0};
        len=0;
        len=file.read(buf,sizeof(buf));
        tcpSocket->write(buf,len);

       sendSize+=len;
    }while(len>0);
    if(sendSize==fileSize)
    {
        ui->textEdit->append("send over");
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
    }
}
