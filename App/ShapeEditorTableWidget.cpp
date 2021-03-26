#include "ShapeEditorTableWidget.h"
#include <QTextCodec>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QMessageBox>
#include <QSharedPointer>
#include <QList>
#include "ShapeEditorSolutionColumnDelegate.h"
#include "ShapeEditorFeatureColumnDelegate.h"
#include "RDocumentInterface.h"
#include "RMixedOperation.h"
#include "RAddObjectOperation.h"
#include "RModifyObjectOperation.h"
#include "RDeleteObjectOperation.h"
#include "RDocument.h"
#include "Global.h"
#include "RVector.h"
#include "RArc.h"
#include "RLine.h"
#include "RCircle.h"
#include "EG.h"
#include "Typedef.h"

ShapeEditorTableWidget::ShapeEditorTableWidget(int width, int height, RDocumentInterface* di, QWidget *parent)
    : _width(width),
      _height(height),
      _documentInterface(di),
      QTableWidget(parent)
{   
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    _menu = new QMenu(this);

    _actionAddLine = new QAction(QIcon(":/icons/add_line_128.png"), tc->toUnicode("添加直线"), this);
    connect(_actionAddLine, &QAction::triggered, this, &ShapeEditorTableWidget::onActionAddLine);

    _actionAddArc = new QAction(QIcon(":/icons/add_arc_128.png"), tc->toUnicode("添加弧线"), this);
    connect(_actionAddArc, &QAction::triggered, this, &ShapeEditorTableWidget::onActionAddArc);

    _actionAddCircle = new QAction(QIcon(":/icons/add_circle_128.png"), tc->toUnicode("添加圆形"), this);
    connect(_actionAddCircle, &QAction::triggered, this, &ShapeEditorTableWidget::onActionAddCircle);

    _actionDelSegment = new QAction(QIcon(":/icons/delete_128.png"), tc->toUnicode("删除线段"), this);
    connect(_actionDelSegment, &QAction::triggered, this, &ShapeEditorTableWidget::onActionDelSegment);

    _actionDeselectSegments = new QAction(QIcon(":/icons/deselect_128.png"), tc->toUnicode("取消选中"), this);
    connect(_actionDeselectSegments, &QAction::triggered, this, &ShapeEditorTableWidget::onActionDeselectSegments);

    _menu->addAction(_actionAddLine);
    _menu->addAction(_actionAddArc);
    _menu->addAction(_actionAddCircle);
    _menu->addSeparator();
    _menu->addAction(_actionDelSegment);
    _menu->addSeparator();
    _menu->addAction(_actionDeselectSegments);

    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::SingleSelection);

    QStringList labels;

    labels << tc->toUnicode("类型")
           << tc->toUnicode("X1")
           << tc->toUnicode("Y1")
           << tc->toUnicode("X2")
           << tc->toUnicode("Y2")
           << tc->toUnicode("半径")
           << tc->toUnicode("方案")
           << tc->toUnicode("特征");

    this->setColumnCount(labels.size());
    this->setHorizontalHeaderLabels(labels);

    //QFontMetrics& metrics = this->fontMetrics();
    //int nAverageWidth = metrics.averageCharWidth();

    //ui->shapeEditor->setColumnWidth(0, nAverageWidth*10);
    //ui->shapeEditor->setColumnWidth(1, nAverageWidth*10);
    //ui->shapeEditor->setColumnWidth(2, nAverageWidth*10);

    // 方案那一列使用代理
    ShapeEditorSolutionColumnDelegate* solution = new ShapeEditorSolutionColumnDelegate(this);
    this->setItemDelegateForColumn(6, solution);

    // 特征那一列使用代理
    ShapeEditorFeatureColumnDelegate* feature = new ShapeEditorFeatureColumnDelegate(this);
    this->setItemDelegateForColumn(7, feature);

    connect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);
    connect(this, &ShapeEditorTableWidget::itemSelectionChanged, this, &ShapeEditorTableWidget::onItemSelectionChanged);

}

ShapeEditorTableWidget::~ShapeEditorTableWidget()
{

}

void ShapeEditorTableWidget::reinitialization()
{
    RDocument& document = _documentInterface->getDocument();

    RLayer::Id layerId = document.getLayerId("0");
    //RLayer::Id layerId = document.getCurrentLayerId();

    QList<REntity::Id> ids = document.queryLayerEntities(layerId).toList();

    for (int i = 0; i < ids.length(); ++i)
    {
        QSharedPointer<REntity> ent = document.queryEntityDirect(ids[i]);

        if (ent->getType() == RS::EntityPolyline)
        {
            QSharedPointer<RPolylineEntity> polyline = ent.dynamicCast<RPolylineEntity>();
            QList<QSharedPointer<RShape>> segments = polyline->getExploded();
            RAddObjectOperation* op = new RAddObjectOperation();

            for (int j = 0; j < segments.length(); ++j)
            {
                if (!segments[j].dynamicCast<RLine>().isNull())
                {
                    QSharedPointer<RLine> line = segments[j].dynamicCast<RLine>();
                    RLine* nLine = line->clone();
                    QSharedPointer<RLineEntity> lineEntity = QSharedPointer<RLineEntity>(new RLineEntity(&document, RLineData(*nLine)));
                    lineEntity->setColor(RColor(0, 128, 255));
                    lineEntity->setLineweight(RLineweight::Weight211);
                    op->addObject(lineEntity, false);
                    delete nLine;
                }
                else if (!segments[j].dynamicCast<RArc>().isNull())
                {
                    QSharedPointer<RArc> arc = segments[j].dynamicCast<RArc>();
                    RArc* nArc = arc->clone();
                    QSharedPointer<RArcEntity> arcEntity = QSharedPointer<RArcEntity>(new RArcEntity(&document, RArcData(*nArc)));
                    arcEntity->setColor(RColor(0, 128, 255));
                    arcEntity->setLineweight(RLineweight::Weight211);
                    op->addObject(arcEntity, false);
                    delete nArc;
                }
            }
            _documentInterface->applyOperation(op);

            RDeleteObjectOperation* delOp = new RDeleteObjectOperation(polyline);
            _documentInterface->applyOperation(delOp);
        }
    }

    _documentInterface->regenerateScenes();

    ids.clear();

    ids = document.queryLayerEntities(layerId).toList();

    for (int i = 0; i < ids.length(); ++i)
    {
        QSharedPointer<REntity> ent = document.queryEntityDirect(ids[i]);

        if (ent->getType() == RS::EntityLine)
        {
            QSharedPointer<RLineEntity> line = ent.dynamicCast<RLineEntity>();
            if (!line.isNull())
            {
                addLine2TableWidget(line);
            }
        }
        else if (ent->getType() == RS::EntityArc)
        {
            QSharedPointer<RArcEntity> arc = ent.dynamicCast<RArcEntity>();
            addArc2TableWidget(arc);
        }
        else if (ent->getType() == RS::EntityCircle)
        {
            QSharedPointer<RCircleEntity> circle = ent.dynamicCast<RCircleEntity>();
            addCircle2TableWidget(circle);
        }
    }
}

void ShapeEditorTableWidget::contextMenuEvent(QContextMenuEvent* event)
{
    if (event != NULL)
    {
        QTableWidgetItem* item = this->itemAt(event->pos());
        if (item != NULL && item->isSelected())
        {
            _actionDelSegment->setEnabled(true);
        }
        else
        {
            _actionDelSegment->setDisabled(true);
        }

        _menu->exec(event->globalPos());
    }

    QTableWidget::contextMenuEvent(event);
}

void ShapeEditorTableWidget::onActionAddArc(bool /*checked*/)
{
    if (!_documentInterface)
    {
        return;
    }

    // 如果有上一行，就把上一行的X2、Y2填到下一行的X1、Y1
    double X1 = 0.0f;
    double Y1 = 0.0f;

    int index = this->rowCount();

    if (index >= 1)
    {
        QTableWidgetItem* itemX2 = this->item(index-1, 3);
        QTableWidgetItem* itemY2 = this->item(index-1, 4);

        bool ok = false;
        X1 = itemX2->text().toDouble(&ok);
        if (!ok)
        {
            X1 = 0.0f;
        }
        Y1 = itemY2->text().toDouble(&ok);
        if (!ok)
        {
            Y1 = 0.0f;
        }
    }


    QSharedPointer<RArcEntity> arcEnt = QSharedPointer<RArcEntity>(
                new RArcEntity(&_documentInterface->getDocument(), RArcData(EG::getArc2PR(RVector(X1, Y1), RVector(X1+1.0, Y1+1.0), 1.0, false))));

    arcEnt->setColor(RColor(0, 128, 255));
    //arcEnt->setLinetypeId(_documentInterface->getDocument().getLinetypeId("dashed"));
    arcEnt->setLineweight(RLineweight::Weight211);

    RAddObjectOperation* op = new RAddObjectOperation(arcEnt, false);

    _documentInterface->applyOperation(op);

    addArc2TableWidget(arcEnt);
}

void ShapeEditorTableWidget::onActionAddLine(bool /*checked*/)
{
    if (!_documentInterface)
    {
        return;
    }

    // 如果有上一行，就把上一行的X2、Y2填到下一行的X1、Y1
    double X1 = 0.0f;
    double Y1 = 0.0f;

    int index = this->rowCount();

    if (index >= 1)
    {
        QTableWidgetItem* itemX2 = this->item(index-1, 3);
        QTableWidgetItem* itemY2 = this->item(index-1, 4);

        bool ok = false;
        X1 = itemX2->text().toDouble(&ok);
        if (!ok)
        {
            X1 = 0.0f;
        }
        Y1 = itemY2->text().toDouble(&ok);
        if (!ok)
        {
            Y1 = 0.0f;
        }
    }


    QSharedPointer<RLineEntity> lineEnt = QSharedPointer<RLineEntity>(
                new RLineEntity(&_documentInterface->getDocument(), RLineData(RLine(X1, Y1, 0.0, 0.0))));

    lineEnt->setColor(RColor(0, 128, 255));
    //lineEnt->setLinetypeId(_documentInterface->getDocument().getLinetypeId("dashed"));
    lineEnt->setLineweight(RLineweight::Weight211);

    RAddObjectOperation* op = new RAddObjectOperation(lineEnt, false);

    _documentInterface->applyOperation(op);

    addLine2TableWidget(lineEnt);
}

void ShapeEditorTableWidget::onActionAddCircle(bool /*checked*/)
{
    if (!_documentInterface)
    {
        return;
    }

    QSharedPointer<RCircleEntity> circle = QSharedPointer<RCircleEntity>(
                new RCircleEntity(&_documentInterface->getDocument(), RCircleData(RCircle(0.0, 0.0, 0.0))));

    circle->setColor(RColor(0, 128, 255));
    //lineEnt->setLinetypeId(_documentInterface->getDocument().getLinetypeId("dashed"));
    circle->setLineweight(RLineweight::Weight211);

    RAddObjectOperation* op = new RAddObjectOperation(circle, false);

    _documentInterface->applyOperation(op);

    addCircle2TableWidget(circle);
}

void ShapeEditorTableWidget::onActionDelSegment(bool /*checked*/)
{
    int index = this->currentRow();

    QTableWidgetItem* item0 = this->item(index, 0);
    REntity::Id id = item0->data(ENTITY_ID).toInt();
    QSharedPointer<REntity> ent = _documentInterface->getDocument().queryEntityDirect(id);

    if (ent.isNull())
    {
        return;
    }

    RDeleteObjectOperation* op = new RDeleteObjectOperation(ent);

    _documentInterface->applyOperation(op);
    _documentInterface->regenerateScenes();
    _documentInterface->autoZoom();

    this->removeRow(index);
}

void ShapeEditorTableWidget::onActionDeselectSegments(bool /*checked*/)
{
    QList<QTableWidgetSelectionRange> ranges = this->selectedRanges();

    for (int i = 0 ; i < ranges.size(); ++i)
    {
        QTableWidgetSelectionRange range = ranges[i];
        this->setRangeSelected(range, false);
    }
}

void ShapeEditorTableWidget::onItemChanged(QTableWidgetItem* item)
{
    if (!_documentInterface)
    {
        return;
    }

    int index = item->row();
    QTableWidgetItem* item0 = this->item(index, 0);
    REntity::Id id = item0->data(ENTITY_ID).toInt();
    QSharedPointer<REntity> ent = _documentInterface->getDocument().queryEntityDirect(id);

    if (ent.isNull())
    {
        return;
    }

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    double value = 0.0;
    bool ok = false;

    if (ent->getType() == RS::EntityLine)
    {
        QSharedPointer<RLineEntity> line = ent.dynamicCast<RLineEntity>();
        if (line.isNull())
        {
            return;
        }

        RVector pt1(line->getStartPoint());
        RVector pt2(line->getEndPoint());

        int column = item->column();
        if (column == 1)
        {
            QTableWidgetItem* itemSX = this->item(index, 1);
            value = itemSX->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            pt1.setX(value);
        }
        else if (column == 2)
        {
            QTableWidgetItem* itemSY = this->item(index, 2);
            value = itemSY->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            pt1.setY(value);
        }
        else if (column == 3)
        {
            QTableWidgetItem* itemEX = this->item(index, 3);
            value = itemEX->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            pt2.setX(value);
        }
        else if (column == 4)
        {
            QTableWidgetItem* itemEY = this->item(index, 4);
            value = itemEY->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            pt2.setY(value);
        }
        else if (column == 7)
        {
            QTableWidgetItem* feature = this->item(index, 7);
            if (feature->text() == tc->toUnicode("拉槽"))
            {
                line->setColor(RColor(255, 0, 0));
            }
            else if (feature->text() == tc->toUnicode("常规"))
            {
                line->setColor(RColor(0, 128, 255));
            }
        }

        line->setShape(RLine(pt1, pt2));

        RModifyObjectOperation* op = new RModifyObjectOperation(line);

        _documentInterface->applyOperation(op);
    }
    else if (ent->getType() == RS::EntityArc)
    {
        QSharedPointer<RArcEntity> arc = ent.dynamicCast<RArcEntity>();
        if (arc.isNull())
        {
            return;
        }

        RVector point1;
        RVector point2;
        double radius = arc->getRadius();
        bool reversed = arc->reverse();
        bool alternativeSolution = false;

        QTableWidgetItem* itemX1 = this->item(index, 1);
        QTableWidgetItem* itemY1 = this->item(index, 2);
        QTableWidgetItem* itemX2 = this->item(index, 3);
        QTableWidgetItem* itemY2 = this->item(index, 4);
        QTableWidgetItem* itemRadius = this->item(index, 5);
        QTableWidgetItem* itemSolution = this->item(index, 6);
        QTableWidgetItem* itemFeature = this->item(index, 7);

        if (itemX1 && itemY1 && itemX2 && itemY2 && itemRadius && itemSolution)
        {
            value = itemX1->text().toDouble(&ok);
            if (!ok)
            {
                itemX1->setText("0.0");
                return;
            }
            point1.setX(value);

            ok = false;
            value = itemY1->text().toDouble(&ok);
            if (!ok)
            {
                itemY1->setText("0.0");
                return;
            }
            point1.setY(value);

            ok = false;
            value = itemX2->text().toDouble(&ok);
            if (!ok)
            {
                itemX2->setText("0.0");
                return;
            }
            point2.setX(value);

            ok = false;
            value = itemY2->text().toDouble(&ok);
            if (!ok)
            {
                itemY2->setText("0.0");
                return;
            }
            point2.setY(value);

            ok = false;
            radius = itemRadius->text().toDouble(&ok);
            if (!ok)
            {
                itemRadius->setText("0.0");
                return;
            }

            if (itemSolution->text() == "1")
            {
                reversed = false;
                alternativeSolution = false;
            }
            else if (itemSolution->text() == "2")
            {
                reversed = true;
                alternativeSolution = false;
            }
            else if (itemSolution->text() == "3")
            {
                reversed = false;
                alternativeSolution = true;
            }
            else if (itemSolution->text() == "4")
            {
                reversed = true;
                alternativeSolution = true;
            }

            if (itemFeature->text() == tc->toUnicode("拉槽"))
            {
                arc->setColor(RColor(255, 0, 0));
            }
            else if (itemFeature->text() == tc->toUnicode("常规"))
            {
                arc->setColor(RColor(0, 128, 255));
            }

            arc->setShape(EG::getArc2PR(point1, point2, radius, reversed, alternativeSolution));

            RModifyObjectOperation* op = new RModifyObjectOperation(arc);

            _documentInterface->applyOperation(op);
        }
    }
    else if (ent->getType() == RS::EntityCircle)
    {
        QSharedPointer<RCircleEntity> circle = ent.dynamicCast<RCircleEntity>();
        if (circle.isNull())
        {
            return;
        }


        RVector ptCenter(circle->getCenter());
        double radius = circle->getRadius();

        int column = item->column();
        if (column == 1)
        {
            QTableWidgetItem* itemSX = this->item(index, 1);
            value = itemSX->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            ptCenter.setX(value);
        }
        else if (column == 2)
        {
            QTableWidgetItem* itemSY = this->item(index, 2);
            value = itemSY->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            ptCenter.setY(value);
        }
        else if (column == 5)
        {
            QTableWidgetItem* itemRadius = this->item(index, 5);
            value = itemRadius->text().toDouble(&ok);
            if (!ok)
            {
                item->setText("0.0");
                return;
            }
            radius = value;
        }
        else if (column == 7)
        {
            QTableWidgetItem* feature = this->item(index, 7);
            if (feature->text() == tc->toUnicode("打孔"))
            {
                circle->setColor(RColor(255, 0, 0));
            }
            else if (feature->text() == tc->toUnicode("常规"))
            {
                circle->setColor(RColor(0, 128, 255));
            }
        }

        RVector offset = ptCenter - circle->getCenter();
        circle->move(offset);
        circle->setRadius(radius);

        RModifyObjectOperation* op = new RModifyObjectOperation(circle);

        _documentInterface->applyOperation(op);
    }

    _documentInterface->regenerateScenes();
    _documentInterface->autoZoom();
}

void ShapeEditorTableWidget::onItemSelectionChanged()
{
    if (!_documentInterface)
    {
        return;
    }

    QList<QTableWidgetSelectionRange> ranges = this->selectedRanges();

    if (ranges.size() == 0)
    {
        _documentInterface->deselectAll();
        return;
    }

    int index = ranges.at(0).topRow();

    QTableWidgetItem* item0 = this->item(index, 0);
    REntity::Id id = item0->data(ENTITY_ID).toInt();
    QSharedPointer<REntity> ent = _documentInterface->getDocument().queryEntityDirect(id);

    if (ent.isNull())
    {
        return;
    }

    _documentInterface->selectEntity(ent->getId());

}


void ShapeEditorTableWidget::addArc2TableWidget(QSharedPointer<RArcEntity> arcEntity)
{
    disconnect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int index = this->rowCount();

    this->insertRow(index);

    int column = this->columnCount();

    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(index, i, item);

        if (i == 0)
        {
            item->setText(tc->toUnicode("弧线"));
            item->setData(ENTITY_ID, arcEntity->getId());
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 1)
        {
            RVector pt = arcEntity->getStartPoint();
            if (pt.x > -EPSINON && pt.x < EPSINON)
            {
                pt.x = 0.0f;
            }
            item->setText(QString("%1").arg(pt.x));
        }
        else if (i == 2)
        {
            RVector pt = arcEntity->getStartPoint();
            if (pt.y > -EPSINON && pt.y < EPSINON)
            {
                pt.y = 0.0f;
            }
            item->setText(QString("%1").arg(pt.y));
        }
        else if (i == 3)
        {
            RVector pt = arcEntity->getEndPoint();
            if (pt.x > -EPSINON && pt.x < EPSINON)
            {
                pt.x = 0.0f;
            }
            item->setText(QString("%1").arg(pt.x));
        }
        else if (i == 4)
        {
            RVector pt = arcEntity->getEndPoint();
            if (pt.y > -EPSINON && pt.y < EPSINON)
            {
                pt.y = 0.0f;
            }
            item->setText(QString("%1").arg(pt.y));
        }
        else if (i == 5)
        {
            double radius = arcEntity->getRadius();
            item->setText(QString("%1").arg(radius));
        }
        else if (i == 6)
        {
            item->setText("1");
        }
        else if (i == 7)
        {
            if (arcEntity->getColor() == SLOT_COLOR)
            {
                item->setText(tc->toUnicode("拉槽"));
            }
            else
            {
                item->setText(tc->toUnicode("常规"));
            }
        }
    }

    connect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);
}

void ShapeEditorTableWidget::addLine2TableWidget(QSharedPointer<RLineEntity> lineEntity)
{
    disconnect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int index = this->rowCount();

    this->insertRow(index);

    int column = this->columnCount();

    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(index, i, item);

        if (i == 0)
        {
            item->setText(tc->toUnicode("直线"));
            item->setData(ENTITY_ID, lineEntity->getId());
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 1)
        {
            RVector pt = lineEntity->getStartPoint();
            if (pt.x > -EPSINON && pt.x < EPSINON)
            {
                pt.x = 0.0f;
            }

            item->setText(QString("%1").arg(pt.x));
        }
        else if (i == 2)
        {
            RVector pt = lineEntity->getStartPoint();
            if (pt.y > -EPSINON && pt.y < EPSINON)
            {
                pt.y = 0.0f;
            }

            item->setText(QString("%1").arg(pt.y));
        }
        else if (i == 3)
        {
            RVector pt = lineEntity->getEndPoint();
            if (pt.x > -EPSINON && pt.x < EPSINON)
            {
                pt.x = 0.0f;
            }

            item->setText(QString("%1").arg(pt.x));
        }
        else if (i == 4)
        {
            RVector pt = lineEntity->getEndPoint();
            if (pt.y > -EPSINON && pt.y < EPSINON)
            {
                pt.y = 0.0f;
            }

            item->setText(QString("%1").arg(pt.y));
        }
        else if (i == 5)
        {
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 6)
        {
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 7)
        {
            if (lineEntity->getColor() == SLOT_COLOR)
            {
                item->setText(tc->toUnicode("拉槽"));
            }
            else
            {
                item->setText(tc->toUnicode("常规"));
            }
        }
    }

    connect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);
}

void ShapeEditorTableWidget::addCircle2TableWidget(QSharedPointer<RCircleEntity> circleEntity)
{
    disconnect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int index = this->rowCount();

    this->insertRow(index);

    int column = this->columnCount();

    for (int i = 0; i < column; ++i)
    {
        QTableWidgetItem* item = new QTableWidgetItem(QTableWidgetItem::Type);
        item->setTextAlignment(Qt::AlignCenter);
        this->setItem(index, i, item);

        if (i == 0)
        {
            item->setText(tc->toUnicode("圆形"));
            item->setData(ENTITY_ID, circleEntity->getId());
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 1)
        {
            RVector pt = circleEntity->getCenter();
            item->setText(QString("%1").arg(pt.x));
        }
        else if (i == 2)
        {
            RVector pt = circleEntity->getCenter();
            item->setText(QString("%1").arg(pt.y));
        }
        else if (i == 3)
        {
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 4)
        {
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 5)
        {
            double radius = circleEntity->getRadius();
            item->setText(QString("%1").arg(radius));
        }
        else if (i == 6)
        {
            item->setFlags(item->flags()&(~Qt::ItemIsEditable));
        }
        else if (i == 7)
        {
            if (circleEntity->getColor() == HOLE_COLOR)
            {
                item->setText(tc->toUnicode("打孔"));
            }
            else
            {
                item->setText(tc->toUnicode("常规"));
            }
        }
    }

    connect(this, &ShapeEditorTableWidget::itemChanged, this, &ShapeEditorTableWidget::onItemChanged);
}

void ShapeEditorTableWidget::mousePressEvent(QMouseEvent* event)
{
    QTableWidgetItem* item =  this->itemAt(event->pos());

    if (!item)
    {
        onActionDeselectSegments();
    }

    QTableWidget::mousePressEvent(event);
}
