#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QFrame>
#include "SolutionSummaryTableWidget.h"
#include "GraphicsViewer.h"
#include "ViewPort.h"
#include <QToolBar>

class CentralWidget : public QFrame
{
    Q_OBJECT
public:
    explicit CentralWidget(QWidget *parent = 0);
    ~CentralWidget();

    void setLayoutWidgets(SolutionSummaryTableWidget* pTableWidget, ViewPort* pViewPort, QToolBar* pToolBar);

signals:

public slots:
};

#endif // CENTRALWIDGET_H
