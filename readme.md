# 学习QT+OpenGL

## 2019-12-10 更新点

1. 基于QtOpenGLWidget + QtPaint混合渲染的方式渲染字符。OpenGL负责渲染三维模型，QtPaint负责渲染二维文本。了解到QtPaintEvent会调用GLPaint函数，所以直接在GLPaint函数中渲染文本字符，而不必重写控件的paintEvent。

### 注意事项

1. QOpenGLWidget控件配置OpenGL上下文时，要在构造函数中进行，而不是在main函数中全局配置（Qt的帮助文档是建议在main函数中全局配置，这是一个坑）。在main函数中全局配置时，会导致OpenGL的一些指令失效，比如glLineWidth函数无法设置线宽。具体请参考如下连接：

    [QPainter中的OpenGL核心配置文件上下文支持](https://www.qt.io/blog/2017/01/27/opengl-core-profile-context-support-qpainter)

2. 混合编程时，请确保在QtPaint绘制之前，OpenGL的所有状态已关闭，所以，最好让QtPaint最后绘制，具体请参考如下连接：

    [OpenGL和QPainter混合编程](https://www.qt.io/blog/2011/11/21/qt-commercial-support-weekly-4-mixing-opengl-and-qpainter-qt-4-5-x-with-sun-studio-12-2)

## 2019-12-16 更新点

1. 增加相机矩阵，正交投影改为透视投影；

2. 鼠标控制模型运更直接。始终要记住，模型始终移动沿着x轴左右移动，沿着z轴上下移动；

3. 更改模型和坐标轴顶点着色器uniform块为投影矩阵和模型矩阵，相机矩阵作为uniform变量每次更新后从cpu传给GPU；

## 2019-12-19 更新点

1. 透视投影改为正交投影；

2. 解决鼠标控制模型漫游遇到的问题。之前的问题是鼠标左键按住移动不释放，当鼠标按相反的方向移动时，模型旋转方向未改变。引起这一问题的原因是获取鼠标上一次的位置始终在鼠标释放的时候才获取，这就导致当前鼠标位置始终与上一次释放的位置比较，比如鼠标上一次释放的位置在A点，鼠标移动到B点后又向A点移动，但是在AB中间的C位置时，鼠标的运动方向依然是AB方向。解决这一问题需要每次都更新鼠标实时位置，在鼠标移动事件中，要把当前位置赋值给鼠标历史位置。

3. 采用多线程的方式操作数据库。因为数据库操作不是即时的，当SQL语句异常或者数据库不存在时，数据库操作执行语句会有一定的卡顿。把数据库操作放到子线程中执行，避免主UI线程卡顿；

4. 多线程的方式处理点集。遍历数据库的数据还需要进一步封装成合适的格式给OpenGLWidget渲染，把这一耗时的操作也放在子线程中。
