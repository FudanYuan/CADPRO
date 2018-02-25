#include "lineedit.h"

LineEdit::LineEdit(QString name, QWidget *parent) :
    QLineEdit(parent)
{
    this->name = name;
    connect(this, &LineEdit::textEdited, this, &LineEdit::onTextEdited);
}

QString LineEdit::getName()
{
    return this->name;
}

void LineEdit::onTextEdited(const QString &value)
{
    emit customTextEdited(this->name, value);
}
