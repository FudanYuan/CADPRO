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
     * value=3,绘制鸡眼孔
    */
    explicit RenderArea(QWidget *parent, int value, int tra_dialog_type);
    explicit RenderArea(QWidget *parent, int lin, double alp, int typ, int value);

    void setShape(const QPainterPath &shape);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void drawCoordinates(QPainter &painter);

    int getLineNum() const;
    void setLineNum(int value);

    int getType() const;
    void setType(int value);

    double getAlpha() const;
    void setAlpha(double value);

    int getDialogType() const;
    void setDialogType(int value);

    int getTrapeziumDialogType() const;
    void setTrapeziumDialogType(int value);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void drawOutline(QPainter &painter);
    void drawShape(QPainter &painter);

    void drawpolygon(QPainter &painter);
    void drawtrapezium(QPainter &painter);
    void draweyelet(QPainter &painter);

    int lineNum;
    double alpha;
    int type;
    int dialogType;

    int trapeziumDialogType;//梯形对话框中的选择类型
};
//! [2]

#endif // RENDERAREA_H
