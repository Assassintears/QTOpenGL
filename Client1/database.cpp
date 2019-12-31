#include <database.h>
#include <QThread>
#include <QTime>

DataBase::DataBase(const QString& connectName)
    :QObject (nullptr)
{
    m_stop = true;
    m_isBreak = true;
    //!开始连接数据库
    m_dataBase = QSqlDatabase::addDatabase("QODBC", connectName);
    m_dataBase.setHostName("192.168.2.136");
    m_dataBase.setDatabaseName("DB");//!如果是QODBC驱动，此处应该是数据源名称
    m_dataBase.setUserName("sa");
    m_dataBase.setPassword("sa");
    m_dataBase.setPort(1433);
    m_connectName = connectName;
}

DataBase::~DataBase()
{
}

void DataBase::selectRealDataFromDB(const QString& sql)
{
    if (m_stop || m_isBreak)
    {
        return;
    }
    QMutexLocker locker(&m_mutex);
    if (!m_dataBase.isOpen())
    {
        if (!m_dataBase.open())
        {
            qDebug() << "数据库断开\n" << m_dataBase.lastError().text() << "\n";
            emit State(2);
            return;
        }
    }
    QSqlQuery query(m_dataBase);
    query.exec(sql);
    if (query.isActive())
    {
        //!取结果集
        QVector<QVector<float>> data;
        while (query.next())
        {
            QVector<float> tmp;
            QSqlRecord rec = query.record();
            if (rec.count() < 1)
            {
                continue;
            }
            for (int i = 0; i < rec.count() - 1; ++i)
            {
                QVariant vt = rec.value(i);
                if (vt.isNull())
                {
                    break;
                }
                else
                {
                    tmp.push_back(vt.toFloat());
                }
            }
            data.push_back(tmp);
            rec.clear();
            tmp.clear();
        }

        //! 防止数据库突然断开，最后一次数据来不及取导致最后一行数据列数与之前的行的列数不一致
        if (!data.isEmpty())
        {
            if (data.size() > 1)
            {
                if (data[0].size() != data[data.size() - 1].size())
                {
                    data.erase(data.end() - 1, data.end());
                }
                emit dataBase(data);
            }
        }
        query.clear();
    }
    m_dataBase.close();
    return;
}

void DataBase::selectAll(int start, int end)
{
    if (m_isBreak)
    {
        return;
    }
    QMutexLocker locker(&m_mutex);
    if (!m_dataBase.isOpen())
    {
        if (!m_dataBase.open())
        {
            qDebug() << "数据库断开\n" << m_dataBase.lastError().text() << "\n";
            emit State(2);
            return;
        }
    }
    QString sql = "select * from 一号煤场";
    QSqlQuery query(m_dataBase);
    query.exec(sql);
    QVector<QVector<int>> data;
    if (query.isActive())
    {
        //!取结果集

        while (query.next())
        {
            QVector<int> tmp;
            QSqlRecord rec = query.record();
            if (rec.count() < 1)
            {
                continue;
            }
            for (int i = 0; i < rec.count() - 1; ++i)
            {
                QVariant vt = rec.value(i);
                if (vt.isNull())
                {
                    break;
                }
                else
                {
                    tmp.push_back(vt.toInt());
                }
            }
            data.push_back(tmp);
            rec.clear();
            tmp.clear();
        }

        //! 防止数据库突然断开，最后一次数据来不及取导致最后一行数据列数与之前的行的列数不一致
        if (!data.isEmpty())
        {
            if (data.size() > 1)
            {
                if (data[0].size() != data[data.size() - 1].size())
                {
                    data.erase(data.end() - 1, data.end());
                }
                emit calcVolum(start, end, data);
            }
        }
        query.clear();
    }
    m_dataBase.close();
    locker.unlock();
    return;
}

void DataBase::initDB()
{
    if (m_isInited)
    {
        return;
    }
    if (!m_dataBase.isOpen())
    {
        if (!m_dataBase.open())
        {
            emit State(1);
            return;
        }
    }
     QSqlQuery query(m_dataBase);
     QString sql = "SELECT * FROM InitTable";
     query.exec(sql);
     if (!query.isActive())
     {
         emit State(1);
         return;
     }

     while (query.next()) {
         QSqlRecord rec = query.record();
         //! 取总表中的煤场ID和实时数据表
        int id = rec.value(rec.indexOf("SiteID")).toInt();
        QString name = rec.value(rec.indexOf("TableName")).toString();
//        m_coalInfo[id] = name;
     }
     m_isInited = true;
     query.clear();
     m_dataBase.close();
}

void DataBase::StartStopScanner(const QString& sql, const QString& select)
{
    QMutexLocker locker(&m_mutex);
   if (!m_dataBase.isOpen())
   {
       if (!m_dataBase.open())
       {
           emit State(2);
           return;
       }
   }
    //!写入指令
    QSqlQuery query(m_dataBase);
    if (!select.isEmpty())//! 开始指令
    {
        query.exec(select);
        if (query.isActive())
        {
//            query.next();//! 调试用，实际需要取消注释，避免扫描仪在工作过程中再次开始
            QSqlRecord rec = query.record();
            QVariant vt = rec.value(0);
            if (0 != vt.toInt())
            {
                rec.clear();
                query.clear();
                m_dataBase.close();
                //! 当前已经在扫描了
                emit State(1);
                return;
            }
            else {
                rec.clear();
                query.clear();
            }
        }
        else
        {
            query.clear();
            m_dataBase.close();
            emit State(3);
            return;
        }
    }

    query.exec(sql);
    if (query.isActive())
    {
        emit State(0);
        query.clear();
        m_dataBase.close();
        return;
    }
    else {
        emit State(3);
        query.clear();
        m_dataBase.close();
        return;
    }
}

void DataBase::reconnectDB()
{
    //! 避免多次重连--定时器在重连过程中会一直发重连信号，所有的重连信号存储在消息队列中，按顺序处理
    if (!m_isBreak)
    {
        return;
    }
    QMutexLocker lock(&m_mutex);
    m_dataBase.close();
    if (m_dataBase.open())
    {
        qDebug() << "重连成功\n";
        m_isBreak = false;
        emit State(4);
        return;
    }
    else {
        QThread::sleep(1);
        return;
    }
}

