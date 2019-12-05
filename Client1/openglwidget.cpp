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


GLWidget::GLWidget(QWidget *parent)
    : QOpenGLWidget(parent),
      QOpenGLFunctions_4_5_Core(),
      m_vbo(QOpenGLBuffer::VertexBuffer),
      charvbo(QOpenGLBuffer::VertexBuffer)
{
    setFocusPolicy(Qt::StrongFocus);
    m_camera.setToIdentity();
    m_camera.lookAt(QVector3D(100, 20, 100), QVector3D(0, 0, 0), QVector3D(0, 1, 0));
    lightColor = QVector3D(1.0f, 1.0f, 1.0f);
    objectColor = QVector3D(1.0f, 0.5f, 0.31f);
    count = 0;
    mode = PointCloud;//!默认点云渲染  

    loadFonts();//!加载字符纹理
}

GLWidget::~GLWidget()
{
//    cleanup();
    FT_Done_Face(face);
    FT_Done_FreeType(ft);
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
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/Dvs.shader");
    m_program->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/Dfs.shader");
    if (!m_program->link())
        close();
    if (!m_program->bind())
        close();
     m_program->release();


    coordPro = new QOpenGLShaderProgram;
    coordPro->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/coordvs.vert");
    coordPro->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/coordfs.frag");
    if (!coordPro->link())
        close();

    if (!coordPro->bind())
        close();
    coordPro->release();

    charPro = new QOpenGLShaderProgram;
    charPro->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/textvs.vert");
    charPro->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/textfs.frag");
    if (!charPro->link())
        close();
    if (!charPro->bind())
        close();
     charPro->release();

}

void GLWidget::updateData(QVector<QVector3D> local, QVector<unsigned int> ii)
{
    if (local.empty())
    {
        qDebug() << "OpenGL未接收到数据\n";
        return;
    }
    m_ok = true;
    m_vbo.destroy();//释放缓冲区数据
    m_vbo.create();
    m_vbo.bind();
    count = 0;
    QVector<QVector3D> tmp;
    for (int i = 0; i < ii.size(); ++i)
    {
        tmp.push_back(local[ii[i]]);
    }
    m_vbo.allocate(tmp.size() * 3 * sizeof(float));
    m_vbo.write(0, tmp.data(), tmp.size() * 3 * sizeof(float));
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
    glEnable(GL_DEPTH_TEST);//允许深度测试
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_LINE_SMOOTH);

    initShaders();

    //!创建uniform缓冲区
    glGenBuffers(1, &uboExampleBlock);
    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    glBufferData(GL_UNIFORM_BUFFER, 2 * 16 * sizeof(float), nullptr, GL_STATIC_DRAW);//分配uniform块空间
    uboBindPoint = 1;
    glBindBufferRange(GL_UNIFORM_BUFFER, uboBindPoint, uboExampleBlock, 0, 2 * 16 * sizeof(float));//!缓冲区绑定点
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    //!着色器绑定uniform块
    m_program->bind();
    unsigned int mprogramuniformID = glGetUniformBlockIndex(m_program->programId(), "Matrices");
    glUniformBlockBinding(m_program->programId(), mprogramuniformID, uboBindPoint);
    m_program->release();

    coordPro->bind();
    unsigned int coordProuniformID = glGetUniformBlockIndex(coordPro->programId(), "Matrices");
    glUniformBlockBinding(coordPro->programId(), coordProuniformID, uboBindPoint);
    coordPro->release();

    m_vao.create();
    m_vbo.create();
    m_colorvbo.create();

    //!坐标轴数据
    QVector<QVector3D> out;
    genCoordData(QVector3D(400, 100, 100), QVector3D(20, 20, 20), out);
    indexSize = out.size();
    VBO.create();
    VBO.bind();
    VBO.allocate(out.data(), out.size() * 3 * sizeof(float));//!必须先绑定才能赋值
    VAO.create();

    //!纹理buffer
    charvao.create();
    charvbo.create();
    charvbo.bind();
    charvbo.setUsagePattern(QOpenGLBuffer::DynamicDraw);
}


void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //!画坐标轴
    coordPro->bind();
    int modelLoc = coordPro->uniformLocation("model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, m_world.data());
    VBO.bind();
    VAO.bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glLineWidth(5.0f);
    glDrawArrays(GL_LINES, 0, indexSize);
//    glPointSize(2.0f);
//    glDrawArrays(GL_POINTS, 0, indexSize);
    VBO.release();
    VAO.release();
    coordPro->release();

    //!画字符

    RenderText("ABC", 100, 100, 1, QVector3D(1.0f, 0.0f, 0.0f));

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
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//!这种方式有一点缺陷--会改变所有三角形图元的渲染模式，不管三角形在此之前已被渲染还是在此之后渲染
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
    else if (PointCloud == mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
    else if(Lines == mode)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawArrays(GL_TRIANGLES, 0, count);
    }
    else
    {}

    m_vbo.release();
    m_vao.release();
    m_program->release();

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
//    m_proj.perspective(45.0f, GLfloat(w) / h, 0.1f, 1000.0f);
    m_proj.ortho(-w / 2, w / 2, -h / 2, h / 2, 1.0f, 1000.0f);
    glBindBuffer(GL_UNIFORM_BUFFER, uboExampleBlock);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, 16 * sizeof(float), m_proj.data());
    glBufferSubData(GL_UNIFORM_BUFFER, 16 * sizeof(float), 16 * sizeof(float), m_camera.data());
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
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


void GLWidget::keyPressEvent(QKeyEvent* key)
{
}

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
    int x = max.x();
    int y = max.y();
    int z = max.z();

    int stepx = step.x();
    int stepy = step.y();
    int stepz = step.z();

    if (0 == x | 0 == y | 0 == z |
            0 == stepx | 0 == stepy | 0 == stepz)
    {
        return false;
    }

    int numX = x / stepx;
    int numY = y / stepy;
    int numZ = z / stepz;
    //!生成刻度
    //! x方向刻度
    for (int i = 0; i < numX; ++i)
    {
        out.push_back(QVector3D((i + 1) * stepx, 0.0f, 0.0f));//!刻度起点
        out.push_back(QVector3D((i + 1) * stepx, 0.0f, -2.0f));//!刻度终点
    }
    //! y方向刻度
    for (int i = 0; i < numY; ++i)
    {
        out.push_back(QVector3D(0.0f, (i + 1) * stepy, 0.0f));//!刻度起点
        out.push_back(QVector3D(-2.0f, (i + 1) * stepy, -2.0f));//!刻度终点
    }
    //! z方向刻度
    for (int i = 0; i < numZ; ++i)
    {
        out.push_back(QVector3D(0.0f, 0.0f, (i + 1) * stepz));//!刻度起点
        out.push_back(QVector3D(0.0f,-2.0f, (i + 1) * stepz));//!刻度终点
    }
    //! xyz轴
    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(1.0f * x, 0.0f, 0.0f));

    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(0.0f, 1.0f * y, 0.0f));

    out.push_back(QVector3D(0.0f, 0.0f, 0.0f));
    out.push_back(QVector3D(0.0f, 0.0f, 1.0f * z));
}

bool GLWidget::loadFonts()
 {
     //!加载字体
     if (FT_Init_FreeType(&ft))
         return false;
     if (FT_New_Face(ft, "arial.ttf", 0, &face))
         return false;
     FT_Set_Pixel_Sizes(face, 0, 48);
     glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //禁用字节对齐限制
     for (GLubyte c = 0; c < 128; c++)
     {
         // 加载字符的字形
         if (FT_Load_Char(face, c, FT_LOAD_RENDER))
         {
             continue;
         }
         // 生成纹理
         GLuint texture;
         glGenTextures(1, &texture);
         glBindTexture(GL_TEXTURE_2D, texture);
         glTexImage2D(
             GL_TEXTURE_2D,
             0,
             GL_RED,
             face->glyph->bitmap.width,
             face->glyph->bitmap.rows,
             0,
             GL_RED,
             GL_UNSIGNED_BYTE,
             face->glyph->bitmap.buffer
         );
         // 设置纹理选项
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         // 储存字符供之后使用
         Character character = {
             texture,
             {static_cast<int>(face->glyph->bitmap.width), static_cast<int>(face->glyph->bitmap.rows)},
             {static_cast<int>(face->glyph->bitmap_left), static_cast<int>(face->glyph->bitmap_top)},
             static_cast<GLuint>(face->glyph->advance.x)
         };
         m_characters.insert(c, character);
     }
 }

void GLWidget::RenderText(std::string text, GLfloat x, GLfloat y,
                            GLfloat scale, QVector3D color)
{
        charPro->bind();
        glUniform3f(charPro->uniformLocation("textColor"), color.x(), color.y(), color.z());
        glActiveTexture(GL_TEXTURE0);
        // 遍历文本中所有的字符
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++)
        {
            Character ch = m_characters[*c];

            GLfloat xpos = x + ch.Bearing.width() * scale;
            GLfloat ypos = y - (ch.Size.height() - ch.Bearing.height()) * scale;

            GLfloat w = ch.Size.width() * scale;
            GLfloat h = ch.Size.height() * scale;
            // 对每个字符更新VBO
            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos,     ypos,       0.0, 1.0 },
                { xpos + w, ypos,       1.0, 1.0 },

                { xpos,     ypos + h,   0.0, 0.0 },
                { xpos + w, ypos,       1.0, 1.0 },
                { xpos + w, ypos + h,   1.0, 0.0 }
            };
            // 在四边形上绘制字形纹理
            glBindTexture(GL_TEXTURE_2D, ch.TextureID);
            // 更新VBO内存的内容
            charvbo.bind();
            charvbo.allocate(sizeof(vertices));
            charvbo.write(0, vertices, sizeof(vertices));

            charvao.bind();
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
            glEnableVertexAttribArray(0);
            // 绘制四边形
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // 更新位置到下一个字形的原点，注意单位是1/64像素
            x += (ch.Advance >> 6) * scale; // 位偏移6个单位来获取单位为像素的值 (2^6 = 64)
            charvbo.release();
            charvao.release();
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
}
