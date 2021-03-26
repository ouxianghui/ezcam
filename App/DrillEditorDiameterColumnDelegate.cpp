#include "DrillEditorDiameterColumnDelegate.h"
#include <QTextOption>
#include <QComboBox>
#include <QPainter>
#include "SystemConfigManager.h"


DrillEditorDiameterColumnDelegate::DrillEditorDiameterColumnDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{

}

DrillEditorDiameterColumnDelegate::~DrillEditorDiameterColumnDelegate()
{

}

QWidget* DrillEditorDiameterColumnDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    QMap<QString, QVariant> map;
    smgr.getHoles(map);

    QComboBox* comboBox = new QComboBox(parent);
    for (QMap<QString, QVariant>::iterator it = map.begin(); it != map.end(); ++it)
    {
        comboBox->addItem(it.key());
    }

    connect(comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onCommitSelect(int)));
    return comboBox;
}

void DrillEditorDiameterColumnDelegate::paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QTextOption opt(Qt::AlignHCenter | Qt::AlignVCenter);
    painter->drawText(option.rect, text, opt);
}

void DrillEditorDiameterColumnDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QString text = index.model()->data(index, Qt::DisplayRole).toString();
    QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
    comboBox->setCurrentText(text);
}

void DrillEditorDiameterColumnDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QComboBox* comboBox = qobject_cast<QComboBox *>(editor);
    QString text = comboBox->currentText();
    model->setData(index, text);
}

void DrillEditorDiameterColumnDelegate::onCommitSelect(int /*index*/)
{
    QComboBox *comboBox = qobject_cast<QComboBox *>(sender());

    emit closedEditor(comboBox);
}
