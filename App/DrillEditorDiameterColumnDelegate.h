#ifndef DRILLEDITORDIAMETERCOLUMNDELEGATE_H
#define DRILLEDITORDIAMETERCOLUMNDELEGATE_H

#include <QStyledItemDelegate>

class DrillEditorDiameterColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    DrillEditorDiameterColumnDelegate(QObject* parent = 0);
    ~DrillEditorDiameterColumnDelegate();

protected:
    virtual QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const;
    virtual void setEditorData(QWidget* editor, const QModelIndex& index) const;
    virtual void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const;

signals:
    void closedEditor(QWidget* widget);

protected slots:
    void onCommitSelect(int index);

private:
    int _columnType;
};

#endif // DRILLEDITORDIAMETERCOLUMNDELEGATE_H
