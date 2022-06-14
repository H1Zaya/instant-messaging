#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTcpSocket>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->record->setReadOnly(true);

    connect(
    ui->msg,
    SIGNAL(returnPressed()),
    ui->sendMsg,
    SIGNAL(clicked()),
    Qt::UniqueConnection
    );

    ui->port->setText("8899");
    setWindowTitle("Server");

    // 创建监听对象
    m_s= new QTcpServer(this);

    connect(m_s,&QTcpServer::newConnection,this,[=](){
        m_tcp = m_s->nextPendingConnection();

        // 连接上将文本改为已连接
        m_status->setText("已连接");

        // 检测是否可以接收数据
        connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
            QByteArray data = m_tcp->readAll();
            ui->record->append("Client: " + data);
        });

        connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
            m_tcp->close();
            //m_tcp->deleteLater();
            m_status->setText("未连接");
        });
    });

    // 给定连接状态
    m_status = new QLabel;
    m_status->setText("未连接");
    ui->statusbar->addWidget(new QLabel("连接状态: "));
    ui->statusbar->addWidget(m_status);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_setListen_clicked()
{
    unsigned short port = ui->port->text().toUShort();
    m_s->listen(QHostAddress::Any,port);
    ui->setListen->setDisabled(true);
}

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->text();
    m_tcp->write(msg.toUtf8()); //转换为Utf-8 编码
    ui->record->append("Server: " + msg);
    ui->msg->clear();
}


void MainWindow::on_record_copyAvailable(bool b)
{

}
