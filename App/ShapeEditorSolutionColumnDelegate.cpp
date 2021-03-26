#include "ShapeEditorSolutionColumnDelegate.h"
#include <QTextOption>
#include <QComboBox>
#include <QPainter>

ShapeEditorSolutionColumnDelegate::ShapeEditorSolutionColumnDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

ShapeEditorSolutionColumnDelegate::~ShapeEditorSolutionColumnDelegate()
{

}

QWidget* ShapeEditorSolutionColumnDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//    if (index.column() == 6)
//    {
        QComboBox* comboBox = new QComboBox(parent);
        comboBox->addItem("1");
        comboBox->addItem("2");
        comboBox->addItem("3");
        comboBox->addItem("4");
        connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCommitSelect(int)));
        return comboBox;
//    }
//    else
//    {
//        return QStyledItemDelegate::createEditor(parent, option, index);
//    }
}

void ShapeEditorSolutionColumnDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
//    if (index.column() == 6)
//    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QTextOption opt(Qt::AlignHCenter | Qt::AlignVCenter);
        painter->drawText(option.rect, text, opt);
//    }
//    else
//    {
//        QStyledItemDelegate::paint(painter, option, index);
//    }
}

void ShapeEditorSolutionColumnDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
//    if (index.column() == 6)
//    {
        QString text = index.model()->data(index, Qt::DisplayRole).toString();
        QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
        comboBox->setCurrentText(text);
//    }
//    else
//    {
//        QStyledItemDelegate::setEditorData(editor, index);
//    }
}

void ShapeEditorSolutionColumnDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
//    if (index.column() == 6)
//    {
        QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
        QString text = comboBox->currentText();
        model->setData(index, text);
//    }
//    else
//    {
//        QStyledItemDelegate::setModelData(editor, model, index);
//    }
}

void ShapeEditorSolutionColumnDelegate::onCommitSelect(int /*index*/)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());

    emit closedEditor(comboBox);
}
