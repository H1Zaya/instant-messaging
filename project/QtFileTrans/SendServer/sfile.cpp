#include "sfile.h"

#include <QFile>
#include <QHostAddress>
#include <QFileInfo>

SFile::SFile(QObject *parent) : QObject(parent)
{

}

void SFile::connectServer(unsigned short port, QString ip)
{
    tSock = new QTcpSocket;
    tSock->connectToHost(QHostAddress(ip),port);

    connect(tSock,&QTcpSocket::connected,this,&SFile::connectOK);
    connect(tSock,&QTcpSocket::disconnected,this,[=](){
        tSock->close();
        tSock->deleteLater();
        emit over();
    });
}

void SFile::sendFile(QString path)
{
    QFile file(path);
    QFileInfo info(path);
    int fileSize = info.size();
    file.open(QFile::ReadOnly);

    while(!file.atEnd())
    {
        int percent;
        // first sending send with the fileSize
        static int num=0;
        if(num==0){
            tSock->write((char*)&fileSize,4);
        }
        QByteArray line = file.readLine();
        num+=line.size();
        percent = num*100/fileSize;
        emit cPercent(percent);
        tSock->write(line);
    }
}
