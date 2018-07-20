#ifndef SHEETDIALOG_H
#define SHEETDIALOG_H

#include <QDialog>
#include <QObject>
#include <QRectF>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QTableWidget>
#include <QGroupBox>
#include <QDialogButtonBox>
#include <QLabel>
#include <QComboBox>
#include <QCheckBox>
#include "view.h"

#define SHEET_XML "sheet.xml"
#define WHOLE tr("整体")
#define STRIP tr("条板")
#define PACKAGE tr("卷装")

struct StripPW
{
    StripPW() :
        position(0),
        width(0)
    {}
    StripPW(qreal pos, qreal wid) :
        position(pos),
        width(wid)
    {}
    qreal position;
    qreal width;
};

struct Sheet
{
    enum SheetType{
        Whole,
        Strip,
        Package,
        None
    };

    Sheet() :
        name(""),
        type(Whole),
        material(""),
        width(1000),
        height(1000),
        componentGap(0),
        topMargin(5),
        rightMargin(5),
        bottomMargin(5),
        leftMargin(5),
        layers(1),
        margin(2),
        doubleStrip(false),
        cutPaneSize(INT_MAX)
    {

    }
    QString name;  // 材料名称
    SheetType type;  // 材料类型
    QString material; // 材料构成，非必填

    qreal width; // 宽度
    qreal height; // 长度

    qreal componentGap; // 零件之间的间距
    qreal topMargin; // 与上边框的间距
    qreal rightMargin; // 与右边框的间距
    qreal bottomMargin; // 与下边框的间距
    qreal leftMargin; // 与左边框的间距

    int layers;  // 切割层数

    // 以下在type为Strip时使用
    qreal margin;  // 上插补强边距余量
    bool doubleStrip;  // 双上插条板
    qreal cutPaneSize;  // 切割平面尺寸
    QList<StripPW> stripPW;  // 上插板位置宽度

    // 获取类型名称
    QString typeName(){
        QString name;
        switch (type) {
        case Whole:
            name = "整体";
            break;
        case Strip:
            name = "条板";
            break;
        case Package:
            name = "卷装";
            break;
        case None:
            name = "无";
            break;
        default:
            break;
        }
        return name;
    }

    // 材料的有效面积
    qreal area() const {
        return (width - leftMargin - rightMargin) *
                (height - topMargin - bottomMargin);
    }

    QRectF boundRect() const {
        return QRectF(0, 0, width, height);
    }

    // 材料的有效区域
    QRectF layoutRect() const {
        return QRectF(leftMargin, topMargin,
                      (width - leftMargin - rightMargin),
                      (height - topMargin - bottomMargin));
    }

    // 上插板区域
    QVector<QRectF> inforcementRects() const{
        QVector<QRectF> retRects;
        switch (type) {
        case Strip:
            for(int i=0; i<stripPW.length(); i++){
                retRects.append(QRectF(stripPW[i].position, topMargin,
                                       stripPW[i].width, height-2*topMargin));
            }
            break;
        case Whole:
        case Package:
        default:
            break;
        }
        return retRects;
    }

    Sheet *sheet(){
        Sheet *s = new Sheet;
        s->name = name;
        s->type = type;
        s->material = material;
        s->width = width;
        s->height = height;
        s->componentGap = componentGap;
        s->topMargin = topMargin;
        s->rightMargin = rightMargin;
        s->bottomMargin = bottomMargin;
        s->leftMargin = leftMargin;
        s->layers = layers;
        s->margin = margin;
        s->doubleStrip = doubleStrip;
        s->cutPaneSize = cutPaneSize;
        s->stripPW = stripPW;

        return s;
    }
};

class Scene;

// sheet管理对话框
class SheetDialog : public QDialog
{
    Q_OBJECT
public:
    enum RoleType{
        Manager,  // 进行材料的管理
        Nest  // 排版时进行材料的增加
    };
    SheetDialog(Sheet::SheetType type = Sheet::None);
    ~SheetDialog();
    void setDialogType(Sheet::SheetType type);  // 设置显示材料类型
    Sheet::SheetType getDialogType();  // 获取显示材料类型
    void setDialogRole(SheetDialog::RoleType role);  // 设置对话框角色
    SheetDialog::RoleType getDialogRole();  // 获取对话框角色
    void initDialog();  // 初始化对话框
    void initSheetListPanel();  // 初始化材料列表
    void initSheetInfoPanel();  // 初始化材料属性
    void initBtnPanel();  // 初始化按钮部分
    void loadSheetInfo();  // 下载材料信息
    void saveSheetInfo();  // 保存材料信息
    void updateSheetInfo(const Sheet *sheetActive);
    void updateSheetActive(Sheet *sheetActive);  // 更新
    void updateSheetListTable(QList<Sheet*> filterList);  // 更新表格
    void updateStripConfigTable(QList<StripPW> stripPW);  // 更新strip配置表格
    void setSheetInfoDisable(bool flag);  // 使能材料属性框
    QList<Sheet*> xmlFileReader(QString fileName);  // 读取xml数据
    void xmlFileWrite(QString fileName, QList<Sheet*> list);  // 写入xml数据
    Sheet* getSheetActive();
    bool sheetNameConflict(const int index);  // 检查名字是否冲突

protected:
     void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private:
    RoleType role;  // 打开该对话框的角色类型
    Sheet::SheetType sheetType;  // 材料类型
    QList<Sheet*> sheetList;  // 读入的材料列表
    QList<Sheet*> showList;  // 显示的材料列表
    Sheet *sheetActive;  // 选中材料
    int currentIndex;  // 当前序号
    bool insertFlag;  // 添加标志
    bool editFlag;  // 编辑标志
    int insertCounter; // 添加计数器
    int editCounter; // 编辑计数器
    int removeCounter; // 删除计数器
    QVBoxLayout *mainLayout;  // 主布局
    QGroupBox *sheetListGroupBox;  // 列表组件
    QTableWidget *sheetListTable;  // 列表表格
    QPushButton *sheetInsertBtn;
    QPushButton *sheetRemoveBtn;
    QPushButton *sheetEditBtn;

    QGroupBox *sheetInfoGroupBox;  // 详情组件
    QGridLayout *infoLayout;  // 详情布局
    QGridLayout *sheetPropertyLayout;  // 属性布局

    QLabel *typeLabel;
    QComboBox *typeCBox;
    QLabel *nameLabel;
    QLineEdit *nameLine;
    QLabel *materialLabel;
    QLineEdit *materialLine;
    QLabel *xLabel;
    QLineEdit *xLine;
    QLabel *yLabel;
    QLineEdit *yLine;
    QLabel *distanceLabel;
    QLineEdit *distanceLine;
    QLabel *leftLabel;
    QLineEdit *leftLine;
    QLabel *topLabel;
    QLineEdit *topLine;
    QLabel *rightLabel;
    QLineEdit *rightLine;
    QLabel *bottomLabel;
    QLineEdit *bottomLine;
    QLabel *marginLabel;
    QLineEdit *marginLine;
    QLabel *layerLabel;
    QLineEdit *layerLine;
    QCheckBox *doubleStrip;
    QLabel *cutSizeLabel;
    QLineEdit *cutSizeLine;

    QTableWidget *stripConfigTable;  // 条板配置
    View *sheetView;  // 材料视图
    Scene *sheetScene;  // 材料图层

    QDialogButtonBox *buttonBox;
signals:
    void sheetSelected(int i);  // 材料被选中

private slots:
    void onMousePositionChanged(QPointF pos);  // 响应鼠标移动
    void onDoubleStripChanged(bool checkd);
    void onSheetTypeComChanged(int index);  // 响应类型下拉框选项改变
    void onDialogButtonClicked(QAbstractButton *button);  // 响应材料信息改变
    void onSheetSelected(const QModelIndex &index);  // 响应材料选中
    void onSheetInsert();  // 响应增加材料
    void onSheetRemove();  // 响应删除材料
    void onSheetEdit();  // 响应编辑材料
};

#endif // SHEETDIALOG_H
