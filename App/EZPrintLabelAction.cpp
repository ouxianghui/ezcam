#include "EZPrintLabelAction.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTextCodec>
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "SystemConfigManager.h"
#include "REntity.h"
#include "Typedef.h"
#include "MaterialListTable.h"
#include "BoardRectTable.h"
#include "ProductBoardTable.h"
#include "SourceBoardTable.h"
#include "DialogPrintPreviewer.h"

EZPrintLabelAction::EZPrintLabelAction(const QString& strSourceBoardID, RGuiAction* pGuiAction)
    : m_strSourceBoardID(strSourceBoardID),
      RActionAdapter(pGuiAction)
{

}

EZPrintLabelAction::~EZPrintLabelAction()
{

}

void EZPrintLabelAction::beginEvent()
{
    if (!documentInterface)
    {
        this->terminate();
        return;
    }

    std::vector<BoardSt> vecBoardSt;

    RDocument& document = documentInterface->getDocument();

    RLayer::Id layerId = document.getLayerId("0");

    if (layerId != RLayer::INVALID_ID)
    {
        RLayer::Id currentLayerId = document.getCurrentLayerId();

        if (layerId != currentLayerId)
        {
            document.setCurrentLayer(layerId);
        }
    }

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    double dSourceBoardW = smgr.getSourceMaterialBoardWidth();
    double dSourceBoardH = smgr.getSourceMaterialBoardHeight();

    SourceBoardSt sbSt;

    if (SourceBoardTable::getBoardByID(m_strSourceBoardID, sbSt))
    {

    }

    QSet<REntity::Id> targetIdSet;
    std::vector<BoardRectSt> vecBoardRectSt;
    if (BoardRectTable::getBoardRectsBySourceBoardID(m_strSourceBoardID, vecBoardRectSt))
    {
        std::sort(vecBoardRectSt.begin(), vecBoardRectSt.end());

        targetIdSet.clear();
        targetIdSet = document.querySelectedEntities();

        for (std::vector<BoardRectSt>::iterator it = vecBoardRectSt.begin(); it != vecBoardRectSt.end(); ++it)
        {
            BoardRectSt& rectSt = *it;

            RVector c1(rectSt.dLtY*dSourceBoardH, -rectSt.dLtX*dSourceBoardW);
            RVector c2(rectSt.dRbY*dSourceBoardH, -rectSt.dRbX*dSourceBoardW);
            RBox box(c1, c2);

            if (box.isValid())
            {
                RBox maxBox;

                QSet<REntity::Id> idSet = document.queryContainedEntitiesXY(box);
                for (QSet<REntity::Id>::iterator iter = idSet.begin(); iter != idSet.end(); ++iter)
                {
                    if (targetIdSet.find(*iter) != targetIdSet.end())
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

                            targetIdSet.remove(*iter);
                        }
                    }
                }

                if (maxBox.isValid())
                {
                    BoardSt boardSt;
                    boardSt.nSourceBoardIndex = sbSt.nIdx;
                    boardSt.nBoardIndex = rectSt.nIdx;
                    if (ProductBoardTable::getBoardByBarcodeNo(rectSt.strBarcodeNo, boardSt.dataSt))
                    {
                        vecBoardSt.push_back(boardSt);
                    }
                }
            }
        }

        if (vecBoardSt.size() > 0)
        {
            DialogPrintPreviewer dlg(vecBoardSt);
            if (QDialog::Accepted == dlg.exec())
            {
            }
        }
        else
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            QMessageBox::information(NULL, tc->toUnicode("信息"), tc->toUnicode("没有选中任何板件."), QMessageBox::Ok);
        }

        /*
        // for test
        for (std::vector<BoardRectSt>::iterator it = vecBoardRectSt.begin(); it != vecBoardRectSt.end(); ++it)
        {
            BoardRectSt& st = *it;
            QPointF lt(st.dLtY*dSourceBoardH, -st.dLtX*dSourceBoardW);
            QPointF rb(st.dRbY*dSourceBoardH, -st.dRbX*dSourceBoardW);

            QRectF rc(lt, rb);
            RBox box(rc);
            if (box.isValid())
            {

                RAddObjectOperation* op = new RAddObjectOperation();

                RVector lt(st.dLtY*dSourceBoardH, -st.dLtX*dSourceBoardW, 0);
                RVector lb(st.dRbY*dSourceBoardH, -st.dLtX*dSourceBoardW, 0);
                RVector rb(st.dRbY*dSourceBoardH, -st.dRbX*dSourceBoardW, 0);
                RVector rt(st.dLtY*dSourceBoardH, -st.dRbX*dSourceBoardW, 0);

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

    this->terminate();
}
