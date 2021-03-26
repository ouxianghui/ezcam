#ifndef SHAPEEDITORTABLEWIDGET_H
#define SHAPEEDITORTABLEWIDGET_H

#include <QTableWidget>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include <vector>
#include "RArcEntity.h"
#include "RLineEntity.h"
#include "RCircleEntity.h"
#include "RPolylineEntity.h"

class QMouseEvent;
class RDocumentInterface;

class ShapeEditorTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit ShapeEditorTableWidget(int width, int height, RDocumentInterface* di, QWidget *parent = 0);
    ~ShapeEditorTableWidget();

    void reinitialization();

protected:
    void contextMenuEvent(QContextMenuEvent* event);

protected:
    void mousePressEvent(QMouseEvent* event);

signals:

private slots:
    void onActionAddArc(bool checked = false);
    void onActionAddLine(bool checked = false);
    void onActionAddCircle(bool checked = false);
    void onActionDelSegment(bool checked = false);
    void onActionDeselectSegments(bool checked = false);

    void onItemChanged(QTableWidgetItem* item);
    void onItemSelectionChanged();

private:
    void drawOutline();
    void addArc2TableWidget(QSharedPointer<RArcEntity> arcEntity);
    void addLine2TableWidget(QSharedPointer<RLineEntity> lineEntity);
    void addCircle2TableWidget(QSharedPointer<RCircleEntity> circleEntity);


private:
    int _width;
    int _height;
    QMenu* _menu;
    QAction* _actionAddArc;
    QAction* _actionAddLine;
    QAction* _actionAddCircle;
    QAction* _actionDelSegment;
    QAction* _actionDeselectSegments;
    RDocumentInterface* _documentInterface;

};

#endif // SHAPEEDITORTABLEWIDGET_H
