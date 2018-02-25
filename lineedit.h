#ifndef LINEEDIT_H
#define LINEEDIT_H

#include <QLineEdit>

class LineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit LineEdit(QString name, QWidget *parent=0);
    QString getName();

private:
    QString name;

signals:
    void customTextEdited(QString name, QString value);

public slots:
    void onTextEdited(const QString &value);
};

#endif // WIDTHLINEEDIT_H
