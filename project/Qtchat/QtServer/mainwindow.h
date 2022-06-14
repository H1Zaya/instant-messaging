#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_setListen_clicked();

    void on_sendMsg_clicked();

    void on_record_copyAvailable(bool b);

private:
    Ui::MainWindow *ui;
    QTcpServer* m_s;
    QTcpSocket* m_tcp;
    QLabel* m_status;
};
#endif // MAINWINDOW_H
