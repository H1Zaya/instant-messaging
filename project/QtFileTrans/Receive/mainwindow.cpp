#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>
#include <qmessagebox.h>
#include "recvfile.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->portText->setText("8888");

    Rs = new QTcpServer(this);

    connect(Rs,&QTcpServer::newConnection,this,[=](){
        QTcpSocket* tcp = Rs->nextPendingConnection();
        // subThread
        RecvFile* subT = new RecvFile(tcp);
        subT->start();

        connect(subT,&RecvFile::down,this,[=](){
            subT->exit();
            subT->wait();
            subT->deleteLater();
            QMessageBox::information(this,"RECV","recv down.");
        });
    });

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listen_clicked()
{
    unsigned short port = ui->portText->text().toUShort();
    Rs->listen(QHostAddress::Any,port);
}
