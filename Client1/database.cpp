#include <database.h>
#include <QThread>
#include <QTime>

DataBase::DataBase(const QString& connectName)
    :QObject (nullptr)
{
    m_stop = true;
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
    if (m_stop)
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
            qDebug() << data.size() << "\n" << data[data.size() - 1].size() << "\n";
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

void DataBase::StartStopScanner(const QString& sql, const QString& select)
{
    QMutexLocker locker(&m_mutex);
    if (!m_dataBase.isOpen())
    {
        if (!m_dataBase.open())
        {
            qDebug() << "Open DB error: " << m_dataBase.lastError().text() << "\n";
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
    m_dataBase.close();
    if (m_dataBase.open())
    {
        qDebug() << "重连成功\n";
        emit State(4);
        return;
    }
    else {
        QThread::sleep(1);
    }
}
