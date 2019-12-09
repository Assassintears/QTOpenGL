#ifndef PROFILE_H
#define PROFILE_H
#include <QWidget>
#include <QVector>
#include <QBrush>
#include <QFont>
#include <QPen>

class Profile : public QWidget
{
    Q_OBJECT

public:
    Profile(QWidget* parent = nullptr);

public:
    void paint(QPainter *painter);


protected:
    void paintEvent(QPaintEvent *event) override;
    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

private:
    QVector<double> vecxy;
    QBrush background;
    QFont textFont;
    QPen textPen;
};

#endif // PROFILE_H
