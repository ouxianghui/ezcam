#include "ShapeViewer.h"
#include "ui_ShapeViewer.h"
#include <QPainter>
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

#include "RSnapAuto.h"
#include "RSnapCenter.h"
#include "RSnapOnEntity.h"
#include "RSnapFree.h"
#include <QTime>

ShapeViewer::ShapeViewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::ShapeViewer),
    _documentInterface(NULL)
{
    ui->setupUi(this);
    //this->setFrameShape(QFrame::Box);
    init();
}

ShapeViewer::~ShapeViewer()
{
    if (_documentInterface)
    {
        delete _documentInterface;
        _documentInterface = NULL;
    }
    delete ui;
}

void ShapeViewer::init()
{
    // create drawing document with document interface:
    RMemoryStorage* pStorage = new RMemoryStorage();
    RSpatialIndexNavel* pSpatialIndex = new RSpatialIndexNavel();
    RDocument* pDocument = new RDocument(*pStorage, *pSpatialIndex);

    _documentInterface = new RDocumentInterface(*pDocument);

    RGraphicsSceneQt* pScene = new RGraphicsSceneQt(*_documentInterface);
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
        _documentInterface->addCoordinateListener(ui->HorizontalRuler);
        _documentInterface->addCoordinateListener(ui->VerticalRuler);
    }
    else
    {
        ui->HorizontalRuler->setHidden(true);
        ui->VerticalRuler->setHidden(true);
        ui->cornerFrame->setHidden(true);
    }

    //EZDefaultAction* pAction = new EZDefaultAction(NULL);
    //_documentInterface->setDefaultAction(pAction);

    //_documentInterface->setSnap(new RSnapOnEntity);

    EZResetAction* pResetAction = new EZResetAction(NULL);
    _documentInterface->setCurrentAction(pResetAction);

    initEventHandler();
}

void ShapeViewer::initEventHandler()
{
    EZEventHandler* pEventHandler = new EZEventHandler(this, _documentInterface, this);

    if (pEventHandler)
    {
        connect(ui->GraphicsView, &RGraphicsViewQt::viewportChanged, pEventHandler, &EZEventHandler::viewportChanged);
        connect(ui->GraphicsView, &RGraphicsViewQt::updateSnapInfo, pEventHandler, &EZEventHandler::updateSnapInfo);
        connect(ui->GraphicsView, &RGraphicsViewQt::updateTextLabel, pEventHandler, &EZEventHandler::updateTextLabel);

        connect(ui->HorizontalScrollBar, &QScrollBar::valueChanged, pEventHandler, &EZEventHandler::hScrollBarValueChanged);
        connect(ui->VerticalScrollBar, &QScrollBar::valueChanged, pEventHandler, &EZEventHandler::vScrollBarValueChanged);
    }
}

void ShapeViewer::resizeEvent(QResizeEvent* event)
{
    if (_documentInterface)
    {
        ui->GraphicsView->setFactor(1.0f, false);
        //ui->GraphicsView->setOffset(RVector(0.0, -ui->GraphicsView->getHeight()));
        _documentInterface->autoZoom();
        QFrame::resizeEvent(event);
    }
}

void ShapeViewer::importFile(const QString& strFilePathName, const QString& strNameFilter)
{
    try
    {
        _documentInterface->clear();

        // import a DXF file into the drawing:
        if (_documentInterface->importFile(strFilePathName, strNameFilter) != RDocumentInterface::IoErrorNoError)
        {
            return;
        }

        _documentInterface->regenerateScenes();
        _documentInterface->autoZoom();
    }
    catch (QException& e)
    {
        qCritical("ViewPort::importFile() exception: %s", e.what());
    }
}

RDocumentInterface* ShapeViewer::getDocumentInterface()
{
    return _documentInterface;
}
