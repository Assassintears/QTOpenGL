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
#include <QVector3D>

//typedef struct PointAttr
//{
//    QVector3D point;
//    QVector3D normal;
//    QVector3D color;
//};


class CData : public QObject
{
    Q_OBJECT
public:
    CData();
    virtual ~CData();

public:
    float y0 = 0.0f;    //! 单位：cm
    float stepy = 20.0f;//! 单位：cm

signals:
    //! points3D: 模型顶点坐标
    //! normal：模型法向量
    //! color：模型颜色
    void hasData(QVector<QVector3D> points3D, QVector<QVector3D> normal, QVector<QVector3D> color);

public slots:
    void calcPoint(QVector<QVector<float>> db);//!从数据库接收到数据

private:
    bool reshape(const QVector<QVector<QVector3D>>& data,
                 QVector<QVector3D>& point3D, QVector<QVector3D>& normal,
                 QVector<QVector3D>& color);
};

#endif // CDATA_H
