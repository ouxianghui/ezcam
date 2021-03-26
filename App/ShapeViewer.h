#ifndef SHAPEVIEWER_H
#define SHAPEVIEWER_H

#include <QFrame>
#include <QImage>
#include <vector>

class RDocument;
class RDocumentInterface;

namespace Ui {
class ShapeViewer;
}

class ShapeViewer : public QFrame
{
    Q_OBJECT

public:
    explicit ShapeViewer(QWidget *parent = 0);
    ~ShapeViewer();

    void importFile(const QString& strFilePathName, const QString& strNameFilter = "");
    RDocumentInterface* getDocumentInterface();

protected:
    void init();
    void initEventHandler();

protected:
    void resizeEvent(QResizeEvent* event);

private:
    Ui::ShapeViewer *ui;
    RDocumentInterface* _documentInterface;
};

#endif // SHAPEVIEWER_H
