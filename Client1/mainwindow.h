#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <openglwidget.h>
#include <QMainWindow>
#include <QPushButton>
#include <QGroupBox>
#include <QComboBox>
#include <QLabel>
#include <QTreeWidget>
#include <QTimer>
#include <QDateTime>
#include <profile.h>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLineEdit>
#include <database.h>
#include "cdata.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif

typedef enum
{
    START,
    END,
    NOTDEFINE
}clientState;


class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent = nullptr);
    ~MainWindow();

private:
    QPushButton* bhistory;      //!历史
    QPushButton* bfile;         //!文件
    QPushButton* bleft_rotate;  //!左旋
    QPushButton* bright_rotate; //!右旋
    QPushButton* boverlook;     //!俯视
    QPushButton* bsidelook;     //!侧视
    QPushButton* bspride;       //!网格图
    QPushButton* bpatch;        //!填充
    QPushButton* bcloud;        //!点云
    QPushButton* bcolor;        //!颜色

    QGroupBox* control;         //!控制
    QLabel* site;               //!场地
    QPushButton* start;         //!开始扫描
    QTreeWidget* coal;          //!煤堆树

    QGroupBox* deit;
    QLabel* y0;
    QLabel* stepy;
    QLineEdit* edity0;
    QLineEdit* editstepy;

    QGroupBox* section;         //!切面

    //!自定义控件
    Profile* profile;           //!qt二维绘图控件

    QLabel* timelabel;          //! 状态栏时间标签
    QLabel* currentstatus;      //! 当前状态标签
    QTimer* timer;              //! 更新状态栏时间的计时器
    QTimer* pollData;           //! 轮询数据库计数器

    GLWidget* openglwidget;     //! OpenGL控件
    CData* cdata;               //! 数据类
    bool m_bIsConnected;        //! 数据库是否连接
//    QSqlDatabase db;            //! 数据库
    clientState state;
    DataBase* database;         //! 轮询数据库实时3D数据
    QThread pullDataBase;       //! 读取数据库数据
    QThread calcPointAttr;      //! 计算顶点属性
signals:
    void pullonce(QString sql); //! 抓取一次数据库数据
    void StartStopScanner(QString sql, QString select = "");
    void reconnectDB();         //! 重连数据库

private:
    void createButtons();       //! 创建按钮
    void layout();
    void createStatusBar();     //! 创建状态栏

private:
     void  SignalSlots();

public slots:
    void on_Start_click();
    void updateTime();
    void setPatch();
    void setLines();
    void setPointCloud();
    void pollingDataBase();//!轮询数据库
    void initY();//!初始化Y0
    void initStepY();//!初始化stepY
    void setStartText(int); //! 设置开始按钮字符
};

#endif // MAINWINDOW_H
