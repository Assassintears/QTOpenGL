#include "mainwindow.h"
#include <QHBoxLayout>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QString>
#include <QStatusBar>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      cdata{}
{
    //!开始连接数据库
    db = QSqlDatabase::addDatabase("QODBC");
    db.setHostName("192.168.2.136");
    db.setDatabaseName("DB");//!如果是QODBC驱动，此处应该是数据源名称
    db.setUserName("sa");
    db.setPassword("sa");
    db.setPort(1433);
   createButtons();
   layout();
   createStatusBar();
   SignalSlots();

}

MainWindow::~MainWindow()
{

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

    profile = new Profile;

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

    control = new QGroupBox(tr("控制："), this);
    site = new QLabel(tr("场地选择"));
    start = new QPushButton(tr("Start"));
    coal = new QTreeWidget;
    coal->clear();
    coal->setHeaderHidden(true);
    coal->setColumnCount(1);

    QTreeWidgetItem* item1 = new QTreeWidgetItem;//!第一个节点
    item1->setText(0, tr("A"));
    QTreeWidgetItem* grandon11 = new QTreeWidgetItem;
    QTreeWidgetItem* grandon12 = new QTreeWidgetItem;
    grandon11->setText(0, tr("扫描仪A"));
    grandon12->setText(0, tr("扫描仪B"));
    item1->addChild(grandon11);
    item1->addChild(grandon12);

    QTreeWidgetItem* item2 = new QTreeWidgetItem;//!第一个节点
    item2->setText(0, tr("B"));
    QTreeWidgetItem* grandon21 = new QTreeWidgetItem;
    QTreeWidgetItem* grandon22 = new QTreeWidgetItem;
    grandon21->setText(0, tr("扫描仪A"));
    grandon22->setText(0, tr("扫描仪B"));
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

    QVBoxLayout* left = new QVBoxLayout;
    QVBoxLayout* right = new QVBoxLayout;
    QHBoxLayout* contral_main = new QHBoxLayout;
    left->addWidget(start);
    left->addStretch();

    right->addStretch();
    right->addWidget(site, 0, Qt::AlignRight);
    right->addWidget(coal);
    contral_main->addLayout(left);
    contral_main->addLayout(right);
    control->setLayout(contral_main);

    deit = new QGroupBox(tr("edit"));
    y0 = new QLabel(tr("Y0"));
    stepy = new QLabel(tr("stepY"));
    edity0 = new QLineEdit;
    editstepy = new QLineEdit;
    edity0->setValidator(new QRegExpValidator(QRegExp("[0-9]+\\.[0-9]+$")));
    editstepy->setValidator(new QRegExpValidator(QRegExp("[0-9]+\\.[0-9]+$")));
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

    QHBoxLayout* middle = new QHBoxLayout;
    left->addLayout(grid);
    left->addStretch();
    middle->setAlignment(left, Qt::AlignTop);
    left->addLayout(middle);
    left->addWidget(openglwidget);
//    left->addStretch();
    right->addWidget(control);
    right->addStretch();
    right->addWidget(deit);
    right->addStretch();
    right->addWidget(section);
    mainlay->addLayout(left);
    mainlay->addLayout(right);

    QWidget* cent = new QWidget;
    cent->setLayout(mainlay);
    this->setCentralWidget(cent);
}

void MainWindow::SignalSlots()
{
    connect(timer, &QTimer::timeout, this, &MainWindow::updateTime);
    connect(bpatch, &QPushButton::clicked, this, &MainWindow::setPatch);
    connect(bcloud, &QPushButton::clicked, this, &MainWindow::setPointCloud);
    connect(bspride, &QPushButton::clicked, this, &MainWindow::setLines);
    connect(pollData, &QTimer::timeout, this, &MainWindow::pollingDataBase);
    connect(this, &MainWindow::dataBase, &cdata, &CData::receiveDataFromDB);
    connect(&cdata, &CData::hasData, openglwidget, &GLWidget::updateData);
    connect(edity0, &QLineEdit::editingFinished, this, &MainWindow::initY);
    connect(editstepy, &QLineEdit::editingFinished, this, &MainWindow::initStepY);
    connect(start, &QPushButton::clicked, this, &MainWindow::on_Start_click);
}

bool MainWindow::his_pushed()
{
    bool res = true;

    if (!db.isOpen())
    {
        if (!db.open())
        {
            qDebug() << "It's filed to connect DB: " << db.lastError().text() << "\n";
            return !res;
        }
    }

    //!写入指令
    QSqlQuery query;
    QString sql = "UPDATE t_Child_operation SET isstate = 1 WHERE operationID = '1'";
    query.exec(sql);
    if (!query.isActive())
    {
        qDebug() << "an error occured when start client\n";
        qDebug() << db.lastError().text() << "\n";
    }
    else {
        qDebug() << "Start Client\n";
        query.clear();
        db.close();
        state = START;
    }
    return res;
}

bool MainWindow::stopIPC()
{
    bool res = true;
    if (!db.isOpen())
    {
        if (!db.open())
        {
            qDebug() << "Can not open DB\n";
            return !res;
        }
    }
    QSqlQuery query;
    QString sql = "UPDATE t_Child_operation SET isstate = 0 WHERE operationID = '1'";
    query.exec(sql);
    if (!query.isActive())
    {
        qDebug() << "an error occured when stop client\n";
        qDebug() << db.lastError().text() << "\n";
    }
    else {
        qDebug() << "Stop Client\n";
        query.clear();
        db.close();
        state = END;
    }
    return res;
}

void MainWindow::pollingDataBase()
{
    if (START == state)
    {
        if (!db.isOpen())
        {
            if (!db.open())
            {
                qDebug() << "打开数据库失败\n";
                return;
            }
        }
        QSqlQuery query;
        QString sql = "select * from T_3D_record";
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
                data.clear();
            }
            query.clear();
        }
        db.close();
    }
    else {
        qDebug() << "Not Start" << QTime::currentTime() << "\n";
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
//    this->openglwidget->paintOpenGL();
}

void MainWindow::setPatch()
{
    this->openglwidget->setRenderMode(Patch);
}

void MainWindow::setPointCloud()
{
    this->openglwidget->setRenderMode(PointCloud);
}

void MainWindow::setLines()
{
    this->openglwidget->setRenderMode(Lines);
}

void MainWindow::initY()
{
    QString t = edity0->text();
    cdata.y0 = t.toFloat();
}

void MainWindow::initStepY()
{
    QString t = editstepy->text();
    cdata.stepy = t.toFloat();
}

void MainWindow::on_Start_click()
{
    QString tx = start->text();
    if ("Start" == tx)
    {
        if (his_pushed())
        {
            start->setText("End");
        }
    }
    else if ("End" == tx)
    {
        if (stopIPC())
        {
            start->setText("Start");
        }
    }
}
