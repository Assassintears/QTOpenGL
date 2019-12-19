#include <cdata.h>
#include <algorithm>
#include <QFile>
#include <QTextStream>

CData::CData()
{
}

CData::~CData()
{
}



 void CData::calcPoint(QVector<QVector<float>> db)
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
        QVector<QVector3D> point;//! 顶点数据
        QVector<QVector3D> normal;
        for (int i = 0; i < rowIndx; ++i)
        {
            for (int j = 0; j < colIndx; ++j)
            {
                QVector3D dd = data[i][j];
                point.push_back(dd);
            }
        }
        //! 计算法向量
        for (int i = 0; i < ii.size(); i += 3)
        {

           QVector3D d0 = point[ii[i]];
           QVector3D d1 = point[ii[i + 1]];
           QVector3D d2 = point[ii[i + 2]];

           QVector3D n = QVector3D::normal((d1 - d0), (d2 - d0));
           normal.push_back(n);
           normal.push_back(n);
           normal.push_back(n);
        }
        emit hasData(point, QVector<QVector3D>(), QVector<QVector3D>(), ii);
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
                //4个点，2个三角形--顺时针
                indx.push_back(pre_1);
                indx.push_back(pre_2);
                indx.push_back(next_1);

                indx.push_back(next_1);
                indx.push_back(pre_2);
                indx.push_back(next_2);
            }
        }
        return true;
    }
    else {
        return false;
    }
}
