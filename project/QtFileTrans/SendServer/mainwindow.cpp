#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QThread>
#include "sfile.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // ip & port initialized
    ui->ipText->setText("127.0.0.1");
    ui->portText->setText("8888");
    // progressBar shows the progress of sending file
    ui->progressBar->setRange(0,100);
    ui->progressBar->setValue(0);

    // new thread
    QThread* t = new QThread;

    SFile* sender = new SFile;

    sender->moveToThread(t);

    connect(this,&MainWindow::sendFile,sender,&SFile::sendFile);

    connect(this,&MainWindow::startConnect,sender,&SFile::connectServer);

    //
    connect(sender,&SFile::connectOK,this,[=](){
        QMessageBox::information(this,"connect","connect down");
    });
    connect(sender,&SFile::over,this,[=](){
        // release
        t->quit();
        t->wait();
        sender->deleteLater();
        t->deleteLater();
    });

    connect(sender,&SFile::cPercent,ui->progressBar,&QProgressBar::setValue);

    t->start();

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connec_clicked()
{
    QString ip = ui->ipText->text();
    unsigned short port = ui->portText->text().toUShort();
    emit startConnect(port,ip);
}

void MainWindow::on_selectButton_clicked()
{
    QString path = QFileDialog::getOpenFileName();
    if(path.isEmpty()){
        QMessageBox::warning(this,"open file","the file path cannot be null");
        return;
    }
    ui->filePath->setText(path);
}

void MainWindow::on_sendButton_clicked()
{
    emit sendFile(ui->filePath->text());
}
