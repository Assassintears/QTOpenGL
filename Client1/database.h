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
#include <QHash>

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

//! 数据库的一些操作
class DataBase : public QObject
{
    Q_OBJECT
public:
    DataBase(const QString& connectName);
    virtual ~DataBase();

signals:
    void dataBase(QVector<QVector<float>> data);                        //! 对外发送表格中的数据
    //! 开始测量状态，0：正常，1：该每场已经在测量， 2：数据库断开，3：指令无效, 4:数据库重连成功, 5: 正在重连
    void State(int);
    void calcVolum(int start, int end, QVector<QVector<int>> db);
    void Volum(float);

public slots:
    void selectRealDataFromDB(const QString& sql);                      //! 从数据库读取数据
    void StartStopScanner(const QString& sql, const QString& select);   //! 开始结束扫描
    void reconnectDB();                                                 //! 数据库断开重连
    void initDB();
    void selectAll(QString s, int start, int end);
private:
    QSqlDatabase m_dataBase;
    QMutex m_mutex;
    QString m_connectName;



public:
    bool m_isInited;
    bool m_stop;        //! 是否停止采集
    bool m_isBreak;     //! 数据库是否断开

};


#endif // DATABASE_H
