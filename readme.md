# 学习QT+OpenGL

## 2019-12-10更新点

1. 基于QtOpenGLWidget + QtPaint混合渲染的方式渲染字符。OpenGL负责渲染三维模型，QtPaint负责渲染二维文本。了解到QtPaintEvent会调用GLPaint函数，所以直接在GLPaint函数中渲染文本字符，而不必重写控件的paintEvent。

### 注意事项

1. QOpenGLWidget控件配置OpenGL上下文时，要在构造函数中进行，而不是在main函数中全局配置（Qt的帮助文档是建议在main函数中全局配置，这是一个坑）。在main函数中全局配置时，会导致OpenGL的一些指令失效，比如glLineWidth函数无法设置线宽。具体请参考如下连接：

    [QPainter中的OpenGL核心配置文件上下文支持](https://www.qt.io/blog/2017/01/27/opengl-core-profile-context-support-qpainter)

2. 混合编程时，请确保在QtPaint绘制之前，OpenGL的所有状态已关闭，所以，最好让QtPaint最后绘制，具体请参考如下连接：

    [OpenGL和QPainter混合编程](https://www.qt.io/blog/2011/11/21/qt-commercial-support-weekly-4-mixing-opengl-and-qpainter-qt-4-5-x-with-sun-studio-12-2)