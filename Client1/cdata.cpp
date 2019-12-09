#include <cdata.h>
#include <algorithm>
#include <QFile>
#include <QTextStream>

CData::CData()
{
    server = new QTcpServer;
//    QHostAddress host("192.168.2.136");
//    server->listen(host, 7171);
    SignalsSlots();

    file.open("data.txt", std::ios::app);
    if (!file.is_open())
    {
        qDebug() << "文件打开失败\n";
    }

}

CData::~CData()
{
    server->close();
}



void CData::SignalsSlots()
{
    connect(server, &QTcpServer::newConnection, this, &CData::acceptConnection);
    connect(this, &CData::receive, this, &CData::onDataProcess);

}

void CData::errors(QAbstractSocket::SocketError e)
{
    if (QAbstractSocket::UnknownSocketError != e)
    {
        ;
    }
}

void CData::onDisconnected()
{
    QHostAddress host("192.168.2.136");
    server->listen(host, 7171);
//    server->close();
}

void CData::acceptConnection()
{
//    socket = server->nextPendingConnection();
//    connect(socket, &QTcpSocket::readyRead, this, &CData::readBuf);
//    connect(socket, &QTcpSocket::disconnected, this, &CData::onDisconnected);
//    connect(socket, &QTcpSocket::connected, this, &CData::connected);
}


void CData::connected()
{
    qDebug() << "yes\n";
}


void CData::readBuf()
{
    QByteArray buf;
    buf = socket->readAll();
    QJsonDocument document;
    document = QJsonDocument::fromJson(buf);
    emit receive(document);
}

 void CData::onDataProcess(QJsonDocument& doc)
 {
 }


 void CData::receiveDataFromDB(QVector<QVector<float>> db)
 {
     if (db.empty())
     {
         qDebug() << "No data" << "\n";
         return;
     }
     int rows = db.size();
     int cols = db[0].size();
     int pointNum = cols - 2;
    QVector<QVector<QVector3D>> data;
    QVector<int> hang;
    for (int i = 0; i < rows; ++i)
    {
        float x = db[i][0];
        QVector<QVector3D> tmp;//!当前行数据
        for (int j = 1; j < db[i].size() - 2; ++j)
        {
            float y = y0 + j * stepy;
            tmp.push_back(QVector3D(x, y, db[i][j]));
        }
        if (2 != db[i].size() - pointNum)
        {
            qDebug() << "--" << i << "  size = " << db[i].size() << "\n";
        }
        data.push_back(tmp);
    }
    //!生成索引,重组数据
    QVector<unsigned int> ii;
    if (data.empty())
    {
        qDebug() << "No data" << "\n";
        return;
    }
    int rowIndx = data.size();
    int colIndx = data[0].size();
    if (Indx(ii, rowIndx, colIndx))
    {
        QVector<QVector3D> local;
        for (int i = 0; i < rowIndx; ++i)
        {
            for (int j = 0; j < colIndx; ++j)
            {
                QVector3D dd = data[i][j];
                local.push_back(dd);
            }
        }
        emit hasData(local, ii);
    }

 }


bool CData::Indx(QVector<unsigned int>& indx, const int hang, const int pointNum)
{
    int it = hang;
    if (it >= 2)//!至少有2行数据
    {
        for (int i = 0; i < it - 1; ++i)
        {
            for (int j = 0; j < pointNum - 1; ++j)
            {
                //当前行2个点序号
                unsigned int pre_1 = i * pointNum + j;
                unsigned int pre_2 = i * pointNum + (j + 1);
                //后一行2个点序号
                unsigned int next_1 = (i + 1) * pointNum + j;
                unsigned int next_2 = (i + 1) * pointNum + (j + 1);
                //4个点，2个三角形
                indx.push_back(pre_1);
                indx.push_back(pre_2);
                indx.push_back(next_1);

                indx.push_back(next_1);
                indx.push_back(next_2);
                indx.push_back(pre_2);
            }
        }
        return true;
    }
    else {
        return false;
    }
}
