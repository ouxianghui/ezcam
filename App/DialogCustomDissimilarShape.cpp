#include "DialogCustomDissimilarShape.h"
#include "ui_DialogCustomDissimilarShape.h"
#include <QTextCodec>
#include <QFontMetrics>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QAbstractButton>
#include <QPushButton>
#include <QMessageBox>
#include <QFileDialog>
#include "RArc.h"
#include "RArcEntity.h"
#include "RLine.h"
#include "RLineEntity.h"
#include "RCircle.h"
#include "RCircleEntity.h"
#include "RPolyline.h"
#include "RPolylineEntity.h"
#include "RDirected.h"
#include "RDocumentInterface.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"
#include "RAddObjectOperation.h"
#include "RDxfExporter.h"
#include <QSet>
#include <vector>
#include "EG.h"
#include "RShape.h"
#include "SegmentsCreator.h"
#include "ProjectConfigManager.h"
#include "Global.h"


DialogCustomDissimilarShape::DialogCustomDissimilarShape(const QString& barcodeNo,
                                                         const QString& productName,
                                                         double width,
                                                         double height,
                                                         bool editExisting,
                                                         QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCustomDissimilarShape),
    _barcodeNo(barcodeNo),
    _productName(productName),
    _width(width),
    _height(height)
{
    ui->setupUi(this);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString title = tc->toUnicode("编辑异形");
    this->setWindowTitle(title + " - " + _productName + QString(" - %1 x %2").arg(_width).arg(_height));

    createWidgets();

    // 为自定义异形，导入相应的dxf文件
    if (editExisting)
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QString projectPath = mgr.getProjectPath();

        QString fileName = projectPath + QString("/shape/%1.dxf").arg(_barcodeNo);

        _shapeViewer->importFile(fileName);
        _shapeEditor->reinitialization();
    }

    this->drawOutline();
}

DialogCustomDissimilarShape::~DialogCustomDissimilarShape()
{
    delete ui;
}

void DialogCustomDissimilarShape::createWidgets()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    _shapeViewer = new ShapeViewer(this);

    _documentInterface = _shapeViewer->getDocumentInterface();

    _shapeEditor = new ShapeEditorTableWidget(_width, _height, _documentInterface, this);

    _buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(_buttonBox, &QDialogButtonBox::accepted, this, &DialogCustomDissimilarShape::acceptButtonClicked);
    connect(_buttonBox, &QDialogButtonBox::rejected, this, &DialogCustomDissimilarShape::rejectButtonClicked);

    QPushButton* importDxfButton = new QPushButton(this);
    importDxfButton->setText(tc->toUnicode("导入"));
    connect(importDxfButton, &QPushButton::clicked, this, &DialogCustomDissimilarShape::onImportDxfButtonClicked);

    QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
    this->setLayout(vBoxLayout);

    QHBoxLayout* hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(_shapeViewer, 5);
    hBoxLayout->addWidget(_shapeEditor, 2);

    QHBoxLayout* layout = new QHBoxLayout();
    layout->addWidget(importDxfButton, 1, Qt::AlignLeft);
    layout->addWidget(_buttonBox, 1, Qt::AlignRight);
    vBoxLayout->addLayout(hBoxLayout, 9);
    vBoxLayout->addLayout(layout, 1);
}

void DialogCustomDissimilarShape::drawOutline()
{
    RDocument& document = _documentInterface->getDocument();

    QString layerName = document.getCurrentLayerName();

    if (!document.hasLayer("1"))
    {
        QSharedPointer<RLayer> pLayer = QSharedPointer<RLayer>(new RLayer(&document, "1"));
        //pLayer->setLinetypeId(document.getLinetypeId("DASHED"));
        pLayer->setLineweight(RLineweight::Weight100);
        RAddObjectOperation* op = new RAddObjectOperation();
        op->addObject(pLayer);
        _documentInterface->applyOperation(op);
    }

    _documentInterface->setCurrentLayer("1");

    RPolyline polyline;

    polyline.appendVertex(RVector(0.0, 0.0));
    polyline.appendVertex(RVector(_width, 0.0));
    polyline.appendVertex(RVector(_width, _height));
    polyline.appendVertex(RVector(0.0, _height));

    polyline.setClosed(true);

    polyline.move(RVector(0.0, 0.0));

    QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                new RPolylineEntity(&_documentInterface->getDocument(), RPolylineData(polyline)));
    polylineEntity->setColor(RColor(255, 255, 255));
    polylineEntity->setLinetypeId(_documentInterface->getDocument().getLinetypeId("DASHED"));
    polylineEntity->setLineweight(RLineweight::Weight050);

    RAddObjectOperation* op = new RAddObjectOperation(polylineEntity, false);

    _documentInterface->applyOperation(op);

    _documentInterface->setCurrentLayer(layerName);
}


QString DialogCustomDissimilarShape::getShape()
{
    return _shapeId;
}

void DialogCustomDissimilarShape::acceptButtonClicked()
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

    if (mgr.isProjectOpened())
    {
        if (!_documentInterface)
        {
            return;
        }

        QList<REntity::Id> segmentIds;

        QList<REntity::Id> circleIds;

        QList<REntity::Id> lineIds;

        QList<REntity::Id> arcIds;

        RDocument& doc = _documentInterface->getDocument();

        RLayer::Id layerId = doc.getCurrentLayerId();

        QList<REntity::Id> ids = doc.queryLayerEntities(layerId).toList();

        for (int i = 0; i < ids.size(); ++i)
        {
            const REntity::Id& id = ids.at(i);

            QSharedPointer<REntity> entity = doc.queryEntity(id);

            // 选取所有的圆形
            if (entity->getType() == RS::EntityCircle)
            {
                circleIds.push_back(id);
            }
            else if (entity->getType() == RS::EntityLine)
            {
                QSharedPointer<RLineEntity> line = entity.dynamicCast<RLineEntity>();
                if (line->getColor() == SLOT_COLOR)
                {
                    lineIds.push_back(id);
                }
                else
                {
                    segmentIds.push_back(id);
                }
            }
            else if (entity->getType() == RS::EntityArc)
            {
                QSharedPointer<RArcEntity> arc = entity.dynamicCast<RArcEntity>();
                if (arc->getColor() == SLOT_COLOR)
                {
                    arcIds.push_back(id);
                }
                else
                {
                    segmentIds.push_back(id);
                }
            }
        }

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        SegmentsCreator creator(doc);

        QList<QList<QSharedPointer<RShape>>> lsSegments = creator.excractSegments(segmentIds);


        int counter = 0;
        for (QList<QList<QSharedPointer<RShape> > >::iterator it = lsSegments.begin(); it != lsSegments.end(); ++it)
        {
            QList<QSharedPointer<RShape> >& segments = *it;
            counter += segments.length();
        }

        if (segmentIds.length() != counter)
        {
            QMessageBox::information(this, tc->toUnicode("消息"), tc->toUnicode("部分线条无法用于创建多边形，请检查。"), tc->toUnicode("知道了"));
            return;
        }

        if (lsSegments.size() == 0
                &&circleIds.size() == 0
                && lineIds.size() == 0
                && arcIds.size() == 0)
        {
            _shapeId = "";
            accept();
            return;
        }

        RMemoryStorage* storage = new RMemoryStorage();
        RSpatialIndexNavel* spatialIndex = new RSpatialIndexNavel();
        RDocument* document = new RDocument(*storage, *spatialIndex);

        RAddObjectOperation* op = new RAddObjectOperation();

        for (QList<REntity::Id>::iterator it = circleIds.begin(); it != circleIds.end(); ++it)
        {
            QSharedPointer<RCircleEntity> circle = doc.queryEntity(*it).dynamicCast<RCircleEntity>();
            QSharedPointer<RCircleEntity> newCircle = QSharedPointer<RCircleEntity>(new RCircleEntity(document, RCircleData(circle->getData())));
            newCircle->setColor(circle->getColor());
            newCircle->setLinetypeId(circle->getLinetypeId());
            newCircle->setLineweight(circle->getLineweight());
            op->addObject(newCircle, false);
        }

        for (QList<REntity::Id>::iterator it = lineIds.begin(); it != lineIds.end(); ++it)
        {
            QSharedPointer<RLineEntity> line = doc.queryEntity(*it).dynamicCast<RLineEntity>();
            QSharedPointer<RLineEntity> newLine = QSharedPointer<RLineEntity>(new RLineEntity(document, RLineData(line->getData())));
            newLine->setColor(line->getColor());
            newLine->setLinetypeId(line->getLinetypeId());
            newLine->setLineweight(line->getLineweight());
            op->addObject(newLine, false);
        }

        for (QList<REntity::Id>::iterator it = arcIds.begin(); it != arcIds.end(); ++it)
        {
            QSharedPointer<RArcEntity> arc = doc.queryEntity(*it).dynamicCast<RArcEntity>();

            QSharedPointer<RArcEntity> newArc = QSharedPointer<RArcEntity>(new RArcEntity(document, RArcData(arc->getData())));
            newArc->setColor(arc->getColor());
            newArc->setLinetypeId(arc->getLinetypeId());
            newArc->setLineweight(arc->getLineweight());
            op->addObject(newArc, false);
        }

        for (QList<QList<QSharedPointer<RShape> > >::iterator it = lsSegments.begin(); it != lsSegments.end(); ++it)
        {
            QList<QSharedPointer<RShape> >& segments = *it;

            RPolyline polyline(segments);

            QSharedPointer<RPolylineEntity> entity = QSharedPointer<RPolylineEntity>(new RPolylineEntity(document, RPolylineData(polyline)));

            entity->setClosed(true);
            entity->setColor(RColor(0, 128, 255));
            entity->setLinetypeId(document->getLinetypeId("continuous"));
            entity->setLineweight(RLineweight::Weight050);

            op->addObject(entity, false);
        }

        op->apply(*document);

        QString projectPath = mgr.getProjectPath();

        QString fileName = projectPath + QString("/shape/%1.dxf").arg(_barcodeNo);

        RDxfExporter* dxfExporter = new RDxfExporter(*document);
        dxfExporter->exportFile(fileName, "");
        document->setFileName(fileName);
        dxfExporter->exportDocument();

        delete dxfExporter;
        dxfExporter = NULL;

        delete document;
        document = NULL;
    }

    _shapeId = "ID=-1;";
    accept();
}

void DialogCustomDissimilarShape::rejectButtonClicked()
{
    reject();
}

void DialogCustomDissimilarShape::onImportDxfButtonClicked(bool /*checked*/)
{
    QString fileName = QFileDialog::getOpenFileName(this, "", "*.dxf");

    if (!fileName.isEmpty() && _documentInterface)
    {
        _documentInterface->clear();

        RVector center(_width/2.0, _height/2.0, 0);

        EG::copyShapes2Document(fileName, center, 0.0, false, false, _documentInterface);

        _documentInterface->regenerateScenes();
        _documentInterface->autoZoom();

        _shapeEditor->reinitialization();

        this->drawOutline();
    }
}
