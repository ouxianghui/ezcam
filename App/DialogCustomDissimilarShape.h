#ifndef DIALOGCUSTOMDISSIMILARSHAPE_H
#define DIALOGCUSTOMDISSIMILARSHAPE_H

#include <QDialog>
#include <QDialogButtonBox>
#include "ShapeViewer.h"
#include "ShapeEditorTableWidget.h"
#include <QList>
#include <QSharedPointer>
#include "RShape.h"
#include "RCircle.h"

class RDocument;
class RDocumentInterface;

namespace Ui {
class DialogCustomDissimilarShape;
}

class DialogCustomDissimilarShape : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomDissimilarShape(const QString& barcodeNo,
                                         const QString& productName,
                                         double width,
                                         double height,
                                         bool editExisting = false,
                                         QWidget *parent = 0);
    ~DialogCustomDissimilarShape();

    QString getShape();

protected:
    void createWidgets();
    void drawOutline();

protected slots:
    void acceptButtonClicked();
    void rejectButtonClicked();
    void onImportDxfButtonClicked(bool checked = false);

private:
    Ui::DialogCustomDissimilarShape *ui;
    ShapeViewer* _shapeViewer;
    ShapeEditorTableWidget* _shapeEditor;
    QDialogButtonBox* _buttonBox;

    QString _barcodeNo;
    QString _productName;
    double _width;
    double _height;
    RDocumentInterface* _documentInterface;

    QString _shapeId;
};

#endif // DIALOGCUSTOMDISSIMILARSHAPE_H
