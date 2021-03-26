#ifndef EZEVENTHANDLER_H
#define EZEVENTHANDLER_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QScrollBar>
#include <QPainter>
#include "RVector.h"
#include "RSnap.h"
#include "RSnapRestriction.h"
#include "RTextLabel.h"
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "RGraphicsView.h"
#include "RRulerQt.h"

class EZEventHandler : public QObject
{
    Q_OBJECT
public:
    explicit EZEventHandler(QWidget* pWidget, RDocumentInterface* pDocumentInterface, QObject *parent = 0);
    ~EZEventHandler();

signals:

public slots:
    void viewportChanged();
    void updateSnapInfo(QPainter* painter, RSnap* snap, RSnapRestriction* restriction);
    void updateTextLabel(QPainter* painter, const RTextLabel& textLabel);
    void hScrollBarValueChanged(int value);
    void vScrollBarValueChanged(int value);

protected:
    void drawSnapLabel(QPainter* painter, RVector pos, RVector posRestriction, QString& text);
    void drawInfoLabel(QPainter* painter, const RTextLabel& textLabel);

private:
    RDocument* m_pDocument;
    RDocumentInterface* m_pDocumentInterface;
    QWidget* m_pWidget;
    RGraphicsView* m_pGraphicsView;
    QScrollBar* m_pHScrollBar;
    QScrollBar* m_pVScrollBar;
    RRulerQt* m_pHRuler;
    RRulerQt* m_pVRuler;
    QLabel* m_pInfoLabel;
};

#endif // EZEVENTHANDLER_H
