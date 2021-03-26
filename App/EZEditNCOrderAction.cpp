#include "EZEditNCOrderAction.h"
#include "RDocumentInterface.h"
#include "RDocument.h"
#include "RGraphicsView.h"
#include "RAddObjectsOperation.h"
#include "RSnapAuto.h"
#include "RLayer.h"
#include <QSharedPointer>
#include <QApplication>
#include <QTextCodec>
#include <QMessageBox>
#include "RAddObjectOperation.h"
#include "RDeleteObjectOperation.h"
#include "BoardRectTable.h"
#include "SystemConfigManager.h"
#include "RTextEntity.h"
#include "RTextData.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"
#include "CuttingPointSearcher.h"
#include "SystemConfigManager.h"

EZEditNCOrderAction::EZEditNCOrderAction(const QString& strSourceBoardID, RGuiAction* pGuiAction)
    : m_strSourceBoardID(strSourceBoardID),
      RActionAdapter(pGuiAction),
      m_state(NoState),
      m_nNcOrder(0)
{
}

EZEditNCOrderAction::~EZEditNCOrderAction()
{

}

void EZEditNCOrderAction::beginEvent()
{
    if (!documentInterface)
    {
        return;
    }

    m_nPickRangePixels = RSettings::getPickRange();

    this->setState(SettingPosition);

    documentInterface->setSnap(new RSnapAuto());

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

    RLayer::Id layerId = document.getLayerId("2");

    if (layerId != RLayer::INVALID_ID)
    {
        QSet<REntity::Id> idSet = document.queryLayerEntities(layerId, true);
        RDeleteObjectsOperation* op = new RDeleteObjectsOperation();
        for (QSet<REntity::Id>::iterator it = idSet.begin(); it != idSet.end(); ++it)
        {
            QSharedPointer<RObject> obj = document.queryEntityDirect(*it).dynamicCast<RObject>();
            op->deleteObject(obj);
        }
        documentInterface->applyOperation(op);
        documentInterface->updateAllEntities();
    }

    SystemConfigManager& mgr = SystemConfigManager::GetInstance();
    m_dSourceBoardW = mgr.getSourceMaterialBoardWidth();
    m_dSourceBoardH = mgr.getSourceMaterialBoardHeight();

    m_nBoardSpacing = mgr.getBoardSpacing();

    m_vecBoardRectSt.clear();

    //by OXL start
    m_vecDeleteRectF.clear();
    //by OXL end

    if (BoardRectTable::getBoardRectsBySourceBoardID(m_strSourceBoardID, m_vecBoardRectSt))
    {
        // for test
        /*
        for (std::vector<BoardRectSt>::iterator it = m_vecBoardRectSt.begin(); it != m_vecBoardRectSt.end(); ++it)
        {
            BoardRectSt& st = *it;
            QPointF lt(st.dLtY*m_dSourceBoardH, -st.dLtX*m_dSourceBoardW);
            QPointF rb(st.dRbY*m_dSourceBoardH, -st.dRbX*m_dSourceBoardW);

            QRectF rc(lt, rb);
            RBox box(rc);
            if (box.isValid())
            {

                RAddObjectOperation* op = new RAddObjectOperation();

                RVector lt(st.dLtY*m_dSourceBoardH, -st.dLtX*m_dSourceBoardW, 0);
                RVector lb(st.dRbY*m_dSourceBoardH, -st.dLtX*m_dSourceBoardW, 0);
                RVector rb(st.dRbY*m_dSourceBoardH, -st.dRbX*m_dSourceBoardW, 0);
                RVector rt(st.dLtY*m_dSourceBoardH, -st.dRbX*m_dSourceBoardW, 0);

                QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                            new RPolylineEntity(&document, RPolylineData()));

                polylineEntity->appendVertex(lt);
                polylineEntity->appendVertex(lb);
                polylineEntity->appendVertex(rb);
                polylineEntity->appendVertex(rt);
                polylineEntity->setClosed(true);
                polylineEntity->setColor(RColor(255, 0, 0));
                //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
                polylineEntity->setLineweight(RLineweight::Weight050);

                op->addObject(polylineEntity, false);
                op->apply(document);
            }
        }
        */
    }

    //by OXL start
    //BoardRectSt转换RectF

    for(size_t index = 0; index < m_vecBoardRectSt.size(); index++)
    {
        RectF rcf;
        BoardRectSt brSt = m_vecBoardRectSt.at(index);

        double dHalfSpacing = (double)(m_nBoardSpacing/2.0);

        rcf.y = brSt.dLtX * m_dSourceBoardW + dHalfSpacing;
        rcf.x = brSt.dLtY * m_dSourceBoardH + dHalfSpacing;

        //rcf.y = floor(brSt.dLtX * m_dSourceBoardW + dHalfSpacing + 0.01);
        //rcf.x = floor(brSt.dLtY * m_dSourceBoardH + dHalfSpacing + 0.01);

        rcf.width = (brSt.dRbY * m_dSourceBoardH) - (rcf.x - dHalfSpacing) - m_nBoardSpacing;
        rcf.height = (brSt.dRbX * m_dSourceBoardW) - (rcf.y - dHalfSpacing) - m_nBoardSpacing;
        //rcf.width = floor(rcf.width + 0.01);
        //rcf.height = floor(rcsf.height+0.01);

        m_vecAllRectF.push_back(rcf);
    }
    //by OXL end

    documentInterface->regenerateScenes();

    // “0”层是板件图形层，收集所有多边形实体，在手动排序时用户点击一个，
    // 从m_targetIdSet中移除相应的多边形实体Id，该多边形实体已经被排序
    layerId = document.getLayerId("0");
    m_targetIdSet.clear();
    m_targetIdSet = document.queryLayerEntities(layerId);

    documentInterface->setCurrentLayer("2");
}

void EZEditNCOrderAction::mousePressEvent(RMouseEvent& event)
{
    if (event.button() == Qt::LeftButton)
    {
        if (!documentInterface)
        {
            return;
        }

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        double dCuttingPointOffset = smgr.getObliqueDistance();

        // 重置下刀点的思路：找到距离新点最近的多边形，用多边形的外包矩形寻找在此外包矩形内的多有点并删除，然后重新放置点实体
        RDocument& document = documentInterface->getDocument();

        // 添加板件分割序号
        RLayer::Id layerId = document.getLayerId("2");
        if (layerId != RLayer::INVALID_ID)
        {
            RLayer::Id currentLayerId = document.getCurrentLayerId();

            if (layerId != currentLayerId)
            {
                document.setCurrentLayer(layerId);
            }

            RVector pt = event.getModelPosition();

            for (std::vector<BoardRectSt>::iterator it = m_vecBoardRectSt.begin(); it != m_vecBoardRectSt.end(); ++it)
            {
                BoardRectSt& rectSt = *it;

                RVector c1(rectSt.dLtY*m_dSourceBoardH, -rectSt.dLtX*m_dSourceBoardW);
                RVector c2(rectSt.dRbY*m_dSourceBoardH, -rectSt.dRbX*m_dSourceBoardW);
                RBox box(c1, c2);

                if (box.isValid() && pt.isInside(box))
                {
                    RBox maxBox;

                    QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                    for (QSet<REntity::Id>::iterator iter = idSet.begin(); iter != idSet.end(); ++iter)
                    {
                        if (m_targetIdSet.find(*iter) != m_targetIdSet.end())
                        {
                            QSharedPointer<REntity> e = document.queryEntityDirect(*iter);

                            if (!e.isNull())
                            {
                                RBox rbox = e->getBoundingBox();

                                // 找最外面的那个板件，因为某些异形中空，就有多个实体相互嵌套的情况
                                if (maxBox.isValid())
                                {
                                    if (rbox.contains(maxBox))
                                    {
                                        maxBox = rbox;
                                    }
                                }
                                else
                                {
                                    maxBox = rbox;
                                }

                                m_targetIdSet.remove(*iter);
                                //e->setDrawOrder(++m_nNcOrder);
                            }
                        }
                    }

                    if (maxBox.isValid())
                    {
                        if (!BoardRectTable::updateBoardRectIndexByID(rectSt.strBoardRectID, ++m_nNcOrder))
                        {
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("编辑切割顺序失败."), QMessageBox::Ok);
                            return;
                        }

                        // 添加下刀点
                        //RVector ptCutting = getSuitableCutPoint(maxBox, dCuttingPointOffset);

                        //by OXL start
                        CuttingPointSearcher pointSearcher(dCuttingPointOffset);

                        QRectF qrc(QPointF(maxBox.c1.x, maxBox.c2.y), QPointF(maxBox.c2.x, maxBox.c1.y));
                        RectF tmpRect;
                        tmpRect.x = qrc.x();
                        tmpRect.y = -qrc.y();
                        tmpRect.width = qrc.width();
                        tmpRect.height = -qrc.height();
                        //by OXL end

                        RVector ptCutting = pointSearcher.search(tmpRect, m_vecAllRectF, m_vecDeleteRectF);
                        ptCutting.setY(-ptCutting.y);

                        double dNCStartY =  fabs(ptCutting.x/m_dSourceBoardH);
                        double dNCStartX =  fabs(ptCutting.y/m_dSourceBoardW);

                        if (!BoardRectTable::updateBoardCuttingPointByID(rectSt.strBoardRectID, dNCStartX, dNCStartY))
                        {
                            QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("编辑切割顺序失败."), QMessageBox::Ok);
                            return;
                        }

                        RAddObjectOperation* opAddObject = new RAddObjectOperation();

                        RVector lt(maxBox.c1.x+30, maxBox.c1.y+maxBox.getHeight()-30, 0);

                        RTextData textData(RVector(),
                                           lt,
                                           30.0f,
                                           0.0f,
                                           RS::VAlignMiddle,
                                           RS::HAlignCenter,
                                           RS::LeftToRight,
                                           RS::Exact,
                                           1.0f,
                                           "",
                                           tc->toUnicode("微软雅黑"),
                                           false,
                                           false,
                                           0.0f,
                                           false);

                        QSharedPointer<RTextEntity> textEntity = QSharedPointer<RTextEntity>(new RTextEntity(&document, textData));

                        QString strMsg = QString("%1").arg(m_nNcOrder);
                        textEntity->setText(strMsg);

                        opAddObject->addObject(textEntity, false);

                        RPointData pointData(ptCutting);
                        QSharedPointer<RPointEntity> pointEntity = QSharedPointer<RPointEntity>(new RPointEntity(&document, pointData));

                        opAddObject->addObject(pointEntity, false);

                        opAddObject->apply(document);
                        documentInterface->regenerateScenes();
                    }

                    break;
                }
            }
        }

        if (m_vecDeleteRectF.size() == m_vecAllRectF.size())
        {
            QMessageBox::information(NULL, tc->toUnicode("消息"), tc->toUnicode("编辑切割顺序完成."), QMessageBox::Ok);
        }
    }
}

void EZEditNCOrderAction::mouseReleaseEvent(RMouseEvent& event)
{
    if (event.button() == Qt::RightButton)
    {
        this->escapeEvent();
    }
}

void EZEditNCOrderAction::coordinateEvent(RCoordinateEvent& /*event*/)
{
    //pickCoordinate(event, false);
}

void EZEditNCOrderAction::coordinateEventPreview(RCoordinateEvent& /*event*/)
{
    //pickCoordinate(event, true);
}

void EZEditNCOrderAction::escapeEvent()
{
    this->getDocumentInterface()->clearPreview();
    this->getDocumentInterface()->repaintViews();
    //this->terminate();

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

    RLayer::Id layerId = document.getLayerId("2");

    if (layerId != RLayer::INVALID_ID)
    {
        QSet<REntity::Id> idSet = document.queryLayerEntities(layerId, true);
        RDeleteObjectsOperation* op = new RDeleteObjectsOperation();
        for (QSet<REntity::Id>::iterator it = idSet.begin(); it != idSet.end(); ++it)
        {
            QSharedPointer<RObject> obj = document.queryEntityDirect(*it).dynamicCast<RObject>();
            op->deleteObject(obj);
        }
        documentInterface->applyOperation(op);
        documentInterface->updateAllEntities();
    }

    // “0”层是板件图形层，收集所有多边形实体，在手动排序时用户点击一个，
    // 从m_targetIdSet中移除相应的多边形实体Id，该多边形实体已经被排序
    layerId = document.getLayerId("0");
    m_targetIdSet.clear();
    m_targetIdSet = document.queryLayerEntities(layerId);

    m_nNcOrder = 0;

    documentInterface->setCurrentLayer("2");

    m_vecDeleteRectF.clear();
}

void EZEditNCOrderAction::initState()
{

}


void EZEditNCOrderAction::setState(State state)
{
    this->m_state = state;

    this->initState();

    if (state != -1)
    {
        this->simulateMouseMoveEvent();
    }

    //this->setCrosshairCursor();
    //documentInterface->setClickMode(RAction::PickCoordinate);
    documentInterface->setCursor(Qt::PointingHandCursor);
}

void EZEditNCOrderAction::simulateMouseMoveEvent()
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

void EZEditNCOrderAction::pickCoordinate(RCoordinateEvent& event, bool preview)
{
    if (!documentInterface)
    {
        return;
    }

    m_point = event.getModelPosition();
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

ROperation* EZEditNCOrderAction::getOperation()
{
    if (!m_point.isValid())
    {
        return NULL;
    }

    RAddObjectsOperation* op = new RAddObjectsOperation();
    //op->setText(this->getToolTitle());

    QSharedPointer<RPointEntity> obj = QSharedPointer<RPointEntity>(new RPointEntity(this->getDocument(), RPointData(m_point)));

    op->addObject(obj);

    return op;
}

void EZEditNCOrderAction::updatePreview(bool clear)
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
void EZEditNCOrderAction::applyOperation()
{
    if (!documentInterface)
    {
        return;
    }

    ROperation* op = this->getOperation();
    if (!op)
    {
        qWarning("Operation returned by this.getOperation() is undefined");
        return;
    }
    documentInterface->applyOperation(op);
}

// 用于世界坐标系（即X轴：左负右正；Y轴：上正下负）的第四象限
RVector EZEditNCOrderAction::getSuitableCutPoint(const RBox& box, double dOffset)
{
    QRectF rc(QPointF(box.c1.x, box.c2.y), QPointF(box.c2.x, box.c1.y));

    if(fabs(rc.width()) <= fabs(rc.height()))
    {
        RVector lb(rc.bottomLeft().x(), rc.bottomLeft().y());
        dOffset = dOffset < fabs(rc.height()) ? dOffset : fabs(rc.height())/4.0f;
        lb.setY(lb.y + dOffset);
        return lb;
    }
    else
    {
        RVector lt(rc.topLeft().x(), rc.topLeft().y());
        dOffset = dOffset < fabs(rc.width()) ? dOffset : fabs(rc.width())/4.0f;
        lt.setX(lt.x + dOffset);
        return lt;
    }
}
