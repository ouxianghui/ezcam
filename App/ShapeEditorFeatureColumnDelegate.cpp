#include "ShapeEditorFeatureColumnDelegate.h"
#include <QTextOption>
#include <QComboBox>
#include <QPainter>
#include <QTextCodec>
#include "ShapeEditorTableWidget.h"

ShapeEditorFeatureColumnDelegate::ShapeEditorFeatureColumnDelegate(QObject* parent)
    : QStyledItemDelegate(parent),
      _parent(parent)
{

}

ShapeEditorFeatureColumnDelegate::~ShapeEditorFeatureColumnDelegate()
{

}

QWidget* ShapeEditorFeatureColumnDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    ShapeEditorTableWidget* shapeEditor = dynamic_cast<ShapeEditorTableWidget*>(_parent);
    if (shapeEditor)
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        int row = shapeEditor->currentRow();
        QTableWidgetItem* item0 = shapeEditor->item(row, 0);
        if (item0)
        {
            QComboBox* comboBox = new QComboBox(parent);
            if (item0->text() == tc->toUnicode("圆形"))
            {
                comboBox->addItem(tc->toUnicode("常规"));
                comboBox->addItem(tc->toUnicode("打孔"));
            }
            else
            {
                comboBox->addItem(tc->toUnicode("常规"));
                comboBox->addItem(tc->toUnicode("拉槽"));
            }

            connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCommitSelect(int)));

            return comboBox;
        }

    }

    return QStyledItemDelegate::createEditor(parent, option, index);
}

void ShapeEditorFeatureColumnDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QTextOption opt(Qt::AlignHCenter | Qt::AlignVCenter);
    painter->drawText(option.rect, text, opt);
}

void ShapeEditorFeatureColumnDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
    comboBox->setCurrentText(text);
}

void ShapeEditorFeatureColumnDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text);
}

void ShapeEditorFeatureColumnDelegate::onCommitSelect(int /*index*/)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());

    emit closedEditor(comboBox);
}
