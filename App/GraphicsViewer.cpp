#include "GraphicsViewer.h"
#include "RFileImporter.h"
#include "RFileExporterRegistry.h"
#include "RFileImporterRegistry.h"

GraphicsViewer::GraphicsViewer(QWidget *parent)
    : QFrame(parent),
      m_pDocument(NULL)
{
}

GraphicsViewer::~GraphicsViewer()
{
    if (m_pDocument)
    {
        delete m_pDocument;
        m_pDocument = NULL;
    }
}

GraphicsViewer::IoErrorCode GraphicsViewer::importFile(const QString& strFilePathName, const QString& strNameFilter)
{
    QFileInfo fi(strFilePathName);

    if (fi.size() == 0)
    {
        return GraphicsViewer::IoErrorZeroSize;
    }

    if (!fi.exists())
    {
        return GraphicsViewer::IoErrorNotFound;
    }

    if (!fi.isReadable())
    {
        return GraphicsViewer::IoErrorPermission;
    }


    if (m_pDocument)
    {
        delete m_pDocument;
        m_pDocument = NULL;
    }

    RMemoryStorage* pStorage = new RMemoryStorage();
    RSpatialIndexNavel* pSpatialIndex = new RSpatialIndexNavel();
    m_pDocument = new RDocument(*pStorage, *pSpatialIndex);

    RFileImporter* pFileImporter = RFileImporterRegistry::getFileImporter(strFilePathName, strNameFilter, *m_pDocument);
    if (pFileImporter == NULL)
    {
        qWarning() << QString("No file importer found for file %1").arg(strFilePathName);
        return GraphicsViewer::IoErrorNoImporterFound;
    }

    GraphicsViewer::IoErrorCode ret = GraphicsViewer::IoErrorNoError;
    QString strPreviousFileName = m_pDocument->getFileName();
    m_pDocument->setFileName(strFilePathName);

    if (pFileImporter->importFile(strFilePathName, strNameFilter))
    {
        m_pDocument->setModified(false);
    }
    else
    {
        m_pDocument->setFileName(strPreviousFileName);
        qWarning() << QString("Import for file %1 failed").arg(strFilePathName);
        ret = GraphicsViewer::IoErrorGeneralImportError;
    }

    delete pFileImporter;
    pFileImporter = NULL;

    QSet<REntity::Id> entityIdSet =  m_pDocument->queryAllEntities();
    for (QSet<REntity::Id>::iterator it = entityIdSet.begin(); it != entityIdSet.end(); ++it)
    {
        REntity::Id& id = (REntity::Id)*it;
        QSharedPointer<REntity> entity = m_pDocument->queryEntity(id);
        if (entity->getType() == RS::EntityPolyline)
        {
            std::cout << "RS::EntityPolyline" << std::endl;
        }
    }


    return ret;
}

void GraphicsViewer::paintEvent(QPaintEvent* event)
{
    if (m_pDocument)
    {
        QSet<REntity::Id> entityIdSet =  m_pDocument->queryAllEntities();
        for (QSet<REntity::Id>::iterator it = entityIdSet.begin(); it != entityIdSet.end(); ++it)
        {
            REntity::Id& id = (REntity::Id)*it;
            QSharedPointer<REntity> entity = m_pDocument->queryEntity(id);
            if (entity->getType() == RS::EntityPolyline)
            {

            }
        }

    }
    QFrame::paintEvent(event);
}

void GraphicsViewer::mousePressEvent(QMouseEvent* event)
{
    QFrame::mousePressEvent(event);
}

void GraphicsViewer::mouseReleaseEvent(QMouseEvent* event)
{
    QFrame::mouseReleaseEvent(event);
}

void GraphicsViewer::resizeEvent(QResizeEvent* event)
{
    QFrame::resizeEvent(event);
}

