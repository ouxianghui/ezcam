#ifndef HOLEEDITORTABLEWIDGET_H
#define HOLEEDITORTABLEWIDGET_H

#include <QTableWidget>

class QMenu;
class QAction;
class QContextMenuEvent;

class HoleEditorTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit HoleEditorTableWidget(QWidget *parent = 0);

protected:
    void init();
    void save2Settings();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

signals:

private slots:
    void onActionAddHole(bool checked = false);
    void onActionDelHole(bool checked = false);

    void onItemChanged(QTableWidgetItem* item);

private:
    QMenu* _menu;
    QAction* _actionAddHole;
    QAction* _actionDelHole;
};

#endif // HOLEEDITORTABLEWIDGET_H
