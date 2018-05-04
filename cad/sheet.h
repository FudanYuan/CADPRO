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
#include "sheet.h"
#include "view.h"
#include "scene.h"

#define SHEET_XML "sheet.xml"
#define WHOLE tr("整体")
#define STRIP tr("样条")
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
        Package
    };

    Sheet();

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

    // 材料的有效面积
    qreal area() const {
        return (width - leftMargin - rightMargin) *
                (height - topMargin - bottomMargin);
    }

    // 材料的有效区域
    QRectF layoutRect() const {
        return QRectF(leftMargin, topMargin,
                      (width - leftMargin - rightMargin),
                      (height - topMargin - bottomMargin));
    }
};

// sheet管理对话框
class SheetDialog : public QDialog
{
    Q_OBJECT
public:
    enum RoleType{
        Manager,  // 进行材料的管理
        Nest  // 排版时进行材料的增加
    };
    SheetDialog();
    void setDialogRole(SheetDialog::RoleType role);  // 设置对话框角色
    SheetDialog::RoleType getDialogRole();  // 获取对话框角色
    void initDialog();  // 初始化对话框
    void initSheetListPanel();  // 初始化材料列表
    void initSheetInfoPanel();  // 初始化材料属性
    void initBtnPanel();  // 初始化按钮部分
    void loadSheetInfo();  // 下载材料信息
    void saveSheetInfo();  // 保存材料信息
    void updateSheetInfo(const Sheet *sheetActive);
    void updateSheetListTable(QList<Sheet*> sheetList);  // 更新表格
    void updateStripConfigTable(QList<StripPW> stripPW);  // 更新strip配置表格
    QList<Sheet*> xmlFileReader(QString fileName);  // 读取xml数据
    void xmlFileWrite(QString fileName, QList<Sheet*> list);  // 写入xml数据

    Sheet* getSheetActive();
private:
    RoleType role;  // 打开该对话框的角色类型
    QList<Sheet*> sheetList;  // 材料列表
    Sheet *sheetActive;  // 选中材料
    int currentIndex;  // 当前序号
    int newSheetItem;  // 新材料个数
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
    void onStripConfigTableChanged(int row, int column);
    void onDoubleStripChanged(bool checkd);
    void onSheetTypeComChanged(int index);  // 响应类型下拉框选项改变
    void onDialogButtonClicked(QAbstractButton *button);  // 响应材料信息改变
    void onSheetSelected(const QModelIndex &index);  // 响应材料选中
    void onSheetInsert();  // 响应增加材料
    void onSheetRemove();  // 响应删除材料
    void onSheetEdit();  // 响应编辑材料
};

#endif // SHEETDIALOG_H
