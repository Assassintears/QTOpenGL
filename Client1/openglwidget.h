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
#include <QPen>
#include <QFont>
#include <QHash>
#include <QList>
#include <QQuaternion>


QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

typedef enum RenderMode
{
    Patch,      //!填充
    PointCloud, //!点云
    Lines,      //!线模式
    NotRender   //!不渲染
};

//! 定义材质
typedef struct Material
{
    QVector3D ambient;  //! 背景光，环境光
    QVector3D diffuse;  //! 漫反射
    QVector3D specular; //! 镜面反
    float shinines;     //! 镜面反射强度
};

//! 定义光源
typedef struct Light
{
    QVector3D postion;  //! 点光源位置
//    QVector3D direction;//! 平行光方向
    QVector3D ambient;  //! 背景光，环境光
    QVector3D diffuse;  //! 漫反射
    QVector3D specular; //! 镜面反
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
    void updateData(QVector<QVector3D> points3D, QVector<QVector3D> normal, QVector<QVector3D> color);
    void rotateRight();
    void rotateLeft();



protected:
    void initializeGL() override;   //!必须重载
    void paintGL() override;        //!必须重载
    void resizeGL(int width, int height) override;          //!必须重载

    QSize minimumSizeHint() const override;                 //!作为子窗口时设置子窗口最小尺寸
    QSize sizeHint() const override;                        //!作为子窗口时设置子窗口默认尺寸

//    void keyPressEvent(QKeyEvent* key) override;//!键盘事件
    void wheelEvent(QWheelEvent *event) override;               //!滚轮事件
    void mousePressEvent(QMouseEvent *event) override;          //!鼠标按下事件
    void mouseMoveEvent(QMouseEvent *event) override;           //!鼠标移动事件
//    void mouseReleaseEvent(QMouseEvent *event) override;        //! 鼠标释放事件
    void mouseDoubleClickEvent(QMouseEvent *event) override;    //! 左键双击事件--重置OpenGL模型初始位置

private:
    void initShaders();
    //生成坐标轴数据
    /***********
    /@ max: x,y,z方向最大值,单位mm
    /@ step: x,y,z方向刻度间距，单位mm
    /@ out: 坐标轴数据，只能用glDrawArrays画图
    ***********/
    bool genCoordData(const QVector3D max, const QVector3D step,
                      QVector<QVector3D>& out);

    void drawCoal();                    //! 渲染煤场
    void drawCoord();                   //! 渲染坐标轴
    void drawText(QPainter& pt);        //! 渲染文本

    RenderMode mode;                    //!渲染模式

    unsigned int uboExampleBlock;       //!uniform缓冲块句柄
    unsigned int uboBindPoint;          //!ubo绑定点

    //!煤场缓冲区句柄
    QOpenGLShaderProgram *m_program;    //!模型着色器
    QOpenGLVertexArrayObject m_vao;     //!VAO用于解释顶点属性，告诉着色器从哪个缓冲区（顶点属性）中取相应的数据
    QOpenGLBuffer m_vbo;                //!array buffer，VBO用于存储顶点属性
    QOpenGLBuffer m_colorvbo;           //! 颜色缓冲区
    QOpenGLBuffer m_normal;             //! 法向量缓冲区
    int count;//!煤场图元计数

    //! 材质
    Material material;
    //! 光源
    Light light;

    //!坐标轴缓冲区句柄
    int indexSize;                              //!坐标系图元计数
    QOpenGLShaderProgram *coordPro;             //!坐标着色器
    QOpenGLBuffer VBO;
    QOpenGLVertexArrayObject VAO;
    QHash<QString, QList<QVector3D> > labels;   //!存储坐标轴刻度及其世界坐标位置,键为标签

    //!各种变换矩阵
    float x0, x1, y0, y1, z0, z1;   //!取值范围
    QMatrix4x4 m_proj;              //! 投影矩阵
    QMatrix4x4 m_camera;            //! 相机矩阵
    QMatrix4x4 m_model;             //! 模型矩阵
    QMatrix4x4 mvp;
    QPointF m_lastPos;              //! 鼠标最后一次的位置
    float scal;                     //! 缩放因子
    float anglex;                   //! x方向旋转角度
    float angley;                   //! y方向旋转角度
    float anglez;                   //! z方向旋转角度
    float PosX;
    float PosY;
    float PosZ;
};

#endif // OPENGLWIDGET_H
