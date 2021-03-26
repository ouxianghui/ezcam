#include "ViewPort.h"
#include "ui_ViewPort.h"
#include <QMessageBox>
#include <QTextCodec>
#include <QException>
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsSceneQt.h"
#include "ROrthoGrid.h"
#include "RGrid.h"
#include "RNavigationAction.h"
#include "EZNavigationAction.h"
#include "EZEventHandler.h"
#include "EZDefaultAction.h"
#include "EZResetAction.h"
#include "EZModifyCuttingPointAction.h"
#include "EZEditNCOrderAction.h"
#include "EZSelectEntitiesAction.h"
#include "EZDocumentSaveAction.h"
#include "EZDocumentSaveAsAction.h"
#include "EZReplaceDocumentAction.h"
#include "EZExportBitmapAction.h"
#include "EZExportNCAction.h"
#include "EZPrintLabelAction.h"
#include "RSnapAuto.h"
#include "RSnapCenter.h"
#include "RSnapOnEntity.h"
#include "RSnapFree.h"
#include <QTime>


ViewPort::ViewPort(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ViewPort),
    m_pDocumentInterface(NULL)
{
    ui->setupUi(this);
    init();
}

ViewPort::~ViewPort()
{
    if (m_pDocumentInterface)
    {
        delete m_pDocumentInterface;
        m_pDocumentInterface = NULL;
    }
    delete ui;
}

void ViewPort::importFile(const QString& strFilePathName, const QString& strNameFilter, const QString& strSourceBoardID)
{
    try
    {
        QTime t;
        t.start();
        RDocument& document = m_pDocumentInterface->getDocument();
        if (document.isModified())
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            if (QMessageBox::Yes == QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("文件已经被修改，是否保存修改？"), QMessageBox::Yes, QMessageBox::No))
            {
                saveCurrentDocument();
            }
        }

        m_strSourceBoardID = strSourceBoardID;

        m_pDocumentInterface->clear();

        qDebug(QString("t = %1\n").arg(t.elapsed()).toStdString().c_str());

        // import a DXF file into the drawing:
        if (m_pDocumentInterface->importFile(strFilePathName, strNameFilter) != RDocumentInterface::IoErrorNoError)
        {
            return;
        }

        qDebug(QString("t = %1\n").arg(t.elapsed()).toStdString().c_str());

        m_pDocumentInterface->regenerateScenes();
        m_pDocumentInterface->autoZoom();

        qDebug(QString("t = %1\n").arg(t.elapsed()).toStdString().c_str());

        //m_pDocumentInterface->regenerateViews();
        //m_pDocumentInterface->repaintViews();
        //ui->GraphicsView->viewportChangeEvent();

        EZResetAction* pResetAction = new EZResetAction(NULL);
        m_pDocumentInterface->setCurrentAction(pResetAction);
    }
    catch (QException& e)
    {
        qCritical("ViewPort::importFile() exception: %s", e.what());
    }
}

bool ViewPort::isDocumentModified()
{
    RDocument& document = m_pDocumentInterface->getDocument();
    if (document.isModified())
    {
        return true;
    }
    return false;
}

void ViewPort::saveDocument()
{
    RDocument& document = m_pDocumentInterface->getDocument();
    if (document.isModified())
    {
        saveCurrentDocument();
    }
}

void ViewPort::init()
{

    // create drawing document with document interface:
    RMemoryStorage* pStorage = new RMemoryStorage();
    RSpatialIndexNavel* pSpatialIndex = new RSpatialIndexNavel();
    RDocument* pDocument = new RDocument(*pStorage, *pSpatialIndex);

    m_pDocumentInterface = new RDocumentInterface(*pDocument);

    RGraphicsSceneQt* pScene = new RGraphicsSceneQt(*m_pDocumentInterface);
    if (RSettings::getBoolValue("GraphicsView/AutoSwitchLinetypes", false) == true)
    {
        pScene->setScreenBasedLinetypes(true);
    }
    ui->GraphicsView->setScene(pScene, true);

    ui->GraphicsView->setViewportNumber(1);
    ui->GraphicsView->setAntialiasing(RSettings::getBoolValue("GraphicsView/Antialiasing", true));

    ui->GraphicsView->setFocusFrameWidget(this);

    EZNavigationAction* pNavigationAction = new EZNavigationAction(this);
    pNavigationAction->setGraphicsView(ui->GraphicsView);
    ui->GraphicsView->setNavigationAction(pNavigationAction);

    ROrthoGrid* pGrid = new ROrthoGrid(*ui->GraphicsView);
    ui->GraphicsView->setGrid(pGrid);
    ui->GraphicsView->setFocus();

    ui->HorizontalScrollBar->setSingleStep(50);
    ui->VerticalScrollBar->setSingleStep(50);

    if (RSettings::getBoolValue("GraphicsView/ShowScrollbars", true) == false)
    {
        ui->InfoLabel->setHidden(true);
        ui->HorizontalScrollBar->setHidden(true);
        ui->VerticalScrollBar->setHidden(true);
    }

    if (RSettings::getBoolValue("GraphicsView/ShowRulers", true))
    {
        ui->HorizontalRuler->setGraphicsView(ui->GraphicsView);
        ui->VerticalRuler->setGraphicsView(ui->GraphicsView);
        m_pDocumentInterface->addCoordinateListener(ui->HorizontalRuler);
        m_pDocumentInterface->addCoordinateListener(ui->VerticalRuler);
    }
    else
    {
        ui->HorizontalRuler->setHidden(true);
        ui->VerticalRuler->setHidden(true);
        ui->cornerFrame->setHidden(true);
    }


    //EZDefaultAction* pAction = new EZDefaultAction(NULL);
    //m_pDocumentInterface->setDefaultAction(pAction);

    //m_pDocumentInterface->setSnap(new RSnapOnEntity);

    EZResetAction* pResetAction = new EZResetAction(NULL);
    m_pDocumentInterface->setCurrentAction(pResetAction);

    initEventHandler();
}

void ViewPort::initEventHandler()
{
    EZEventHandler* pEventHandler = new EZEventHandler(this, m_pDocumentInterface, this);

    if (pEventHandler)
    {
        connect(ui->GraphicsView, &RGraphicsViewQt::viewportChanged, pEventHandler, &EZEventHandler::viewportChanged);
        connect(ui->GraphicsView, &RGraphicsViewQt::updateSnapInfo, pEventHandler, &EZEventHandler::updateSnapInfo);
        connect(ui->GraphicsView, &RGraphicsViewQt::updateTextLabel, pEventHandler, &EZEventHandler::updateTextLabel);

        connect(ui->HorizontalScrollBar, &QScrollBar::valueChanged, pEventHandler, &EZEventHandler::hScrollBarValueChanged);
        connect(ui->VerticalScrollBar, &QScrollBar::valueChanged, pEventHandler, &EZEventHandler::vScrollBarValueChanged);
    }
}

void ViewPort::resizeEvent(QResizeEvent* event)
{
    if (m_pDocumentInterface)
    {
        ui->GraphicsView->setFactor(1.0f, false);
        //ui->GraphicsView->setOffset(RVector(0.0, -ui->GraphicsView->getHeight()));
        m_pDocumentInterface->autoZoom();
        QFrame::resizeEvent(event);
    }
}

void ViewPort::resetActionToolButtonClicked(bool /*checked*/)
{
    EZResetAction* pResetAction = new EZResetAction(NULL);
    m_pDocumentInterface->setCurrentAction(pResetAction);
}

void ViewPort::hidenInformationLayerToolButtonClicked(bool /*checked*/)
{
    RDocument& document = m_pDocumentInterface->getDocument();
    RLayer::Id layerId =  document.getLayerId("1");
    if (layerId != RLayer::INVALID_ID)
    {
        QSharedPointer<RLayer> layer1 = document.queryLayerDirect(layerId);
        if (!layer1.isNull())
        {
            if (layer1->isFrozen())
            {
                layer1->setFrozen(false);
            }
            else
            {
                layer1->setFrozen(true);
            }
            m_pDocumentInterface->regenerateScenes();
        }
    }
}

void ViewPort::hidenCuttingPointLayerToolButtonClicked(bool /*checked*/)
{
    RDocument& document = m_pDocumentInterface->getDocument();
    RLayer::Id layerId =  document.getLayerId("2");
    if (layerId != RLayer::INVALID_ID)
    {
        QSharedPointer<RLayer> layer2 = document.queryLayerDirect(layerId);
        if (!layer2.isNull())
        {
            if (layer2->isFrozen())
            {
                layer2->setFrozen(false);
            }
            else
            {
                layer2->setFrozen(true);
            }
            m_pDocumentInterface->regenerateScenes();
        }
    }
}

void ViewPort::modifyCuttingPointToolButtonClicked(bool checked)
{
    if (checked)
    {
        RDocument& document = m_pDocumentInterface->getDocument();
        RLayer::Id id = document.getLayerId("0");
        QSharedPointer<RLayer> layer0 = document.queryLayerDirect(id);
        if (!layer0.isNull())
        {
            if (layer0->isLocked())
            {
                layer0->setLocked(false);
            }
        }

        id = document.getLayerId("1");
        QSharedPointer<RLayer> layer1 = document.queryLayerDirect(id);
        if (!layer1.isNull())
        {
            if (!layer1->isLocked())
            {
                layer1->setLocked(true);
            }
        }

        id = document.getLayerId("2");
        QSharedPointer<RLayer> layer2 = document.queryLayerDirect(id);
        if (!layer2.isNull())
        {
            if (layer2->isLocked())
            {
                layer2->setLocked(false);
            }

            if (layer2->isFrozen())
            {
                layer2->setFrozen(false);
                m_pDocumentInterface->regenerateScenes();
            }
        }

        EZModifyCuttingPointAction* pAction = new EZModifyCuttingPointAction(m_strSourceBoardID, NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
    else
    {
        EZResetAction* pResetAction = new EZResetAction(NULL);
        m_pDocumentInterface->setCurrentAction(pResetAction);
    }
}

void ViewPort::editNCOrderToolButtonClicked(bool checked)
{
    if (checked)
    {
        RDocument& document = m_pDocumentInterface->getDocument();
        RLayer::Id id = document.getLayerId("0");
        QSharedPointer<RLayer> layer0 = document.queryLayerDirect(id);
        if (!layer0.isNull())
        {
            if (layer0->isFrozen())
            {
                layer0->setFrozen(false);
            }
            if (!layer0->isLocked())
            {
                layer0->setLocked(true);
            }
        }

        id = document.getLayerId("1");
        QSharedPointer<RLayer> layer1 = document.queryLayerDirect(id);
        if (!layer1.isNull())
        {
            if (!layer1->isLocked())
            {
                layer1->setLocked(true);
            }
        }

        id = document.getLayerId("2");
        QSharedPointer<RLayer> layer2 = document.queryLayerDirect(id);
        if (!layer2.isNull())
        {
            if (layer2->isLocked())
            {
                layer2->setLocked(false);
            }
        }

        EZEditNCOrderAction* pAction = new EZEditNCOrderAction(m_strSourceBoardID, NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
    else
    {
        EZResetAction* pResetAction = new EZResetAction(NULL);
        m_pDocumentInterface->setCurrentAction(pResetAction);
    }
}

void ViewPort::selectBoardToolButtonClicked(bool checked)
{
    if (checked)
    {
        RDocument& document = m_pDocumentInterface->getDocument();
        RLayer::Id id = document.getLayerId("0");
        QSharedPointer<RLayer> layer0 = document.queryLayerDirect(id);
        if (!layer0.isNull())
        {
            if (layer0->isFrozen())
            {
                layer0->setFrozen(false);
            }
            if (layer0->isLocked())
            {
                layer0->setLocked(false);
            }
        }

        id = document.getLayerId("1");
        QSharedPointer<RLayer> layer1 = document.queryLayerDirect(id);
        if (!layer1.isNull())
        {
            if (!layer1->isLocked())
            {
                layer1->setLocked(true);
            }
        }

        id = document.getLayerId("2");
        QSharedPointer<RLayer> layer2 = document.queryLayerDirect(id);
        if (!layer2.isNull())
        {
            if (!layer2->isLocked())
            {
                layer2->setLocked(true);
            }
        }

        EZSelectEntitiesAction* pAction = new EZSelectEntitiesAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
    else
    {
        EZResetAction* pAction = new EZResetAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}

void ViewPort::resetViewportToolButtonClicked(bool /*checked*/)
{
    if (m_pDocumentInterface)
    {
        m_pDocumentInterface->autoZoom();
    }
}

void ViewPort::saveCurrentDocument()
{
    if (m_pDocumentInterface)
    {
        EZDocumentSaveAction* pAction = new EZDocumentSaveAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}

void ViewPort::saveCurrentDocumentAs()
{
    if (m_pDocumentInterface)
    {
        EZDocumentSaveAsAction* pAction = new EZDocumentSaveAsAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}

void ViewPort::replaceCurrentDocument()
{
    if (m_pDocumentInterface)
    {
        EZReplaceDocumentAction* pAction = new EZReplaceDocumentAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}

void ViewPort::exportCurrentDocument2Bitmap()
{
    if (m_pDocumentInterface)
    {
        EZExportBitmapAction* pAction = new EZExportBitmapAction(NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}

void ViewPort::exportCurrentDocument2NC()
{
    if (m_pDocumentInterface)
    {
        RDocument& document = m_pDocumentInterface->getDocument();
        RLayer::Id id = document.getLayerId("0");
        QSharedPointer<RLayer> layer0 = document.queryLayerDirect(id);
        if (!layer0.isNull())
        {
            if (layer0->isFrozen())
            {
                layer0->setFrozen(false);
            }
            if (layer0->isLocked())
            {
                layer0->setLocked(false);
            }

            EZExportNCAction* pAction = new EZExportNCAction(m_strSourceBoardID, NULL);
            m_pDocumentInterface->setCurrentAction(pAction);
        }
    }
}

void ViewPort::printSelectedBoardLabel()
{
    if (m_pDocumentInterface)
    {
        EZPrintLabelAction* pAction = new EZPrintLabelAction(m_strSourceBoardID, NULL);
        m_pDocumentInterface->setCurrentAction(pAction);
    }
}
