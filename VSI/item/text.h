#ifndef TEST_H
#define TEST_H

#include <QGraphicsPathItem>
#include <QGraphicsSceneMouseEvent>
#include "shape.h"
#include <QPointF>
#include "textdialog.h"
#include "itemproperties.h"

class Text : public Shape, public QGraphicsPathItem
{
    Q_OBJECT
public:
    Text(QGraphicsItem *parent=0);
    void startDraw(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 开始绘图
    void drawing(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;  // 绘图开始
    bool updateFlag(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE; // paint

    QString getTextcontent() const;
    void setTextcontent(const QString &value);

    int getTextPixelSize() const;
    void setTextPixelSize(int value);

    QColor getTextcolor() const;
    void setTextcolor(const QColor &value);
    ItemProperties *textproperties;
    TextDialog *textdialog;

protected:
    //鼠标事件
    void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    //拖拽事件
    void dragEnterEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragLeaveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dragMoveEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;
    void dropEvent(QGraphicsSceneDragDropEvent *event) Q_DECL_OVERRIDE;

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) Q_DECL_OVERRIDE;


private:

    QPointF cPoint, sPoint;
    int textWeight;//字体粗细
    int textPixelSize;//设置字体大小
    int textPointSize;//设置字体点大小
    bool textBold;//设置粗体
    bool textItalic;//设置斜体
    bool textUnderline;//设置下划线
    QString textcontent;//文本内容
    QColor textcolor;//文本字体颜色

signals:
    void select(Text *text);

public slots:
    void on_commandLinkButton_2_clicked();
    void onSceneMoveableChanged(bool moveable) Q_DECL_OVERRIDE;  //  响应场景可移动性改变
    void typechange();
};


#endif // TEST_H
