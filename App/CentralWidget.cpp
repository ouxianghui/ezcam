#include "CentralWidget.h"
#include <QGridLayout>

CentralWidget::CentralWidget(QWidget *parent) : QFrame(parent)
{

}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::setLayoutWidgets(SolutionSummaryTableWidget* pTableWidget, ViewPort* pViewPort, QToolBar* pToolBar)
{
//    QHBoxLayout* pHLayout = new QHBoxLayout(this);
//    this->setLayout(pHLayout);

    QVBoxLayout* pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(pTableWidget, 1);

    QHBoxLayout* pCADHBoxLayout = new QHBoxLayout();
    pCADHBoxLayout->addWidget(pToolBar);
    pCADHBoxLayout->addWidget(pViewPort);
    pVLayout->addLayout(pCADHBoxLayout, 9);
}
