#include <profile.h>
#include <QPainter>
#include <QVector>

Profile::Profile(QWidget* parent)
    :QWidget (parent)
{
    background = QBrush(QColor(64, 32, 64));
    textPen = QPen(Qt::white);
    textPen.setWidth(1);

    for (int i = 0; i < 50; ++i)
    {
        vecxy.push_back(i);
        vecxy.push_back(i);
    }
}

void Profile::paintEvent(QPaintEvent *event)
{
    QPainter painter;
    painter.begin(this);
    painter.setRenderHint(QPainter::Antialiasing);
    paint(&painter);
    painter.end();
}

void Profile::paint(QPainter *painter)
{
    for (int i = 0; i < vecxy.size() - 3; i += 2)
    {
        QPointF p1 = QPointF(vecxy.at(i), vecxy.at(i + 1));
        QPointF p2 = QPointF(vecxy.at(i + 2), vecxy.at(i + 3));
        painter->drawLine(p1, p2);
    }
}

QSize Profile::minimumSizeHint() const
{
    return QSize(50, 50);
}
QSize Profile::sizeHint() const
{
    return QSize(400, 400);
}

