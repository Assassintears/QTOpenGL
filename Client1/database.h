#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVector>
#include <QVector3D>
#include <QMutex>
#include <QMutexLocker>

//! 数据库的一些操作
class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(const QString& connectName);
    virtual ~DataBase();

signals:
    void dataBase(QVector<QVector<float>> data);//! 对外发送表格中的数据
    //! 开始测量状态，0：正常打开，1：该每场已经在测量， 2：打开数据库失败，3：指令无效
    void State(int);

public slots:
    void selectRealDataFromDB(const QString& sql);//! 从数据库读取数据
    void StartStopScanner(const QString& sql, const QString& select);
private:
    QSqlDatabase m_dataBase;
    QMutex m_mutex;
};


#endif // DATABASE_H
