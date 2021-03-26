#ifndef GRAPHICSVIEWER_H
#define GRAPHICSVIEWER_H

#include <QFrame>
#include "RAddObjectOperation.h"
#include "RLineEntity.h"
#include "RScriptHandlerRegistry.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"
#include "RDocument.h"
#include "RPainterPath.h"

class GraphicsViewer : public QFrame
{
    Q_OBJECT

    enum IoErrorCode {
        /**
         * No error occurred during import or export.
         */
        IoErrorNoError,
        /**
         * No suitable importer was found.
         */
        IoErrorNoImporterFound,
        /**
         * File size is zero bytes.
         */
        IoErrorZeroSize,
        /**
         * File not found.
         */
        IoErrorNotFound,
        /**
         * No permissions to read file.
         */
        IoErrorPermission,
        /**
         * General import error, i.e. non of the errors above.
         */
        IoErrorGeneralImportError,
        /**
         * General import error if an URL was given, i.e. non of the errors above.
         */
        IoErrorGeneralImportUrlError
    };
public:
    explicit GraphicsViewer(QWidget *parent = 0);
    ~GraphicsViewer();

    GraphicsViewer::IoErrorCode importFile(const QString& strFilePathName, const QString& strNameFilter = "");

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);
    void resizeEvent(QResizeEvent* event);

signals:
public slots:

private:
    RDocument* m_pDocument;
    RPainterPath m_pDxfEntitiesPath;
    RPainterPath m_pUserGraphicsPath;
};

#endif // GRAPHICSVIEWER_H
