#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QFrame>
#include <QCursor>
#include <QDragEnterEvent>
#include "RDocumentInterface.h"
#include "SourceBoardTable.h"

class RDocument;
class RDocumentInterface;

namespace Ui {
class ViewPort;
}

class ViewPort : public QFrame
{
    Q_OBJECT

public:
    explicit ViewPort(QWidget *parent = 0);
    ~ViewPort();
    void importFile(const QString& strFilePathName, const QString& strNameFilter, const QString& strSourceBoardID);

    bool isDocumentModified();
    void saveDocument();

protected:
    void init();
    void initEventHandler();

protected:
    void resizeEvent(QResizeEvent* event);

public slots:
    void resetActionToolButtonClicked(bool checked = false);
    void hidenInformationLayerToolButtonClicked(bool checked = false);
    void hidenCuttingPointLayerToolButtonClicked(bool checked = false);
    void modifyCuttingPointToolButtonClicked(bool checked = false);
    void editNCOrderToolButtonClicked(bool checked = false);
    void selectBoardToolButtonClicked(bool checked = false);
    void resetViewportToolButtonClicked(bool checked = false);

    void saveCurrentDocument();
    void saveCurrentDocumentAs();
    void replaceCurrentDocument();
    void exportCurrentDocument2Bitmap();
    void exportCurrentDocument2NC();
    void printSelectedBoardLabel();

private:
    Ui::ViewPort *ui;
    RDocumentInterface* m_pDocumentInterface;
    QString m_strSourceBoardID;
};

#endif // VIEWPORT_H
