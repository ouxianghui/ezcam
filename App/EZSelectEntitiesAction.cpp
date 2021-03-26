#include "EZSelectEntitiesAction.h"
#include "RMouseEvent.h"
#include "RSettings.h"
#include "RMath.h"
#include "RGraphicsViewQt.h"
#include <QSharedPointer>
#include "REntity.h"
#include "REntityData.h"
#include "RBlock.h"
#include "RBlockReferenceEntity.h"
#include "RBlockReferenceData.h"
#include "RSnapFree.h"
#include "RSnapAuto.h"
#include <QString>
#include <QObject>
#include "RMoveReferencePointOperation.h"
#include "RMoveSelectionOperation.h"
#include "RAddObjectOperation.h"
#include "RAddObjectsOperation.h"


EZSelectEntitiesAction::EZSelectEntitiesAction(RGuiAction* pGuiAction)
    :RActionAdapter(pGuiAction)
{

}

EZSelectEntitiesAction::~EZSelectEntitiesAction()
{

}

void EZSelectEntitiesAction::beginEvent()
{
    m_nPickRangePixels = RSettings::getPickRange();
    m_nMinPickRangePixels = std::min(m_nPickRangePixels / 2, 10);
    m_d1Model = RVector::invalid;
    m_d1Screen = RVector::invalid;
    m_d2Model = RVector::invalid;
    m_d2Screen = RVector::invalid;
    setState(Neutral);

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

    documentInterface->setCurrentLayer("2");

    m_pDocument = this->getDocument();
    documentInterface->setSnap(new RSnapFree());
}

void EZSelectEntitiesAction::setState(State state)
{
    // oxh add
    this->m_state = state;

//    if (this->m_state == SettingEntity)
//    {
//        documentInterface->setClickMode(RAction::PickCoordinate);
//    }
//    else
//    {
//        documentInterface->setClickMode(RAction::PickingDisabled);
//    }

    QString ltip = "Select entity or region";

    switch (m_state)
    {
    case Neutral:
        m_d1Model = RVector::invalid;
        m_d1Screen = RVector::invalid;
        m_d2Model = RVector::invalid;
        m_d2Screen = RVector::invalid;
        if (documentInterface->hasSelection())
        {
            ltip += "\n";
            ltip += "Move entity or reference";
        }

        break;
    case Dragging:
        m_d2Model = RVector::invalid;
        m_d2Screen = RVector::invalid;
        break;
    case SettingCorner2:
        //this->setLeftMouseTip("Set second corner");
        //this->setRightMouseTip("");
        break;

    default:
        break;
    }
}

void EZSelectEntitiesAction::suspendEvent()
{
    if (this->guiAction)
    {
        this->guiAction->setChecked(false);
    }
}

void EZSelectEntitiesAction::resumeEvent()
{

}

void EZSelectEntitiesAction::mouseMoveEvent(RMouseEvent& event)
{
    // we're in the middle of panning: don't do anything:
    if (event.buttons() == Qt::MidButton ||
            (event.buttons() == Qt::LeftButton && event.modifiers() == Qt::ControlModifier))
    {
        return;
    }

    RGraphicsView& view = event.getGraphicsView();
    RVector screenPosition;
    RVector referencePoint;
    REntity::Id entityId = REntity::INVALID_ID;
    double range = 0.0f;
    switch (m_state)
    {
    case Neutral:
        screenPosition = event.getScreenPosition();
        referencePoint = view.getClosestReferencePoint(screenPosition, m_nMinPickRangePixels);
        if (referencePoint.isValid())
        {
            this->highlightReferencePoint(referencePoint);
        }
        else
        {
            range = view.mapDistanceFromView(m_nPickRangePixels);
            double strictRange = view.mapDistanceFromView(10);
            RMouseEvent::setOriginalMousePos(event.globalPos());
            entityId = documentInterface->getClosestEntity(event.getModelPosition(), range, strictRange, false);
            RMouseEvent::resetOriginalMousePos();
            if (entityId != RObject::INVALID_ID && m_pDocument->isEntityEditable(entityId))
            {
                this->highlightEntity(entityId);
            }
        }
        break;

    case Dragging:
        m_d2Model = event.getModelPosition();
        m_d2Screen = event.getScreenPosition();

        if (!m_d1Screen.equalsFuzzy(m_d2Screen, 10 /*this.minPickRangePixels*/))
        {
            // if the dragging started in an empty space,
            // start box selection:
            setState(SettingCorner2);
            // make sure one mouse move is enough to get a visible preview
            // (for testing, one mouse move event has to be enough):
            this->mouseMoveEvent(event);
        }
        break;

    case SettingCorner2:
        m_d2Model = event.getModelPosition();
        previewSelectionBox(this->getDocumentInterface(),
                            RBox(m_d1Model, m_d2Model),
                            m_d1Model.x > m_d2Model.x);
        break;

        // easy in block entity drag and drop (point mark labels):
        //    case DefaultAction.State.MovingEntityInBlock:
        //        this.moveEntityInBlock(event.getModelPosition(), true);
        //        break;

    default:
        break;
    }
}

void EZSelectEntitiesAction::previewSelectionBox(RDocumentInterface* di, RBox box, bool crossSelection) {
    QList<RVector> points;
    points.push_back(box.c1);
    points.push_back(RVector(box.c1.x, box.c2.y));
    points.push_back(box.c2);
    points.push_back(RVector(box.c2.x, box.c1.y));
    points.push_back(box.c1);

    previewSelectionPolygon(di, points, crossSelection);
}

/**
 * Adds a selection polygon with the given points to the preview.
 * \param points Array of RVector objects
 */
void EZSelectEntitiesAction::previewSelectionPolygon(RDocumentInterface* di, const QList<RVector>& points, bool crossSelection)
{
    RPolyline polygon = RPolyline(points, true);
    RColor color = RSettings::getColor("GraphicsViewColors/SelectionBoxColor", RColor(128,0,0,255));
    RLineweight::Lineweight lw = RLineweight::Weight000;
    RColor c;
    if (crossSelection)
    {
        c = RSettings::getColor("GraphicsViewColors/SelectionBoxBackgroundCrossColor", RColor(0,0,255,30));
    }
    else
    {
        c = RSettings::getColor("GraphicsViewColors/SelectionBoxBackgroundColor", RColor(0,255,0,30));
    }
    // TODO c.getQColor() doesn't work
    QColor bgColor = QColor(c.red(), c.green(), c.blue(), c.alpha());
    QBrush brush = QBrush(bgColor, Qt::SolidPattern);
    if (crossSelection)
    {
        QList<qreal> dashes;
        dashes.append(10.0f);
        dashes.append(5.0f);
        di->addShapeToPreview(polygon, color, brush, lw, Qt::CustomDashLine, dashes);
    }
    else
    {
        // TODO "Qt.SolidLine" doesn't convert to number
        di->addShapeToPreview(polygon, color, brush, lw, Qt::SolidLine);
    }
}

void EZSelectEntitiesAction::mouseReleaseEvent(RMouseEvent& event)
{
    bool persistentSelection = RSettings::getBoolValue("GraphicsView/PersistentSelection", false);

    bool add = false;
    if ((event.modifiers() == Qt::ShiftModifier) || (event.modifiers() == Qt::ControlModifier) || persistentSelection == true)
    {
        add = true;
    }

    RGraphicsView& view = event.getGraphicsView();
    double range = 0.0f;
    double strictRange = 0.0f;
    REntity::Id entityId = REntity::INVALID_ID;
    if (event.button() == Qt::LeftButton)
    {
        switch (m_state)
        {
        case Dragging:
            range = view.mapDistanceFromView(m_nPickRangePixels);
            //range = view.mapDistanceFromView(10);
            strictRange = view.mapDistanceFromView(10);

            entityId = documentInterface->getClosestEntity(event.getModelPosition(), range, strictRange, false);
            //qDebug("entity id: ", entityId);
            if (entityId != REntity::INVALID_ID)
            {
                if (add && m_pDocument->isSelected(entityId))
                {
                    this->deselectEntity(entityId);
                }
                else
                {
                    this->selectEntity(entityId, add);
                }
            }
            else
            {
                if (!add)
                {
                    if (persistentSelection == false)
                    {
                        documentInterface->clearSelection();
                    }
                }
            }
            documentInterface->clearPreview();
            documentInterface->repaintViews();
            this->setState(Neutral);
            break;

        case SettingCorner2:
            documentInterface->clearPreview();
            m_d2Model = event.getModelPosition();

            if ((event.modifiers() == Qt::ShiftModifier) || (event.modifiers() == Qt::ControlModifier) || persistentSelection == true)
            {
                // add all entities in window to the selection:
                documentInterface->selectBoxXY(RBox(m_d1Model, m_d2Model), true);
            }
            else
            {
                // select entities in window only:
                documentInterface->selectBoxXY(RBox(m_d1Model, m_d2Model), false);
            }
            // event.setConsumed(true);
            this->setState(Neutral);
            break;

            // easy in block entity drag and drop (point mark labels):
            //        case DefaultAction.State.MovingEntityInBlock:
            //            this.moveEntityInBlock(event.getModelPosition(), false);
            //            break;

        default:
            break;
        }
    }
    else if (event.button() == Qt::RightButton)
    {
        if (this->m_state != Neutral)// && m_state != MovingEntityInBlock)
        {
            // oxh modify 2015-05-24
            this->escapeEvent();
            //documentInterface->clearPreview();
            //documentInterface->repaintViews();
            //this->setState(Neutral);
        }
        // use right-click into empty area to deselect everything:
        else if (m_state == Neutral && RSettings::getBoolValue("GraphicsView/RightClickToDeselect", false))
        {

            int rightClickRange = RSettings::getIntValue("GraphicsView/RightClickRange", 10);
            //view = event.getGraphicsView();
            range = view.mapDistanceFromView(rightClickRange);
            strictRange = view.mapDistanceFromView(10);

            entityId = documentInterface->getClosestEntity(event.getModelPosition(), range, strictRange, false);
            if (entityId != -1)
            {
                this->selectEntity(entityId, add);

                // TODO: show entity context menu?
                //CadToolBar.back();
            }
            else
            {
                if (documentInterface->hasSelection())
                {
                    documentInterface->clearSelection();
                    documentInterface->clearPreview();
                    documentInterface->repaintViews();
                }
                else
                {
                    // todo
                    //CadToolBar.back();
                }
            }
        }
        else
        {
            // todo
            //CadToolBar.back();
        }

        // oxh add 2015-05-24
        //this->escapeEvent();
    }
}

void EZSelectEntitiesAction::mousePressEvent(RMouseEvent& event)
{
    if (event.button() == Qt::LeftButton && event.modifiers() != Qt::ControlModifier)
    {
        if (m_state == Neutral)
        {
            m_d1Model = event.getModelPosition();
            m_d1Screen = event.getScreenPosition();
            this->setState(Dragging);
            documentInterface->clearPreview();
        }
    }
}

void EZSelectEntitiesAction::mouseDoubleClickEvent(RMouseEvent& event)
{
    if (event.button() == Qt::LeftButton && m_state == Neutral)
    {
        RGraphicsView& view = event.getGraphicsView();
        double range = view.mapDistanceFromView(m_nPickRangePixels);
        double strictRange = view.mapDistanceFromView(10);
        REntity::Id entityId = documentInterface->getClosestEntity(event.getModelPosition(), range, strictRange, false);
        if (entityId == RObject::INVALID_ID)
        {
            return;
        }

        this->entityDoubleClicked(entityId, event);
    }
}

void EZSelectEntitiesAction::escapeEvent()
{
    documentInterface->clearPreview();
    documentInterface->repaintViews();

    // todo
    //CadToolBar.back();
    this->setState(Neutral);

    //this->terminate();
}

void EZSelectEntitiesAction::coordinateEvent(RCoordinateEvent& event)
{
    pickCoordinate(event, false);
}

void EZSelectEntitiesAction::coordinateEventPreview(RCoordinateEvent& event)
{
    pickCoordinate(event, true);
}

void EZSelectEntitiesAction::pickCoordinate(RCoordinateEvent& /*event*/, bool /*preview*/)
{

}

/**
 * Called when the mouse cursor hovers over an entity.
 */
void EZSelectEntitiesAction::highlightEntity(REntity::Id& entityId)
{
    if (!documentInterface)
    {
        return;
    }

    documentInterface->highlightEntity(entityId);
}

/**
 * Called when the mouse cursor hovers over a reference point.
 */
void EZSelectEntitiesAction::highlightReferencePoint(const RVector& referencePoint)
{
    if (!documentInterface)
    {
        return;
    }

    documentInterface->highlightReferencePoint(referencePoint);
}

/**
 * Called when the user deselects a single entity.
 */
void EZSelectEntitiesAction::deselectEntity(REntity::Id& entityId)
{
    if (!documentInterface)
    {
        return;
    }
    documentInterface->deselectEntity(entityId);
}

/**
 * Called when the user selects a single entity.
 */
void EZSelectEntitiesAction::selectEntity(REntity::Id& entityId, bool add)
{
    if (!documentInterface)
    {
        return;
    }
    documentInterface->selectEntity(entityId, add);
}

/**
 * Called when the user selects a single entity.
 */
void EZSelectEntitiesAction::entityDoubleClicked(REntity::Id& entityId, RMouseEvent& event)
{
    if (!documentInterface || !m_pDocument)
    {
        return;
    }

    QSharedPointer<REntity> entity = m_pDocument->queryEntity(entityId);

    if (entity->getType() == RS::EntityText
            || entity->getType() == RS::EntityAttribute
            || entity->getType() == RS::EntityAttributeDefinition)
    {

        if (RSettings::getBoolValue("GraphicsView/DoubleClickEditText", true) == true)
        {
            // oxh todo
            //include("scripts/Modify/EditText/EditText.js");
            //EditText.editText(entity);
        }
    }
    else if (entity->getType() == RS::EntityBlockRef)
    {
        QSharedPointer<RBlockReferenceEntity> blockEntity = entity.dynamicCast<RBlockReferenceEntity>();

        if (!blockEntity.isNull())
        {
            // in block text editing with double-click:
            RBlock::Id blockId = blockEntity->getReferencedBlockId();
            QSharedPointer<RBlock> block = m_pDocument->queryBlock(blockId);
            if (block)
            {
                RGraphicsView& view = event.getGraphicsView();
                double range = view.mapDistanceFromView(m_nPickRangePixels);
                // cursor, mapped to block coordinates:
                RVector pBlock = blockEntity->mapToBlock(event.getModelPosition());
                RBox box = RBox(pBlock - RVector(range,range), pBlock - RVector(range,range));
                QSet<REntity::Id> res = m_pDocument->queryIntersectedEntitiesXY(box, true, false, blockId);
                REntity::Id entityInBlockId;
                if (res.size() == 1)
                {
                    entityInBlockId = *(res.begin());
                }
                else
                {
                    entityInBlockId = m_pDocument->queryClosestXY(res, pBlock, range*2, false);
                }
                QSharedPointer<REntity> entityInBlock = m_pDocument->queryEntity(entityInBlockId);
                if (entityInBlock
                        && entityInBlock->getType() == RS::EntityTextBased
                        && entityInBlock->getCustomProperty("QCAD", "InBlockTextEdit", "0") == "1")
                {
                    // todo
                    //include("scripts/Modify/EditText/EditText.js");
                    //EditText.editText(entityInBlock);
                    return;
                }
            }

            if (RSettings::getBoolValue("GraphicsView/DoubleClickEditBlock", false) == true)
            {
                // todo oxh
                //include("scripts/Block/Block.js");
                //Block.editBlock(documentInterface, blockEntity->getReferencedBlockName());
            }
        }
    }
}

void EZSelectEntitiesAction::keyPressEvent(QKeyEvent& event)
{
    if (event.key() == Qt::Key_Z && event.modifiers() == Qt::ControlModifier)
    {
        documentInterface->undo();
    }
    else if (event.key() == Qt::Key_Z && event.modifiers() == (Qt::ControlModifier + Qt::ShiftModifier))
    {
        documentInterface->redo();
    }
}

void EZSelectEntitiesAction::keyReleaseEvent(QKeyEvent& /*event*/)
{

}
