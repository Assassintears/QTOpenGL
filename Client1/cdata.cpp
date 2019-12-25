#include <cdata.h>
#include <algorithm>
#include <QFile>
#include <QTextStream>
#include <QThread>

CData::CData()
{
}

CData::~CData()
{
}


void CData::calcPoint(QVector<QVector<float>> db)
{
//    qDebug() << "数据处理线程: " << QThread::currentThread() << "\n";
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
       float x = db[i][0] / 1000.0f;
       QVector<QVector3D> tmp;//!当前行数据
       for (int j = 1; j < db[i].size() - 2; ++j)
       {
           float y = (y0 + j * stepy) / 100.0f; //! cm转化为m
           float z = db[i][j] / 1000.0f;        //! mm转化为m
           tmp.push_back(QVector3D(x, y, z));
       }
       if (2 != db[i].size() - pointNum)
       {
           qDebug() << "--" << i << "  size = " << db[i].size() << "\n";
       }
       data.push_back(tmp);
   }
   //! 重组数据
   QVector<unsigned int> ii;
   if (data.empty())
   {
       qDebug() << "No data" << "\n";
       return;
   }
   QVector<QVector3D> point;//! 顶点数据
   QVector<QVector3D> normal;
   QVector<QVector3D> color;
   reshape(data, point, normal, color);
   emit hasData(point, normal, QVector<QVector3D>());
}

bool CData::reshape(const QVector<QVector<QVector3D>>& data,
             QVector<QVector3D>& point3D, QVector<QVector3D>& normal,
             QVector<QVector3D>& color)
{
    if (!data.isEmpty())
    {
        int rows = data.size();
        int cols = data[0].size();
        if (1 == rows)
        {
            point3D = data[0];
            return true;
        }
        else
        {
            for (int i = 0; i < rows - 1; ++i)
            {
                for (int j = 0; j < cols - 1; ++j)
                {
                    //! 当前行2个点
                    QVector3D cur1 = data[i][j];
                    QVector3D cur2 = data[i][j + 1];
                    //! 下一行2个点
                    QVector3D next1 = data[i + 1][j];
                    QVector3D next2 = data[i + 1][j + 1];
                    //! 前一个三角形法向量
                    QVector3D n1 = QVector3D::normal((cur2 - cur1), (next1 - cur1));
                    //! 后一个三角形法向量
                    QVector3D n2 = QVector3D::normal((cur2 - next1), (next2 - next1));
                    point3D.push_back(cur1);
                    point3D.push_back(cur2);
                    point3D.push_back(next1);
                    point3D.push_back(next1);
                    point3D.push_back(cur2);
                    point3D.push_back(next2);
                    normal.push_back(n1);
                    normal.push_back(n1);
                    normal.push_back(n1);
                    normal.push_back(n2);
                    normal.push_back(n2);
                    normal.push_back(n2);
                }
            }
        }
        return true;
    }
    else {
        return false;
    }
}
