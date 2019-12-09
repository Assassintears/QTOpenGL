#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenGLFunctions_4_5_Core>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QOpenGLTexture>
#include <QPainter>

//#include <ft2build.h>
//#include FT_FREETYPE_H


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

typedef enum RenderMode
{
    Patch,      //!填充
    PointCloud, //!点云
    Lines,      //!线模式
    NotRender   //!不渲染
};

struct Character {
    GLuint TextureID;  // 字形纹理的ID
    QSize Size;       // 字形大小
    QSize Bearing;    // 从基准线到字形左部/顶部的偏移值
    GLuint Advance;    // 原点距下一个字形原点的距离
};




class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = nullptr);
    ~GLWidget() override;

public slots:
    void cleanup();
    void paintOpenGL();
    void setRenderMode(RenderMode mode);
    void updateData(QVector<QVector3D> local, QVector<unsigned int> ii);



protected:
    void initializeGL() override;   //!必须重载
    void paintGL() override;        //!必须重载
    void resizeGL(int width, int height) override;  //!必须重载

    QSize minimumSizeHint() const override; //!作为子窗口时设置子窗口最小尺寸
    QSize sizeHint() const override;        //!作为子窗口时设置子窗口默认尺寸

    void keyPressEvent(QKeyEvent* key) override;//!键盘事件
    void wheelEvent(QWheelEvent *event) override;//!滚轮事件
    void mousePressEvent(QMouseEvent *event) override;//!鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;//!鼠标移动事件

private:
    void initShaders();
    //生成坐标轴数据
    /***********
    /* max: x,y,z方向最大值,单位mm
    /* step: x,y,z方向刻度间距，单位mm
    /* out: 坐标轴数据，只能用glDrawArrays画图
    ***********/
    bool genCoordData(const QVector3D max, const QVector3D step,
                      QVector<QVector3D>& out);
    RenderMode mode;

    unsigned int uboExampleBlock;//!uniform缓冲块句柄
    unsigned int uboBindPoint;  //!ubo绑定点

    //!煤场缓冲区句柄
    QOpenGLShaderProgram *m_program;    //!模型着色器
    QOpenGLVertexArrayObject m_vao;//!VAO用于解释顶点属性，告诉着色器从哪个缓冲区（顶点属性）中取相应的数据
    QOpenGLBuffer m_vbo;//!array buffer，VBO用于存储顶点属性
    QOpenGLBuffer m_colorvbo;   //!颜色缓冲区
    int count;//!煤场图元计数

    //!坐标轴缓冲区句柄
    int indexSize;//!坐标系图元计数
    QOpenGLShaderProgram *coordPro;//!坐标着色器
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;

    //!各种变换矩阵
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
    QVector3D lightColor;//!光源颜色
    QVector3D objectColor;//!物体反射的颜色
    QVector3D lightPos;//!光源位置

    QMatrix4x4 scaleM;//!缩放矩阵
    QMatrix4x4 rotationM;//!旋转矩阵
    QMatrix4x4 translateM;//!平移矩阵

    QPointF m_lastPos;//!鼠标最后一次的位置


    QVector<QVector<QVector3D>> cloudPoints;//!模型三维点云,二维vector
    QVector<GLuint> index;
    bool m_ok = false;

    int nRange;

    //!字符纹理
    QOpenGLTexture *mTexture;
    struct Vertex
    {
        QVector3D positon;
        QVector2D tex;
    };
//    QOpenGLBuffer textvbo;
//    QOpenGLVertexArrayObject textvao;
    QOpenGLShaderProgram *textPro;//!纹理着色器
    GLuint textvbo, textvao;

    QOpenGLTexture* genTexture(int width, int height, const QString &text, int textPixelSize, const QColor &textColor);

};

#endif // OPENGLWIDGET_H
