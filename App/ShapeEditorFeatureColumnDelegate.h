#ifndef SHAPEEDITORFEATURECOLUMNDELEGATE_H
#define SHAPEEDITORFEATURECOLUMNDELEGATE_H

#include <QStyledItemDelegate>

class ShapeEditorFeatureColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ShapeEditorFeatureColumnDelegate(QObject* parent = 0);
    ~ShapeEditorFeatureColumnDelegate();

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
    QObject* _parent;
};

#endif // SHAPEEDITORFEATURECOLUMNDELEGATE_H
