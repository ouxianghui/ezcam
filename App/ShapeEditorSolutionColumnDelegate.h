#ifndef SHAPEEDITORSOLUTIONCOLUMNDELEGATE_H
#define SHAPEEDITORSOLUTIONCOLUMNDELEGATE_H

#include <QStyledItemDelegate>

class ShapeEditorSolutionColumnDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    ShapeEditorSolutionColumnDelegate(QObject* parent = 0);
    ~ShapeEditorSolutionColumnDelegate();

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

#endif // SHAPEEDITORSOLUTIONCOLUMNDELEGATE_H
