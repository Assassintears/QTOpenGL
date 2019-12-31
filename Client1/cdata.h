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

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif


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
    void Volum(float);

public slots:
    void calcPoint(QVector<QVector<float>> db);//!从数据库接收到数据
    void calcVolum(int start, int end, QVector<QVector<int>> db);

private:
    bool reshape(const QVector<QVector<QVector3D>>& data,
                 QVector<QVector3D>& point3D, QVector<QVector3D>& normal,
                 QVector<QVector3D>& color);

    //! 根据海伦公式计算三角形面积
    //! s = sqrt(p(p-a)(p-b)(p-c)),
    //! 其中，p为三角形半周长，即
    //! p = (a + b + c) / 2
    float _calVolum(QVector<int> first, QVector<int> second, int stepx = 200, int stepy = 200);
};

#endif // CDATA_H
