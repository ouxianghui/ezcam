#include "EZModifyCuttingPointAction.h"
#include "RDocumentInterface.h"
#include "RGraphicsView.h"
#include "RAddObjectOperation.h"
#include "RDeleteObjectOperation.h"
#include "RSnapOnEntity.h"
#include "RSnapEnd.h"
#include "BoardRectTable.h"
#include "SystemConfigManager.h"
#include <QMessageBox>
#include <QTextCodec>

EZModifyCuttingPointAction::EZModifyCuttingPointAction(const QString& strSourceBoardID, RGuiAction* pGuiAction)
    : m_strSourceBoardID(strSourceBoardID),
    RActionAdapter(pGuiAction),
    m_state(NoState)
{

}

EZModifyCuttingPointAction::~EZModifyCuttingPointAction()
{

}

void EZModifyCuttingPointAction::beginEvent()
{
    m_nPickRangePixels = RSettings::getPickRange();

    this->setState(SettingPosition);

    if (!documentInterface)
    {
        return;
    }

    RDocument& document = documentInterface->getDocument();

    // 如果没有图层“2”，就添加一个名字为“2”的图层，该图层用来绘制NC下刀顺序和下刀点位置
    if (!document.hasLayer("2"))
    {
        QSharedPointer<RLayer> pLayer = QSharedPointer<RLayer>(new RLayer(&document, "2"));
        pLayer->setLinetypeId(document.getLinetypeId("continuous"));

        RAddObjectOperation* op = NULL;
        op = new RAddObjectOperation(pLayer);
        documentInterface->applyOperation(op);
    }

    SystemConfigManager& mgr = SystemConfigManager::GetInstance();
    m_dSourceBoardW = mgr.getSourceMaterialBoardWidth();
    m_dSourceBoardH = mgr.getSourceMaterialBoardHeight();

    m_vecBoardRectSt.clear();

    if (BoardRectTable::getBoardRectsBySourceBoardID(m_strSourceBoardID, m_vecBoardRectSt))
    {
    }

    documentInterface->setCurrentLayer("2");
    documentInterface->setSnap(new RSnapOnEntity());
}

void EZModifyCuttingPointAction::mousePressEvent(RMouseEvent& event)
{ 
    if (event.button() == Qt::LeftButton)
    {
        if (!documentInterface)
        {
            return;
        }

        /*
        RDocument& document = documentInterface->getDocument();

        RLayer::Id layerId = document.getLayerId("2");
        if (layerId != RLayer::INVALID_ID)
        {
            RLayer::Id currentLayerId = document.getCurrentLayerId();

            if (layerId != currentLayerId)
                document.setCurrentLayer(layerId);

            RGraphicsView& view = event.getGraphicsView();
            double range = view.mapDistanceFromView(m_nPickRangePixels);
            double strictRange = view.mapDistanceFromView(10);
            REntity::Id entityId = documentInterface->getClosestEntity(event.getModelPosition(), range, strictRange, false);
            if (entityId == RObject::INVALID_ID)
            {
                return;
            }

            // 重置下刀点的思路：找到距离新点最近的多边形，用多边形的外包矩形寻找在此外包矩形内的多有点并删除，然后重新放置点实体

            QSharedPointer<REntity> entity = document.queryEntityDirect(entityId);
            if (!entity.isNull())
            {
                // DrawOrder == 0，排除最外层的虚线框（也是多边形）
                if (entity->getType() == RS::EntityPolyline && entity->getDrawOrder() != 0)
                {
                    RBox box = entity->getBoundingBox();
                    QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                    for (QSet<REntity::Id>::iterator it = idSet.begin(); it != idSet.end(); ++it)
                    {
                        QSharedPointer<REntity> e = document.queryEntityDirect(*it);
                        if (!e.isNull() && e->getType() == RS::EntityPoint)
                        {
                            RDeleteObjectOperation* op = new RDeleteObjectOperation(e);
                            documentInterface->applyOperation(op);
                        }
                    }
                }
            }
        }
        */
    }
}

void EZModifyCuttingPointAction::mouseReleaseEvent(RMouseEvent& event)
{
    if (event.button() == Qt::RightButton)
    {
        this->escapeEvent();
    }
}

void EZModifyCuttingPointAction::coordinateEvent(RCoordinateEvent& event)
{
    pickCoordinate(event, false);
}

void EZModifyCuttingPointAction::coordinateEventPreview(RCoordinateEvent& event)
{
    pickCoordinate(event, true);
}

void EZModifyCuttingPointAction::escapeEvent()
{
    this->getDocumentInterface()->clearPreview();
    this->getDocumentInterface()->repaintViews();
    //this->terminate();
}

void EZModifyCuttingPointAction::initState()
{
}

void EZModifyCuttingPointAction::setState(State state)
{
    this->m_state = state;

    this->initState();

    if (state != -1)
    {
        this->simulateMouseMoveEvent();
    }

    //this->setCrosshairCursor();
    this->getDocumentInterface()->setClickMode(RAction::PickCoordinate);
}

void EZModifyCuttingPointAction::simulateMouseMoveEvent()
{
    if (!documentInterface)
    {
        return;
    }

    RGraphicsView* pView = documentInterface->getLastKnownViewWithFocus();
    if (!pView)
    {
        return;
    }

    pView->simulateMouseMoveEvent();
}

void EZModifyCuttingPointAction::pickCoordinate(RCoordinateEvent& event, bool preview)
{
    if (!documentInterface)
    {
        return;
    }

    RDocument& document = documentInterface->getDocument();

    RLayer::Id layerId = document.getLayerId("2");
    if (layerId != RLayer::INVALID_ID)
    {
        RLayer::Id currentLayerId = document.getCurrentLayerId();

        if (layerId != currentLayerId)
            document.setCurrentLayer(layerId);

        m_point = event.getModelPosition();

        RGraphicsView& view = event.getGraphicsView();
        double range = view.mapDistanceFromView(m_nPickRangePixels);
        double strictRange = view.mapDistanceFromView(10);
        REntity::Id entityId = documentInterface->getClosestEntity(m_point, range, strictRange, false);
        if (entityId == RObject::INVALID_ID)
        {
            return;
        }

        QSharedPointer<REntity> entity = document.queryEntityDirect(entityId);
        if (!entity.isNull())
        {
            // 预览的时候就不删除原先的下刀点
            if (!preview)
            {
                // DrawOrder == 0，排除最外层的虚线框（也是多边形）
                if (entity->getType() == RS::EntityPolyline && entity->getDrawOrder() != 0)
                {
                    RBox box = entity->getBoundingBox();
                    QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                    for (QSet<REntity::Id>::iterator it = idSet.begin(); it != idSet.end(); ++it)
                    {
                        QSharedPointer<REntity> e = document.queryEntityDirect(*it);
                        if (!e.isNull() && e->getType() == RS::EntityPoint)
                        {
                            RDeleteObjectOperation* op = new RDeleteObjectOperation(e);
                            documentInterface->applyOperation(op);
                        }
                    }
                }
            }
        }

        if (preview)
        {
            this->updatePreview(false);
        }
        else
        {
            documentInterface->setRelativeZero(m_point);
            this->applyOperation();
        }
    }
}

RAddObjectsOperation* EZModifyCuttingPointAction::getOperation()
{
    if (!m_point.isValid())
    {
        return NULL;
    }

    RAddObjectsOperation* op = new RAddObjectsOperation();

    QSharedPointer<RPointEntity> obj = QSharedPointer<RPointEntity>(new RPointEntity(this->getDocument(), RPointData(m_point)));

    op->addObject(obj);

    return op;
}

void EZModifyCuttingPointAction::updatePreview(bool clear)
{
    if (!documentInterface)
    {
        return;
    }

    if (clear == true)
    {
        documentInterface->clearPreview();
    }

    ROperation* op = this->getOperation();
    if (op)
    {
        documentInterface->previewOperation(op);
    }

    documentInterface->repaintViews();
}

/**
 * Applies the operation returned by this.getOperation().
 */
void EZModifyCuttingPointAction::applyOperation()
{
    if (!documentInterface)
    {
        return;
    }

    if (!m_point.isValid())
    {
        return;
    }

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    for (std::vector<BoardRectSt>::iterator it = m_vecBoardRectSt.begin(); it != m_vecBoardRectSt.end(); ++it)
    {
        BoardRectSt& rectSt = *it;

        RVector c1(rectSt.dLtY*m_dSourceBoardH, -rectSt.dLtX*m_dSourceBoardW);
        RVector c2(rectSt.dRbY*m_dSourceBoardH, -rectSt.dRbX*m_dSourceBoardW);
        RBox box(c1, c2);

        if (box.isValid() && m_point.isInside(box))
        {
            double dNCStartY =  fabs(m_point.x/m_dSourceBoardH);
            double dNCStartX =  fabs(m_point.y/m_dSourceBoardW);
            if (!BoardRectTable::updateBoardCuttingPointByID(rectSt.strBoardRectID, dNCStartX, dNCStartY))
            {
                QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("修改下刀点失败."), QMessageBox::Ok);
                return;
            }

            RAddObjectsOperation* op = this->getOperation();
            if (!op)
            {
                qWarning("Operation returned by this.getOperation() is undefined");
                return;
            }
            documentInterface->applyOperation(op);

            break;
        }
    }
}
