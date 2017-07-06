#include "udp2.h"
#include <QDebug>
#include <QTimer>
#include <QDateTime>

Udp2::Udp2(QWidget *parent) : QWidget(parent)
{
    _udp = new QUdpSocket;
    //_udp->bind(10002);//绑定端口
    _udp->bind(QHostAddress::AnyIPv4, 10002);

    //_udp->joinMulticastGroup(QHostAddress("224.0.0.131"));//加入到多播

    connect(_udp, SIGNAL(readyRead()), this, SLOT(slotReadyRead()));

    QTimer * timer = new QTimer;
    timer->setInterval(1000);
    timer->start();

    connect(timer, &QTimer::timeout, this, [&](){
        quint64 timestamp = QDateTime::currentMSecsSinceEpoch();
        QString str = QString::number(timestamp);   //数字转换为字符串
        _udp->writeDatagram(str.toUtf8(), QHostAddress("127.0.0.1"), 10001);
    });
}

void Udp2::slotReadyRead()
{
    //接收
    while(_udp->hasPendingDatagrams())
    {
        quint32 datagramSize = _udp->pendingDatagramSize();//获取下个包的大小
        QByteArray buf(datagramSize, 0);
        _udp->readDatagram(buf.data(), buf.size());
        qDebug() << "udp2:" << buf;
    }


}
