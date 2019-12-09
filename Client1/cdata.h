#ifndef CDATA_H
#define CDATA_H
#include <QVector>
#include <QVector3D>
#include <QtNetwork>
//#include <QThread>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonValue>
#include <fstream>

class MainWindow;

class CData : public QObject
{
    Q_OBJECT
public:
    CData();
    virtual ~CData();

public:
    void SignalsSlots();

public:
    float y0 = 0.0f;
    float stepy = 1.0f;

signals:
    void receive(QJsonDocument& doc);
    void hasData(QVector<QVector3D> localData, QVector<unsigned int> ind);

public slots:
    void acceptConnection();
    void onDataProcess(QJsonDocument& doc);
    void errors(QAbstractSocket::SocketError);
    void connected();
    void onDisconnected();
    void readBuf();
    void receiveDataFromDB(QVector<QVector<float>> db);//!从数据库接收到数据


private:
    bool Indx(QVector<unsigned int>& indx, const int hang, const int pointNum);

    QTcpServer* server;
    QTcpSocket* socket;
    std::ofstream file;
};

#endif // CDATA_H
