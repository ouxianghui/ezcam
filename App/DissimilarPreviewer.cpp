#include "DissimilarPreviewer.h"
#include "ui_DissimilarPreviewer.h"
#include "DissimilarLibraryOperator.h"
#include <QPainter>
#include <QApplication>

DissimilarPreviewer::DissimilarPreviewer(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DissimilarPreviewer)
{
    ui->setupUi(this);
    this->setFrameShape(QFrame::Box);
    //this->setFrameShadow(QFrame::Raised);

    m_bg.load(":/icons/previewer_bg.png");
}

DissimilarPreviewer::~DissimilarPreviewer()
{
    delete ui;
}

void DissimilarPreviewer::drawDissimilar(int nID, const std::map<QString, QVariant>& paramsMap)
{
    m_nID = nID;
    m_paramsMap = paramsMap;
    this->update();
}

void DissimilarPreviewer::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRect rc = painter.viewport();
    painter.drawImage(rc, m_bg);

    DissimilarLibraryOperator::draw2QPainter(painter, m_nID, m_paramsMap);

    QFrame::paintEvent(event);
}
