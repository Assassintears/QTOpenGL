#include "mainwindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QString>
#include <QStatusBar>
#include <QMetaType>


MainWindow::MainWindow(QMainWindow *parent)
    : QMainWindow(parent)
{
   QIcon* icon = new QIcon(":/Resources/app.ico");
   if (!icon->isNull())
   {
       this->setWindowIcon(*icon);
   }

    //! 注册从元数据
    qRegisterMetaType<QVector<QVector<float> > > ("origData");
    qRegisterMetaType<QVector<QVector3D> > ("Points");
    qRegisterMetaType<QVector<unsigned int> > ("index");
    qRegisterMetaType<QVector<QVector<int> > > ("volum");

    database = new DataBase(QString("connect"));
    database->moveToThread(&pullDataBase);
    pullDataBase.start();

    cdata = new CData;
    cdata->moveToThread(&calcPointAttr);
    calcPointAttr.start();

//    m_isCoalSelected.insert(1, false);//! 一号煤场1号天车
//    m_isCoalSelected.insert(1, false);//! 一号煤场2号天车
//    m_isCoalSelected.insert(2, false);//! 二号煤场1号天车
//    m_isCoalSelected.insert(2, false);//! 二号煤场1号天车
   createButtons();
   layout();
   createStatusBar();
   SignalSlots();
}

MainWindow::~MainWindow()
{
    pullDataBase.quit();
    calcPointAttr.quit();
    pullDataBase.wait();
    calcPointAttr.wait();
}

void MainWindow::createButtons()
{
    openglwidget = new GLWidget;
    profile = new Profile;
    bhistory = new QPushButton(this);
    bfile = new QPushButton(this);
    bleft_rotate = new QPushButton(this);
    bright_rotate = new QPushButton(this);
    boverlook = new QPushButton(this);
    bsidelook = new QPushButton(this);
    bspride = new QPushButton(this);
    bpatch = new QPushButton(this);
    bcloud = new QPushButton(this);
    bcolor = new QPushButton(this);

    bhistory->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bfile->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bleft_rotate->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bright_rotate->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bsidelook->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bspride->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bpatch->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bcloud->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    bcolor->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    boverlook->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    //! 设置按钮icon
    bhistory->setStyleSheet("border-image:url(:/Resources/nowtime.png)");
    bfile->setStyleSheet("border-image:url(:/Resources/connect.png)");
    bleft_rotate->setStyleSheet("border-image:url(:/Resources/left_turn.png)");
    bright_rotate->setStyleSheet("border-image:url(:/Resources/right_turn.png)");
    boverlook->setStyleSheet("border-image:url(:/Resources/down_look.png)");
    bsidelook->setStyleSheet("border-image:url(:/Resources/foward_look.png)");
    bspride->setStyleSheet("border-image:url(:/Resources/view_grid.png)");
    bpatch->setStyleSheet("border-image:url(:/Resources/view_padd.png)");
    bcloud->setStyleSheet("border-image:url(:/Resources/view_point.png)");
    bcolor->setStyleSheet("border-image:url(:/Resources/cubage.png)");

    control = new QGroupBox(tr("场地选择"), this);
    start = new QPushButton(tr("Start"));
    coal = new QTreeWidget;
    coal->clear();
    coal->setHeaderHidden(true);
    coal->setColumnCount(1);

    QTreeWidgetItem* item1 = new QTreeWidgetItem;//!第一个节点
    item1->setText(0, tr("一号煤场"));
    QTreeWidgetItem* grandon11 = new QTreeWidgetItem;
    QTreeWidgetItem* grandon12 = new QTreeWidgetItem;
    grandon11->setText(0, tr("一号天车"));
    grandon12->setText(0, tr("二号天车"));
    item1->addChild(grandon11);
    item1->addChild(grandon12);

    QTreeWidgetItem* item2 = new QTreeWidgetItem;//!第一个节点
    item2->setText(0, tr("二号煤场"));
    QTreeWidgetItem* grandon21 = new QTreeWidgetItem;
    QTreeWidgetItem* grandon22 = new QTreeWidgetItem;
    grandon21->setText(0, tr("一号天车"));
    grandon22->setText(0, tr("二号天车"));
    item2->addChild(grandon21);
    item2->addChild(grandon22);
    coal->addTopLevelItem(item1);
    coal->addTopLevelItem(item2);
    item1->setCheckState(0, Qt::Unchecked);
    grandon11->setCheckState(0, Qt::Unchecked);
    grandon12->setCheckState(0, Qt::Unchecked);
    item2->setCheckState(0, Qt::Unchecked);
    grandon21->setCheckState(0, Qt::Unchecked);
    grandon22->setCheckState(0, Qt::Unchecked);

    region = new QGroupBox(tr("区域"));
    region_query = new QPushButton(tr("查询"));
    region_start_label = new QLabel(tr("开始值/m"));
    region_end_label = new QLabel(tr("结束值/m"));
    region_start = new QLineEdit;
    region_start->setValidator(new QRegExpValidator(QRegExp("[0-9\\.]+$")));
    region_end = new QLineEdit;
    region_end->setValidator(new QRegExpValidator(QRegExp("[0-9\\.]+$")));
    volum_label = new QLabel(tr("体积/m^3"));
    region_volum = new QLabel(tr("0"));


    QVBoxLayout* right = new QVBoxLayout;
    QHBoxLayout* contral_main = new QHBoxLayout;


    right->addWidget(coal);
    contral_main->addLayout(right);
    control->setLayout(contral_main);

    deit = new QGroupBox(tr("edit"));
    y0 = new QLabel(tr("Y0/cm"));
    stepy = new QLabel(tr("stepY/cm"));
    edity0 = new QLineEdit;
    editstepy = new QLineEdit;
    edity0->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    editstepy->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    edity0->setValidator(new QRegExpValidator(QRegExp("[0-9\\.]+$")));
    editstepy->setValidator(new QRegExpValidator(QRegExp("[0-9\\.]+$")));
    QVBoxLayout* left_edit = new QVBoxLayout;
    QVBoxLayout* right_edit = new QVBoxLayout;
    QHBoxLayout* edit_lay = new QHBoxLayout;
    left_edit->addWidget(y0);
    left_edit->addStretch();
    left_edit->addWidget(stepy);
    right_edit->addWidget(edity0);
    right_edit->addStretch();
    right_edit->addWidget(editstepy);
    edit_lay->addLayout(left_edit);
    edit_lay->addLayout(right_edit);
    deit->setLayout(edit_lay);


    section = new QGroupBox(tr("切面"), this);
    QHBoxLayout* section_lay = new QHBoxLayout;
    section_lay->addWidget(profile);
    section->setLayout(section_lay);

    timelabel = new QLabel;
    currentstatus = new QLabel;
    timer = new QTimer;
    timer->start(1000);//!一秒更新一次时间
    pollData = new QTimer;
    pollData->start(1000);//!100ms轮询一次数据库
    state = NOTDEFINE;
    currentstatus->setText(tr("初始化"));
}

void MainWindow::layout()
{
    QVBoxLayout* left = new QVBoxLayout;
    QVBoxLayout* right = new QVBoxLayout;
    QHBoxLayout* mainlay = new QHBoxLayout;
    QGridLayout* grid = new QGridLayout;
    grid->addWidget(bhistory, 0, 0);
    grid->addWidget(new QLabel(tr("历史"), this), 1, 0);
    grid->addWidget(bfile, 0, 1);
    grid->addWidget(new QLabel(tr("文件"), this), 1, 1);
    grid->addWidget(bleft_rotate, 0, 2);
    grid->addWidget(new QLabel(tr("左旋"), this), 1, 2);
    grid->addWidget(bright_rotate, 0, 3);
    grid->addWidget(new QLabel(tr("右旋"), this), 1, 3);
    grid->addWidget(boverlook, 0, 4);
    grid->addWidget(new QLabel(tr("俯视"), this), 1, 4);
    grid->addWidget(bsidelook, 0, 5);
    grid->addWidget(new QLabel(tr("侧视"), this), 1, 5);
    grid->addWidget(bspride, 0, 6);
    grid->addWidget(new QLabel(tr("网格"), this), 1, 6);
    grid->addWidget(bpatch, 0, 7);
    grid->addWidget(new QLabel(tr("填充"), this), 1, 7);
    grid->addWidget(bcloud, 0, 8);
    grid->addWidget(new QLabel(tr("点云"), this), 1, 8);
    grid->addWidget(bcolor, 0, 9);
    grid->addWidget(new QLabel(tr("颜色"), this), 1, 9);

    control->setFixedSize(QSize(180, 100));
    deit->setFixedSize(QSize(180, 100));

    QVBoxLayout* region_vl = new QVBoxLayout;

    QHBoxLayout* vllu = new QHBoxLayout;
    vllu->addWidget(region_start_label);
    vllu->addWidget(region_start);
    vllu->addWidget(region_end_label);
    vllu->addWidget(region_end);
    QHBoxLayout* vlld = new QHBoxLayout;
    vlld->addWidget(volum_label);
    vlld->addWidget(region_volum);
    region_vl->addLayout(vllu);
    region_vl->addLayout(vlld);
    region_vl->addWidget(region_query, 0, Qt::AlignRight);
    region->setLayout(region_vl);

    QHBoxLayout* middle = new QHBoxLayout;
    left->addLayout(grid);
    left->addStretch();
    middle->setAlignment(left, Qt::AlignTop);
    left->addLayout(middle);
    left->addWidget(openglwidget);

    right->addWidget(start, 0,  Qt::AlignLeft);
    start->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    right->addWidget(control, 0, Qt::AlignRight);
    right->addStretch();
    right->addWidget(deit, 0, Qt::AlignRight);
    right->addStretch();
    right->addWidget(region);
    right->addWidget(section);
    mainlay->addLayout(left);
    mainlay->addLayout(right);

    QWidget* cent = new QWidget;
    cent->setLayout(mainlay);
    this->setCentralWidget(cent);
}

void MainWindow::SignalSlots()
{
    connect(&pullDataBase, &QThread::finished, database, &QObject::deleteLater);
    connect(&calcPointAttr, &QThread::finished, cdata, &QObject::deleteLater);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    connect(bpatch, &QPushButton::clicked, this, &MainWindow::setPatch);        //! 设置填充
    connect(bcloud, &QPushButton::clicked, this, &MainWindow::setPointCloud);   //! 设置点云
    connect(bspride, &QPushButton::clicked, this, &MainWindow::setLines);       //! 设置线框
    connect(pollData, &QTimer::timeout, this, &MainWindow::pollingDataBase);    //! 读取数据库数据
    connect(this, &MainWindow::pullonce,
            database, &DataBase::selectRealDataFromDB);                         //! 线程读取数据库数据
    connect(this, &MainWindow::reconnectDB, database, &DataBase::reconnectDB);  //! 重连数据库
    connect(database, &DataBase::dataBase, cdata, &CData::calcPoint);           //! 发送信号给数据类，表示有数据需要处理
    connect(cdata, &CData::hasData, openglwidget, &GLWidget::updateData);       //! 数据类处理完发送给OpenGL渲染
    connect(edity0, &QLineEdit::editingFinished, this, &MainWindow::initY);
    connect(editstepy, &QLineEdit::editingFinished, this, &MainWindow::initStepY);
    connect(start, &QPushButton::clicked, this, &MainWindow::on_Start_click);
    connect(this, &MainWindow::StartStopScanner, database, &DataBase::StartStopScanner);
    connect(database, &DataBase::State, this, &MainWindow::setStartText);
    connect(bright_rotate, &QPushButton::clicked, openglwidget, &GLWidget::rotateRight);
    connect(bleft_rotate, &QPushButton::clicked, openglwidget, &GLWidget::rotateLeft);

    connect(coal, &QTreeWidget::itemClicked, this, &MainWindow::on_click_coal);

    //! 计算体积信号
    connect(region_query, &QPushButton::clicked, this, &MainWindow::on_query);
    connect(this, &MainWindow::calVolum, database, &DataBase::selectAll);
    connect(database, &DataBase::calcVolum, cdata, &CData::calcVolum);
    connect(cdata, &CData::Volum, this, &MainWindow::VolumRes);
//    connect(database, &DataBase::Volum, this, &MainWindow::VolumRes);
}

void MainWindow::pollingDataBase()
{
    if (START == state)
    {
        //! 这里判断读取哪个表格数据
        QString sql = "select * from 一号煤场";
        emit pullonce(sql);
    }
}

void MainWindow::createStatusBar()
{
    statusBar()->addWidget(new QLabel(tr("当前状态:")));
    statusBar()->addWidget(currentstatus);
    statusBar()->addWidget(new QLabel(tr("当前时间:")));
    statusBar()->addWidget(timelabel);
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    timelabel->setText(currentTime.toString("yyyy-MM-dd hh:mm:ss"));
    if (database->m_isBreak)
    {
        emit reconnectDB();
    }

}

void MainWindow::setPatch()
{
    openglwidget->setRenderMode(Patch);
    openglwidget->update();
}

void MainWindow::setPointCloud()
{
    openglwidget->setRenderMode(PointCloud);
    openglwidget->update();

}

void MainWindow::setLines()
{
    this->openglwidget->setRenderMode(Lines);
    openglwidget->update();
}

void MainWindow::initY()
{
    QString t = edity0->text();
    cdata->y0 = t.toFloat();
}

void MainWindow::initStepY()
{
    QString t = editstepy->text();
    cdata->stepy = t.toFloat();
}

void MainWindow::on_Start_click()
{
    QString tx = start->text();
    //! 判断哪些被选中

    QString start = "UPDATE IPC SET State = 1 WHERE SiteID = 1";
    QString stop = "UPDATE IPC SET State = 0 WHERE SiteID = 1";
    QString select = "SELECT State FROM IPC WHERE SiteID = 1"; //! 开始测量之前，先查看当前煤场是否已经在测量
    if ("Start" == tx)
    {
        emit StartStopScanner(start, select);

    }
    else if ("End" == tx)
    {
        emit StartStopScanner(stop);
        database->m_stop = true;
    }
    else {
        ;
    }
}

void MainWindow::setStartText(int state)
{
    QString tx = start->text();
    if (0 == state)     //! 正常返回
    {
        if ("Start" == tx)
        {
            start->setText("End");
            this->state = START;
            database->m_stop = false;
            currentstatus->setText(tr("正在扫描"));
        }
        else
        {
            start->setText("Start");
            this->state = END;
            currentstatus->setText(tr("结束扫描"));
        }
    }
    else if (2 == state)        //! 数据库断开
    {
        if ("Start" == tx)
        {
            this->state = END;
            database->m_stop = true;    //! 停止采集
        }
        else
        {
            this->state = START;
            database->m_stop = false;   //! 继续采集
        }
        currentstatus->setText(tr("重连数据库"));
        database->m_isBreak = true;
    }
    else if (3 == state)        //! 指令无效
    {
        if ("Start" == tx)
        {
            this->state = START;
            database->m_stop = false;
        }
        else
        {
            this->state = END;
            database->m_stop = true;
        }
        currentstatus->setText(tr("指令无效"));
    }
    else if (4 == state)        //! 重连成功
    {
        database->m_isBreak = false;
        currentstatus->setText(tr("重连成功"));
        if ("Start" == tx)
        {
            this->state = END;
            database->m_stop = true;    //! 停止采集
            currentstatus->setText(tr("停止扫描"));
        }
        else
        {
            this->state = START;
            database->m_stop = false;   //! 继续采集
            currentstatus->setText(tr("继续扫描"));
        }
    }
}

void MainWindow::on_query()
{
    QString s = region_start->text();
    QString e = region_end->text();
    if (s.isEmpty())
    {
        qDebug() << "必须输入起始值\n";
        return;
    }
    if (e.isEmpty())
    {
        qDebug() << "必须输入结束值\n";
        return;
    }
    int start = static_cast<int>(s.toFloat() * 1000.0f);
    int end = static_cast<int>(e.toFloat() * 1000.0f);
    if (start >= end)
    {
        qDebug() << "起始值必须小于结束值\n";
        return;
    }
    if (START == this->state)
    {
        qDebug() << "当前正在扫描，请等待扫描结束后查询\n";
        return;
    }

    emit calVolum(start, end);
    return;

}

void MainWindow::VolumRes(float volum)
{
    region_volum->setText(QString::number(volum));
}

void MainWindow::on_click_coal(QTreeWidgetItem *item, int column)
{
    if (item->parent())//! 当前选中子控件
    {
        QTreeWidgetItem* parent = item->parent();
        const int childCount = parent->childCount();
        int selectCount = 0;//! 被选中的数量
        for (int i = 0; i < childCount; ++i)
        {
            if (Qt::Checked == parent->child(i)->checkState(column))
            {
                selectCount++;
            }
        }

        Qt::CheckState currentState = item->checkState(column);
        if (Qt::Unchecked == currentState) //! 孩子被取消
        {
            item->setCheckState(column, Qt::Unchecked);
            if (0 == selectCount)//! 所有都被取消了
            {
                parent->setCheckState(column, Qt::Unchecked);
            }
        }
        else
        {
            item->setCheckState(column, Qt::Checked);
            if (childCount == selectCount)//! 所有都被选中了
            {
                parent->setCheckState(column, Qt::Checked);
            }
        }
    }
    else
    {
        if (Qt::Unchecked == item->checkState(column))
        {
            item->setCheckState(column, Qt::Unchecked);
            for (int i = 0; i < item->childCount(); ++i)
            {
                item->child(i)->setCheckState(column, Qt::Unchecked);
            }
        }
        else
        {
            item->setCheckState(column, Qt::Checked);
            for (int i = 0; i < item->childCount(); ++i)
            {
                item->child(i)->setCheckState(column, Qt::Checked);
            }
        }
    }
}
