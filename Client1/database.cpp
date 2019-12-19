#include <database.h>

DataBase::DataBase(const QString& connectName)
    :QObject (nullptr)
{
    //!开始连接数据库
    m_dataBase = QSqlDatabase::addDatabase("QODBC", connectName);
    m_dataBase.setHostName("192.168.2.136");
    m_dataBase.setDatabaseName("DB");//!如果是QODBC驱动，此处应该是数据源名称
    m_dataBase.setUserName("sa");
    m_dataBase.setPassword("sa");
    m_dataBase.setPort(1433);
}

DataBase::~DataBase()
{}

void DataBase::selectRealDataFromDB(const QString& sql)
{
    QMutexLocker locker(&m_mutex);
    if (!m_dataBase.isOpen())
    {
        if (!m_dataBase.open())
        {
            qDebug() << "打开数据库失败\n" << m_dataBase.lastError().text() << "\n";
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
            if (1 != tmp.size())
            {
                data.push_back(tmp);
            }
            else
            {
                qDebug() << "size = " << tmp.size() << "\n";
            }
            rec.clear();
            tmp.clear();
        }
        if (!data.isEmpty())
        {
            emit dataBase(data);
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
            return State(2);
        }
    }
    //!写入指令
    QSqlQuery query(m_dataBase);
    if (!select.isEmpty())//! 开始指令
    {
        query.exec(select);
        if (query.isActive())
        {
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
        return;
    }
    else {
        emit State(3);
        return;
    }
}
