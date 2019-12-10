#include <openglwidget.h>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>
#include <QDateTime>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QIODevice>
#include <QVector>
#include <QVector3D>
#include <QDebug>
#include <string>
#include <QTransform>
#include <QQuaternion>
#include <QPainter>
#include <QPen>
#include <QFont>


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      QOpenGLFunctions_4_5_Core(),
      m_vbo(QOpenGLBuffer::VertexBuffer)
{
    QSurfaceFormat format;
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setVersion(4, 5);
    format.setProfile(QSurfaceFormat::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);

    setFocusPolicy(Qt::StrongFocus);
    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(10.0f, 20.0f, 10.0f), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    lightColor = QVector3D(1.0f, 1.0f, 1.0f);
    objectColor = QVector3D(1.0f, 0.5f, 0.31f);
    count = 0;
    mode = PointCloud;//!默认点云渲染

    nRange = 800;
}

GLWidget::~GLWidget()
{
//    cleanup();
//    FT_Done_Face(face);
//    FT_Done_FreeType(ft);
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(1280, 760);
}

void GLWidget::initShaders()
{
    //!模型着色器
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/Dvs.shader");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Dfs.shader");
    if (!m_program->link())
        close();
    if (!m_program->bind())
        close();
     m_program->release();

    //!坐标轴着色器
    coordPro = new QOpenGLShaderProgram;
    coordPro->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/coordvs.vert");
    coordPro->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/coordfs.frag");
    if (!coordPro->link())
        close();

    if (!coordPro->bind())
        close();
    coordPro->release();
}

void GLWidget::updateData(QVector<QVector3D> local, QVector<unsigned int> ii)
{
    if (local.empty())
    {
        qDebug() << "OpenGL未接收到数据\n";
        return;
    }
//    m_ok = true;
    m_vbo.destroy();//释放缓冲区数据
    m_vbo.create();
    m_vbo.bind();
    count = 0;
    QVector<QVector3D> tmp;
    for (int i = 0; i < ii.size(); ++i)
    {
        tmp.push_back(local[ii[i]]);
    }
    m_vbo.allocate(tmp.size() * 3 * static_cast<int>(sizeof(float)));
    m_vbo.write(0, tmp.data(), tmp.size() * 3 * static_cast<int>(sizeof(float)));
    count = ii.size();
    update();
}



void GLWidget::cleanup()
{
    if (m_program == nullptr)
        return;
    makeCurrent();
    delete m_program;
    m_program = nullptr;
    doneCurrent();
}

void GLWidget::initializeGL()
{
    initializeOpenGLFunctions();//创建OpenGL上下文环境，初始化OpenGL
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLWidget::cleanup);//最好有这句话
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);


    initShaders();

    //!创建uniform缓冲区
    glGenBuffers(1, &uboExampleBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    glBufferData(GL_UNIFORM_BUFFER, 2 * 16 * sizeof(float), nullptr, GL_STATIC_DRAW);//分配uniform块空间
    //!ubo缓冲区绑定点
    uboBindPoint = 1;
    glBindBufferRange(GL_UNIFORM_BUFFER, uboBindPoint, uboExampleBlock, 0, 2 * 16 * sizeof(float));//!缓冲区绑定点
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //!着色器绑定uniform块
    //! 模型着色器
    m_program->bind();
    unsigned int mprogramuniformID = glGetUniformBlockIndex(m_program->programId(), "Matrices");
    glUniformBlockBinding(m_program->programId(), mprogramuniformID, uboBindPoint);
    m_program->release();
    //!坐标轴着色器
    coordPro->bind();
    unsigned int coordProuniformID = glGetUniformBlockIndex(coordPro->programId(), "Matrices");
    glUniformBlockBinding(coordPro->programId(), coordProuniformID, uboBindPoint);
    coordPro->release();


    m_vao.create();
    m_vbo.create();
    m_colorvbo.create();

    //!坐标轴数据
    QVector<QVector3D> out;
    genCoordData(QVector3D(400, 100, 100), QVector3D(50, 30, 30), out);
    indexSize = out.size();
    VBO.create();
    VBO.bind();
    VBO.allocate(out.data(), out.size() * 3 * static_cast<int>(sizeof(float)));//!必须先绑定才能赋值
    VAO.create();
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);//允许深度测试
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //!OpenGL Draw
    QPainter painter(this);
    painter.beginNativePainting();

    //!render model
    coordPro->bind();
    int modelLoc = coordPro->uniformLocation("model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_world.data());
    VBO.bind();
    VAO.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glLineWidth(2.0f);
    glDrawArrays(GL_LINES, 0, indexSize);
    VBO.release();
    VAO.release();
    coordPro->release();

    //!画煤场
    m_program->bind();
    modelLoc = m_program->uniformLocation("model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_world.data());
    m_vbo.bind();
    m_vbo.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    if (Patch == mode)
    {
//        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//!这种方式有一点缺陷--会改变所有三角形图元的渲染模式，不管三角形在此之前已被渲染还是在此之后渲染
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
    else if (PointCloud == mode)
    {
//        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawArrays(GL_POINTS, 0, count);
    }
    else if(Lines == mode)
    {
//        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_LINE_STRIP, 0, count);
    }
    else
    {}

    m_vbo.release();
    m_vao.release();
    m_program->release();

    //!Draw Text
    QPen pen;
    pen.setColor(Qt::red);
    painter.setPen(pen);
    for (QHash<QString, QList<QVector3D>>::iterator it = labels.begin(); it != labels.end(); ++it)
    {
        QList<QVector3D> points = it.value();
        for (int i = 0; i < points.size(); ++i)
        {
            QVector3D d3 = points.at(i);
            QVector4D ndc = m_proj * m_camera * m_world * QVector4D(d3, 1.0f);
            //!计算像素
            int x = static_cast<int>((this->width() * 0.5f - 0.5f) * (ndc[0] + ndc[3]));
            int y = static_cast<int>((this->height() * 0.5f - 0.5f) * (ndc[3] - ndc[1]));
            painter.drawText(x, y, it.key());
        }
    }
    painter.endNativePainting();

}

void GLWidget::resizeGL(int w, int h)
{
    rotationM.setToIdentity();
    translateM.setToIdentity();
    scaleM.setToIdentity();
    //!w, h是像素值
    m_world.setToIdentity();
    //!转换为左手坐标系：
    m_world.translate(-80, -50, 0.f);//!这里要想办法根据窗口大小自适应平移距离
    m_world.rotate(-90, QVector3D(1, 0, 0));//!绕x轴旋转90°
    m_world *= QMatrix4x4(0, 1, 0, 0,
                          1, 0, 0, 0,
                          0, 0, 1, 0,
                          0, 0, 0, 1);

    m_proj.setToIdentity();
    if (w <= h){
        m_proj.ortho(-nRange, nRange, -nRange*h/w, nRange*h/w, -nRange, nRange);
    }
    else{
        m_proj.ortho(-nRange*w/h, nRange*w/h, -nRange, nRange, -nRange, nRange);
    }

    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), m_proj.data());
    glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), m_camera.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    int side = qMin(w, h);
    glViewport((w - side) / 2, (h - side) / 2, side, side);

}

void GLWidget::paintOpenGL()
{
    update();
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
   m_lastPos = event->localPos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    float dx = static_cast<int>(event->localPos().x() - m_lastPos.x());//移动的像素距离
    float dy = static_cast<int>(event->localPos().y() - m_lastPos.y());
    if (event->buttons() & Qt::LeftButton)
    {
        rotationM.setToIdentity();
        QVector3D n = QVector3D(dy, -dx, 0.0).normalized();//!旋转轴
        float acc = sqrt(dx * dx + dy * dy) / 50.0f;//旋转角度
        QVector3D rotationAxis = n.normalized();
        rotationM.rotate(-acc, rotationAxis);

        m_world *= rotationM;
    }
    else if (event->buttons() & Qt::MidButton)
    {
        translateM.setToIdentity();
        translateM.translate(dx * 0.03f, 0.0f, -dy * 0.03f);
        m_world *= translateM;
    }
    update();
}


//void GLWidget::keyPressEvent(QKeyEvent* key)
//{
//}

void GLWidget::wheelEvent(QWheelEvent *event)
{
    scaleM.setToIdentity();
    if (event->delta() > 0)//滚轮远离使用者
        {
            scaleM *= QMatrix4x4(1.5f, 0.0f, 0.0f, 0.f,
                                 0.f, 1.5f, 0.f, 0.f,
                                 0.f, 0.f, 1.5f, 0.f,
                                 0.f, 0.f, 0.f, 1.f);

        }
        else if (event->delta() < 0)//滚轮靠近使用者
        {
            scaleM *= QMatrix4x4(0.8f, 0.0f, 0.0f, 0.0f,
                                 0.0f, 0.8f, 0.0f, 0.0f,
                                 0.0f, 0.0f, 0.8f, 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f);
        }
    m_world *= scaleM;
    update();
}


void GLWidget::setRenderMode(RenderMode mode)
{
    this->mode = mode;
}

bool GLWidget::genCoordData(const QVector3D max, const QVector3D step,
                  QVector<QVector3D>& out)
{
    float x = max.x();
    float y = max.y();
    float z = max.z();

    float stepx = step.x();
    float stepy = step.y();
    float stepz = step.z();

    if ( abs(x - 0.0f) < 1e-3f | abs(y - 0.0f) < 1e-3f | abs(z - 0.0f) < 1e-3f |
            abs(stepx - 0.0f) < 1e-3f | abs(stepy - 0.0f) < 1e-3f | abs(stepz - 0.0f) < 1e-3f)
    {
        return false;
    }

    int numX = static_cast<int>(x / stepx);
    int numY = static_cast<int>(y / stepy);
    int numZ = static_cast<int>(z / stepz);
    //!生成刻度
    //! x方向刻度
    for (int i = 0; i < numX; ++i)
    {
        out.push_back(QVector3D((i + 1) * stepx, 0.0f, 0.0f));//!刻度起点
        out.push_back(QVector3D((i + 1) * stepx, 0.0f, -2.0f));//!刻度终点
        QString label = QString::number(static_cast<int>((i + 1) * stepx));
        labels[label].push_back(QVector3D((i + 1) * stepx, 0.0f, -2.0f));
    }
    //! y方向刻度
    for (int i = 0; i < numY; ++i)
    {
        out.push_back(QVector3D(0.0f, (i + 1) * stepy, 0.0f));//!刻度起点
        out.push_back(QVector3D(-2.0f, (i + 1) * stepy, -2.0f));//!刻度终点
        QString label = QString::number(static_cast<int>((i + 1) * stepy));
        labels[label].push_back(QVector3D(-2.0f, (i + 1) * stepy, -2.0f));
    }
    //! z方向刻度
    for (int i = 0; i < numZ; ++i)
    {
        out.push_back(QVector3D(0.0f, 0.0f, (i + 1) * stepz));//!刻度起点
        out.push_back(QVector3D(0.0f,-2.0f, (i + 1) * stepz));//!刻度终点
        QString label = QString::number(static_cast<int>((i + 1) * stepz));
        labels[label].push_back(QVector3D(0.0f,-2.0f, (i + 1) * stepz));
    }
    //! xyz轴
    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(1.0f * x, 0.0f, 0.0f));

    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(0.0f, 1.0f * y, 0.0f));

    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(0.0f, 0.0f, 1.0f * z));

    //! xyz标签
    QString label("X");
    labels[label].push_back(QVector3D(1.0f * x + 10.0f, 0.0f, 0.0f));
    label = "Y";
    labels[label].push_back(QVector3D(0.0f, 1.0f * y + 10.0f, 0.0f));
    label = "Z";
    labels[label].push_back(QVector3D(0.0f, 0.0f, 1.0f * z + 10.0f));
}
