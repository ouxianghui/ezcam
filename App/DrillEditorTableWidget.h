#ifndef DRILLEDITORTABLEWIDGET_H
#define DRILLEDITORTABLEWIDGET_H

#include <QTableWidget>

class QMenu;
class QAction;
class QContextMenuEvent;

class DrillEditorTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit DrillEditorTableWidget(QWidget *parent = 0);

protected:
    void init();
    void save2Settings();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

signals:

private slots:
    void onActionAddDrill(bool checked = false);
    void onActionDelDrill(bool checked = false);

    void onItemChanged(QTableWidgetItem* item);

private:
    QMenu* _menu;
    QAction* _actionAddDrill;
    QAction* _actionDelDrill;
};

#endif // DRILLEDITORTABLEWIDGET_H
