#ifndef SFILE_H
#define SFILE_H

#include <QObject>
#include <QTcpSocket>

class SFile : public QObject
{
    Q_OBJECT
public:
    explicit SFile(QObject *parent = nullptr);

    // connect
    void connectServer(unsigned short port,QString ip);

    // send
    void sendFile(QString path);

signals:
    void connectOK();
    void over();
    void cPercent(int num);

private:
    QTcpSocket* tSock;

};

#endif // SFILE_H
