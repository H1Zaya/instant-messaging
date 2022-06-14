#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QHostAddress>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // QFont ft,ft1;
    // ft.setPointSize(10);
    // ft1.setPointSize(8);
    // ui->port_num->setFont(ft);
    // ui->ip_num->setFont(ft);

    // 回车绑定槽函数 发送(即光标在我们的msg组件时,回车可以执行sendmsg组件的槽函数具体指令
    // 其中msg为linetext格式的组件,sendMsg为PushButton组件
    // 大大便利了信息的发送
    connect(
    ui->msg,
    SIGNAL(returnPressed()),
    ui->sendMsg,
    SIGNAL(clicked()),
    Qt::UniqueConnection
    );

    ui->record->setReadOnly(true);

    ui->port->setText("8899");
    ui->ip->setText("127.0.0.1");
    setWindowTitle("Client");
    ui->disconnected->setDisabled(true);

    // 创建监听对象
    m_tcp = new QTcpSocket(this);

    // m_tcp是用于通讯的套接字
    connect(m_tcp,&QTcpSocket::readyRead,this,[=](){
        QByteArray data = m_tcp->readAll();
        ui->record->append("Server: " + data);
    });


    connect(m_tcp,&QTcpSocket::disconnected,this,[=](){
        m_tcp->close();
        //m_tcp->deleteLater();
        m_status->setText("未连接");
        ui->record->append("已经成功断开连接!!");
        ui->disconnected->setDisabled(true);
        ui->connect->setEnabled(true);
    });

    connect(m_tcp,&QTcpSocket::connected,this,[=](){
        m_status->setText("已连接");
        ui->record->append("已经成功连接至服务器!!");
        ui->connect->setDisabled(true);
        ui->disconnected->setEnabled(true);
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

void MainWindow::on_sendMsg_clicked()
{
    QString msg = ui->msg->text();
    m_tcp->write(msg.toUtf8()); //转换为Utf-8 编码
    ui->record->append("Client: " + msg);
    ui->msg->clear();
}

void MainWindow::on_connect_clicked()
{
    QString ip = ui->ip->text();
    unsigned short port = ui->port->text().toUShort();
    m_tcp->connectToHost(QHostAddress(ip),port);
}

void MainWindow::on_disconnected_clicked()
{
    m_tcp->close();
    ui->connect->setDisabled(false);
    ui->disconnected->setEnabled(false);
}
