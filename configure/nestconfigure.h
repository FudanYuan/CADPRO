#ifndef NESTCONFIGURE_H
#define NESTCONFIGURE_H

#include <QObject>
#include <QDialog>
#include <QPushButton>
#include <QTableWidget>
#include <QDialogButtonBox>
#include <QLayout>
#include <QComboBox>
#include <QColor>
#include <QSettings>
#include <QList>
#include <QDebug>
#define NEST_CONFG_FILE_PATH "nest_config.ini"

class NestConfigure : public QObject
{
    Q_OBJECT
public:
    // 网格类型
    enum GridType{rectangular, across, node};

    // 语言
    enum Language{Czech, Chinese, Deutsch, English, French, Italian, Japanese, Poland, Portuguese, Russian, Spanish, Ukrainian};
    // 【pyccknn】【俄语】
    // 【ykpaihcbka】【乌克兰】
    // 【espanol】【西班牙语】
    // 【italiano】【意大利语】
    // 【deutsch】【德语】
    // 【polski】【波兰】
    // 【francais】【法语】
    // 【cestina】【捷克语】
    // 【portugues】【葡萄牙语】

    // 网格
    struct Grid
    {
        Grid() :
            showGrid(false),
            gridType(NestConfigure::rectangular),
            gridColor(Qt::gray),
            xStep(100),
            yStep(100)
        {

        }
        bool showGrid; // 是否显示网格
        GridType gridType;  // 网格类型
        QColor gridColor; // 网格颜色
        double xStep; // x步长
        double yStep; // y步长

        void setGrid(GridType gridType, QColor gridColor, int xStep, int yStep){
            this->gridType = gridType;
            this->gridColor = gridColor;
            this->xStep = xStep;
            this->yStep = yStep;
        }
    };

    // 材料颜色
    struct SheetStyle
    {
        SheetStyle() :
            sheetColor(Qt::white),
            sheetMarginColor(Qt::red),
            reinforcementColor(Qt::darkRed),
            reinforcementMarginColor(Qt::yellow),
            sheetFilled(true),
            backgroundColor(Qt::white)
        {}

        QColor sheetColor;  // 材料颜色
        QColor sheetMarginColor;  // 材料边缘颜色
        QColor reinforcementColor;  // 上插板颜色
        QColor reinforcementMarginColor;  // 上插板边缘颜色
        bool sheetFilled;  // 材料填充
        QColor backgroundColor;  // 背景颜色
    };

    // 切割件颜色
    struct PieceStyle
    {
        PieceStyle() :
            outsideColor(Qt::darkCyan),
            insideColor(Qt::cyan),
            liningColor(Qt::darkMagenta),
            reinforcementColor(Qt::darkYellow),
            otherColor(Qt::darkBlue)
        {}

        QColor outsideColor;
        QColor insideColor;
        QColor liningColor;
        QColor reinforcementColor;
        QColor otherColor;
    };

    // 配置键值对
    struct KeyValue
    {
        KeyValue(QString key, QVariant value) {
            this->key = key;
            this->value = value;
        }

        QString key;
        QVariant value;

        void setValue(const QVariant &v){
            this->value = v;
        }
    };

    explicit NestConfigure(QObject *parent=0);
    ~NestConfigure();

    Grid mainGrid;  // 主网格
    Grid secondGrid;  // 副网格
    bool allowEntityOutOfSheet;  // 允许实体超出材料
    SheetStyle sheetStyle;  // 材料类型
    PieceStyle pieceStyle;  // 切割件类型
    QColor backgroundColor;  // 背景颜色
    bool showPiecesDistance;  // 显示切割件距离
    Language language;  // 语言
    QString workPathStr;  // 工作路径

    // 配置文件
    QSettings *settings;  // 配置对象
    void writeConfig(QSettings *settings);
    void readConfig(QSettings *settings);
    void writeConfigGrid(QSettings *settings);
    void readConfigGrid(QSettings *settings);
    void writeConfigSheetStyle(QSettings *settings);
    void readConfigSheetStyle(QSettings *settings);
    void writeConfigPieceStyle(QSettings *settings);
    void readConfigPieceStyle(QSettings *settings);
    void writeConfigOthers(QSettings *settings);
    void readConfigOthers(QSettings *settings);

    static void updateConfig(QList<KeyValue> keyValue);

signals:
    void gridChanged(bool show);

public slots:
    void onConfigChanged(QString name, QVariant value);  // 响应配置变化

};

#endif // NESTCONFIGURE_H
