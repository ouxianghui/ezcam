#include "EZEventHandler.h"
#include "RGraphicsViewQt.h"

EZEventHandler::EZEventHandler(QWidget* pWidget, RDocumentInterface* pDocumentInterface, QObject *parent)
    : QObject(parent),
      m_pWidget(pWidget),
      m_pDocumentInterface(pDocumentInterface),
      m_pDocument(NULL),
      m_pGraphicsView(NULL),
      m_pHScrollBar(NULL),
      m_pVScrollBar(NULL),
      m_pHRuler(NULL),
      m_pVRuler(NULL),
      m_pInfoLabel(NULL)
{
    if (m_pWidget)
    {
        m_pHScrollBar = pWidget->findChild<QScrollBar*>("HorizontalScrollBar");
        m_pVScrollBar = pWidget->findChild<QScrollBar*>("VerticalScrollBar");
        m_pGraphicsView = pWidget->findChild<RGraphicsViewQt*>("GraphicsView");
        m_pHRuler = pWidget->findChild<RRulerQt*>("HorizontalRuler");
        m_pVRuler = pWidget->findChild<RRulerQt*>("VerticalRuler");
        m_pInfoLabel = pWidget->findChild<QLabel*>("InfoLabel");
    }
    if (m_pDocumentInterface)
    {
        m_pDocument = &(m_pDocumentInterface->getDocument());
    }
}

EZEventHandler::~EZEventHandler()
{

}

void EZEventHandler::viewportChanged()
{
    if (!m_pHScrollBar || !m_pVScrollBar)
    {
        return;
    }

    m_pHScrollBar->blockSignals(true);
    m_pVScrollBar->blockSignals(true);

    RBox box = m_pDocument->getBoundingBox(true, true);
    int min = box.getMinimum().x * m_pGraphicsView->getFactor() - 800;
    int max = box.getMaximum().x * m_pGraphicsView->getFactor() - m_pGraphicsView->getWidth() + 800;
    m_pHScrollBar->setRange(min, max);
    m_pHScrollBar->setPageStep(m_pGraphicsView->getWidth());
    m_pHScrollBar->setValue(-m_pGraphicsView->getOffset().x * m_pGraphicsView->getFactor());

    min = m_pGraphicsView->getHeight() - box.getMaximum().y * m_pGraphicsView->getFactor() - 800;
    max = 800 - box.getMinimum().y * m_pGraphicsView->getFactor();
    m_pVScrollBar->setRange(min, max);
    m_pVScrollBar->setPageStep(m_pGraphicsView->getHeight());
    m_pVScrollBar->setValue(m_pGraphicsView->getOffset().y * m_pGraphicsView->getFactor());

    // oxh 2015/05/19
    //min = box.getMinimum().y * m_pGraphicsView->getFactor() - 800;
    //max = box.getMaximum().y * m_pGraphicsView->getFactor() + m_pGraphicsView->getHeight() + 800;
    //m_pVScrollBar->setRange(min, max);
    //m_pVScrollBar->setPageStep(m_pGraphicsView->getHeight());
    //m_pVScrollBar->setValue(-m_pGraphicsView->getOffset().y * m_pGraphicsView->getFactor());

    m_pHScrollBar->blockSignals(false);
    m_pVScrollBar->blockSignals(false);

    // rulers
    if (m_pHRuler)
    {
        m_pHRuler->updateViewport();
    }
    if (m_pVRuler)
    {
        m_pVRuler->updateViewport();
    }

    // grid info label
    RGrid* grid = m_pGraphicsView->getGrid();
    if (grid)
    {
        m_pInfoLabel->setText(grid->getInfoText());
    }
}

void EZEventHandler::updateSnapInfo(QPainter* painter, RSnap* snap, RSnapRestriction* restriction)
{
    if (!snap)
    {
        return;
    }

    RVector pos = snap->getLastSnap();
    if (!pos.isValid())
    {
        return;
    }

    RVector posRestriction;
    if (restriction)
    {
        posRestriction = restriction->getLastSnap();
    }

    QString text = "";
    switch (snap->getStatus())
    {
    //    case RSnap.Free:
    //        text = qsTr("Free");
    //        break;
    case RSnap::Grid:
        text = tr("Grid");
        break;
    case RSnap::Endpoint:
        text = tr("End");
        break;
    case RSnap::OnEntity:
        text = tr("On Entity");
        break;
    case RSnap::Perpendicular:
        text = tr("Perpendicular");
        break;
    case RSnap::Center:
        text = tr("Center");
        break;
    case RSnap::Middle:
        text = tr("Middle");
        break;
    case RSnap::Intersection:
        text = tr("Intersection");
        break;
    case RSnap::Reference:
        text = tr("Reference");
        break;
    }

    this->drawSnapLabel(painter, pos, posRestriction, text);
}

void EZEventHandler::drawSnapLabel(QPainter* painter, RVector pos, RVector posRestriction, QString& text)
{
    RVector p = m_pGraphicsView->mapToView(pos);
    RVector pr = m_pGraphicsView->mapToView(posRestriction);

    RColor color = RSettings::getColor("GraphicsViewColors/TextLabelColor", RColor(249,198,31));
    painter->setPen(color.toCompat());

    QFont font = RSettings::getSnapLabelFont();
    QFontMetrics fm(font);
    painter->setFont(font);

    if (text != "")
    {
        painter->drawText(p.x + 5,
                          p.y + 5,
                          fm.width(text)+10,
                          fm.height()+10,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          text,
                          NULL);
    }

    painter->drawEllipse(p.x-5, p.y-5, 10, 10);

    // restriction position:
    if (pr.isValid())
    {
        painter->drawLine(pr.x, pr.y-5, pr.x+5, pr.y);
        painter->drawLine(pr.x+5, pr.y, pr.x, pr.y+5);
        painter->drawLine(pr.x, pr.y+5, pr.x-5, pr.y);
        painter->drawLine(pr.x-5, pr.y, pr.x, pr.y-5);
    }

    // display distance/angle:
    int display = RSettings::getIntValue("DisplaySettings/DisplayDistanceAngle", 0);
    if (display == 0)
    {
        return;
    }

    RDocumentInterface* di = m_pGraphicsView->getDocumentInterface();
    RDocument* doc = m_pGraphicsView->getDocument();

    RVector relativeZero = di->getRelativeZero();

    double dist = 0.0f;
    double angle = 0.0f;

    if (posRestriction.isValid())
    {
        dist = relativeZero.getDistanceTo(posRestriction);
        angle = relativeZero.getAngleTo(posRestriction);
    }
    else
    {
        dist = relativeZero.getDistanceTo(pos);
        angle = relativeZero.getAngleTo(pos);
    }

    char result[64];
    memset(result, 0, 64);

    int lp = doc->getLinearPrecision();
    QString lformat = "%." + QString("%1").arg(lp) + "f";
    char pLFormat[64];
    memset(pLFormat, 0, 64);
    memcpy(pLFormat, lformat.toStdString().c_str(), lformat.toStdString().size());
    sprintf(result, pLFormat, dist);
    QString distStr(result);

    memset(result, 0, 64);

    angle = RMath::rad2deg(angle);
    int ap = doc->getAnglePrecision();
    QString aformat = "%." + QString("%1").arg(ap) + "f";
    char pAFormat[64];
    memset(pAFormat, 0, 64);
    memcpy(pAFormat, aformat.toStdString().c_str(), aformat.toStdString().size());
    sprintf(result, pAFormat, angle);
    QString angStr(result);

    QString sep = RSettings::getStringValue("Input/PolarCoordinateSeparator", "<");

    color = RSettings::getColor("GraphicsViewColors/MeasurementToolsColor", RColor(155,220,112));
    painter->setPen(color.toCompat());

    switch(display)
    {
    case 0:
        text = "";
        break;
    case 1:
        text = distStr + sep + angStr + "°";
        break;
    case 2:
        text = distStr;
        break;
    case 3:
        text = angStr + "°";
        break;
    default:
        text = "";
    }

    if (text != "")
    {
        painter->drawText(p.x + 5,
                          p.y - 15 - fm.height(),
                          fm.width(text)+10,
                          fm.height()+10,
                          Qt::AlignHCenter | Qt::AlignVCenter,
                          text,
                          NULL);
    }
}

void EZEventHandler::updateTextLabel(QPainter* painter, const RTextLabel& textLabel)
{
    drawInfoLabel(painter, textLabel);
}

void EZEventHandler::drawInfoLabel(QPainter* painter, const RTextLabel& textLabel)
{
    RVector pos = textLabel.getPosition();
    QString text = textLabel.getText();
    //var align = textLabel.getAlignment();

    RVector p = m_pGraphicsView->mapToView(pos);
    //info labels might have individual colors in future
    //var color = RSettings.getColor("GraphicsViewColors/TextLabelColor", new RColor(249,198,31));
    RColor color = RSettings::getColor("GraphicsViewColors/MeasurementToolsColor", RColor(155,220,112));
    painter->setPen(QColor(color.red(), color.green(), color.blue(), color.alpha()));

    if (text != "")
    {
        QFont font = RSettings::getInfoLabelFont();
        QFontMetrics fm(font);
        int w = fm.width(text)+10;
        int h = fm.height()+10;
        QFlag flags = Qt::AlignHCenter | Qt::AlignVCenter;
        //qDebug(flags);
        painter->setFont(font);

        //        var x, y;
        //        var dx = graphicsView.mapDistanceFromView(10);
        //        var dy = graphicsView.mapDistanceFromView(10);

        //        if (align&Qt.AlignRight) {

        //        }

        painter->drawText(p.x - w - 15, p.y + h + 5, w, h, flags, text, NULL);
    }
}

void EZEventHandler::hScrollBarValueChanged(int value)
{
    bool blocked = m_pHScrollBar->signalsBlocked();
    m_pHScrollBar->blockSignals(true);
    RVector off = m_pGraphicsView->getOffset();
    off.x = -(value / m_pGraphicsView->getFactor());
    m_pGraphicsView->setOffset(off);
    m_pHScrollBar->blockSignals(blocked);
    if (m_pHRuler)
    {
        m_pHRuler->updateViewport();
    }

}

void EZEventHandler::vScrollBarValueChanged(int value)
{
    bool blocked = m_pVScrollBar->signalsBlocked();
    m_pVScrollBar->blockSignals(true);
    RVector off = m_pGraphicsView->getOffset();
    off.y = value / m_pGraphicsView->getFactor();
    //off.y = -(value / m_pGraphicsView->getFactor());
    m_pGraphicsView->setOffset(off);
    m_pVScrollBar->blockSignals(blocked);
    if (m_pVRuler)
    {
        m_pVRuler->updateViewport();
    }
}
