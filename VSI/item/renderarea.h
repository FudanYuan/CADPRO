#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QList>
#include <QPainterPath>
#include <QRect>
#include <QWidget>
#include <QDebug>

QT_BEGIN_NAMESPACE
class QPaintEvent;
QT_END_NAMESPACE



class RenderArea : public QWidget
{
    Q_OBJECT

public:
    /*value表示当前属于哪个对话框,
     * value=1,绘制正多边形的
     * value=2,绘制梯形的
    */
    RenderArea(QWidget *parent, int value, int tra_dialog_type);
    RenderArea(QWidget *parent, int lin, double alp, int typ, int value);

    void setShape(const QPainterPath &shape);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void drawCoordinates(QPainter &painter);

    int getLine_num() const;
    void setLine_num(int value);

    int getType() const;
    void setType(int value);

    double getAlpha() const;
    void setAlpha(double value);

    int getDialog_type() const;
    void setDialog_type(int value);

    int getTrapezium_dialog_type() const;
    void setTrapezium_dialog_type(int value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:

    void drawOutline(QPainter &painter);
    void drawShape(QPainter &painter);

    void drawpolygon(QPainter &painter);
    void drawtrapezium(QPainter &painter);
    void draweyelet(QPainter &painter);

    int line_num;
    double alpha;
    int type;
    int dialog_type;

    int trapezium_dialog_type;//梯形对话框中的选择类型
};
//! [2]

#endif // RENDERAREA_H
