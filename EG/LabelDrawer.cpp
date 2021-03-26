#include "LabelDrawer.h"
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QTextCodec>
#include "Typedef.h"
#include "QR.h"

LabelDrawer::LabelDrawer(double dEdgeMargin,
                         double dLineSpacing,
                         double dLogoSize,
                         double dBoardSize,
                         double dQRCodeSize)
    : m_dEdgeMargin(dEdgeMargin),
      m_dLineSpacing(dLineSpacing),
      m_dLogoSize(dLogoSize),
      m_dBoardSize(dBoardSize),
      m_dQRCodeSize(dQRCodeSize)
{
    QString strDir = QApplication::applicationDirPath();
    QString strFileName = QString("/logo/logo.png");
    QString strFilePathName = strDir + strFileName;

    m_logoImage.load(strFilePathName);
}

LabelDrawer::~LabelDrawer()
{

}

void LabelDrawer::setEdgeMargin(double dMargin)
{
    m_dEdgeMargin = dMargin;
}

void LabelDrawer::setLineSpacing(double dSpacing)
{
    m_dLineSpacing = dSpacing;
}

void LabelDrawer::setLogoSize(double dLogoSize)
{
    m_dLineSpacing = dLogoSize;
}

void LabelDrawer::setBoardSize(double dBoardSize)
{
    m_dBoardSize = dBoardSize;
}

void LabelDrawer::setQRCodeSize(double dQRCodeSize)
{
    m_dQRCodeSize = dQRCodeSize;
}

QImage LabelDrawer::generateW6H4LabelImage(const BoardSt& st, int nLTContext)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    double dScale = 4.0/6.0;

    QSize icSize(PREVIEW_LABEL_SIZE, PREVIEW_LABEL_SIZE*dScale);

    QImage labelImage(icSize.width(), icSize.height(), QImage::Format_ARGB32);

    QPainter painter(&labelImage);

    QFont font(tc->toUnicode("黑体"), 10, QFont::Bold);

    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing);

    drawW6H4Label(painter, st, nLTContext);

    return labelImage;
}

QImage LabelDrawer::generateW5H3LabelImage(const BoardSt& st)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    double dScale = 3.0/5.0;

    QSize icSize(PREVIEW_LABEL_SIZE, PREVIEW_LABEL_SIZE*dScale);

    QImage labelImage(icSize.width(), icSize.height(), QImage::Format_ARGB32);

    QPainter painter(&labelImage);

    QFont font(tc->toUnicode("黑体"), 9, QFont::Bold);

    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing);

    drawW5H3Label(painter, st);

    return labelImage;
}

QImage LabelDrawer::generateW4H3LabelImage(const BoardSt& st)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    double dScale = 3.0/4.0;

    QSize icSize(PREVIEW_LABEL_SIZE, PREVIEW_LABEL_SIZE*dScale);

    QImage labelImage(icSize.width(), icSize.height(), QImage::Format_ARGB32);

    QPainter painter(&labelImage);

    QFont font(tc->toUnicode("黑体"), 9, QFont::Bold);

    painter.setFont(font);

    painter.setRenderHint(QPainter::Antialiasing);

    drawW4H3Label(painter, st);

    return labelImage;
}

void LabelDrawer::drawW6H4Label(QPainter& painter, const BoardSt& st, int nLTContext)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QRect rcViewport = painter.viewport();

    painter.fillRect(rcViewport, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::DashLine));
    painter.drawRect(QRectF(rcViewport.x()+4, rcViewport.y()+4, rcViewport.width()-8, rcViewport.height()-8));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::SolidLine));
    QRectF rcLTContext(m_dEdgeMargin, m_dEdgeMargin, m_dLogoSize, m_dLogoSize);

    // 绘制第一块内容，视口左上角的Logo或客户信息
    if (nLTContext == 0)
    {
        painter.drawImage(rcLTContext, m_logoImage);
    }
    else if (nLTContext == 1)
    {
        QRectF rcText = painter.boundingRect(rcLTContext, Qt::TextWordWrap, st.dataSt.strClientInformation);
        painter.drawText(rcText, st.dataSt.strClientInformation);
    }

    const int nOffset = 10;

    // 先算字体高度
    int nFontHeight = painter.fontMetrics().height();

    // 绘制第二块内容，视口左下角的矩形框、板件尺寸和封边量
    QRectF rcBoardH(m_dEdgeMargin,
                    m_dEdgeMargin+m_dLogoSize+m_dLineSpacing+nOffset,
                    m_dBoardSize,
                    nFontHeight);
    QString strBoardH = QString("%1").arg(st.dataSt.nBoardHeight);
    QRectF rcHText = painter.boundingRect(rcBoardH, Qt::TextWordWrap, strBoardH);
    painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcHText.width())/2.0,
                            rcHText.y(),
                            rcHText.width(),
                            rcHText.height()),
                     strBoardH);

    QRectF rcBox(m_dEdgeMargin,
                 m_dEdgeMargin+m_dLogoSize+m_dLineSpacing*2+rcHText.height()+nOffset,
                 m_dBoardSize,
                 m_dBoardSize);
    painter.drawRect(rcBox);

    QRectF rcBoardW(m_dEdgeMargin+m_dBoardSize+m_dLineSpacing,
                    m_dEdgeMargin+m_dLogoSize+m_dLineSpacing*2+rcHText.height()+nOffset,
                    nFontHeight,
                    m_dBoardSize);

    QString strBoardW = QString("%1").arg(st.dataSt.nBoardWidth);
    QRectF rcWText = painter.boundingRect(rcBoardW, Qt::TextWordWrap, strBoardW);

    QTransform transform;
    transform.translate(rcBoardW.center().x(), rcBoardW.center().y());
    transform.rotate(-90.0);
    //painter.setWorldTransform(transform);
    painter.setTransform(transform);

    painter.drawText(QRectF(-rcWText.width()/2,
                            -rcWText.height()/2,
                            rcWText.width(),
                            rcWText.height()),
                     strBoardW);

    painter.resetTransform();

    // 获取封边量
    if (!st.dataSt.strEdgeWeight.isEmpty() && !st.dataSt.strEdgeWeight.isNull())
    {
        QStringList sl = st.dataSt.strEdgeWeight.split(' ');
        if (sl.size() >= 4)
        {
            // 绘制封边量，第一个放底部，第二个放右侧，第三个放上方，第四个放左侧
            QString strBottom = sl.at(0);
            QString strRight = sl.at(1);
            QString strTop = sl.at(2);
            QString strLeft = sl.at(3);

            QRectF rcBttom(m_dEdgeMargin,
                           m_dEdgeMargin+m_dLogoSize+m_dBoardSize+m_dLineSpacing*2+rcWText.height()-nFontHeight+nOffset,
                           m_dBoardSize,
                           nFontHeight);

            QRectF rcBottomText = painter.boundingRect(rcBttom, Qt::TextWordWrap, strBottom);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcBottomText.width())/2.0,
                                    rcBottomText.y(),
                                    rcBottomText.width(),
                                    rcBottomText.height()),
                             strBottom);

            QRectF rcRight(m_dEdgeMargin+m_dBoardSize-nFontHeight,
                           m_dEdgeMargin+m_dLogoSize+m_dLineSpacing*2+rcWText.height()+nOffset,
                           nFontHeight,
                           m_dBoardSize);

            QRectF rcRightText = painter.boundingRect(rcRight, Qt::TextWordWrap, strRight);

            QTransform transformR;
            transformR.translate(rcRight.center().x(), rcRight.center().y());
            transformR.rotate(-90.0);
            painter.setTransform(transformR);

            painter.drawText(QRectF(-rcRightText.width()/2,
                                    -rcRightText.height()/2,
                                    rcRightText.width(),
                                    rcRightText.height()),
                             strRight);

            painter.resetTransform();

            /*
            QRectF rcRight(m_dEdgeMargin+m_dBoardSize/2.0,
                           m_dEdgeMargin+m_dLogoSize+m_dLineSpacing/2.0+nFontHeight+m_dBoardSize/2.0+nOffset,
                           m_dBoardSize,
                           nFontHeight);

            QRectF rcRightText = painter.boundingRect(rcRight, Qt::TextWordWrap, strRight);

            painter.drawText(rcRightText, strRight);
            */

            QRectF rcTop(m_dEdgeMargin,
                         m_dEdgeMargin+m_dLogoSize+m_dLineSpacing*2+rcWText.height()+nOffset,
                         m_dBoardSize,
                         nFontHeight);

            QRectF rcTopText = painter.boundingRect(rcTop, Qt::TextWordWrap, strTop);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcTopText.width())/2.0,
                                    rcTopText.y(),
                                    rcTopText.width(),
                                    rcTopText.height()),
                             strTop);


            // 左侧
            QRectF rcLeft(m_dEdgeMargin,
                          m_dEdgeMargin+m_dLogoSize+m_dLineSpacing*2+rcWText.height()+nOffset,
                          nFontHeight,
                          m_dBoardSize);

            QRectF rcLeftText = painter.boundingRect(rcLeft, Qt::TextWordWrap, strLeft);

            QTransform transformL;
            transformL.translate(rcLeft.center().x(), rcLeft.center().y());
            transformL.rotate(90.0);
            painter.setTransform(transformL);

            painter.drawText(QRectF(-rcLeftText.width()/2,
                                    -rcLeftText.height()/2,
                                    rcLeftText.width(),
                                    rcLeftText.height()),
                             strLeft);

            painter.resetTransform();

            /*
            QRectF rcLeft(m_dEdgeMargin,
                          m_dEdgeMargin+m_dLogoSize+m_dLineSpacing/2.0+nFontHeight+m_dBoardSize/2.0+nOffset,
                          m_dBoardSize,
                          nFontHeight);

            QRectF rcLeftText = painter.boundingRect(rcLeft, Qt::TextWordWrap, strLeft);

            painter.drawText(rcLeftText, strLeft);
            */
        }
    }

    // 绘制板材类型、条码编号、产品名称、工件名称、尺寸
    QRectF rcMaterialType(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                          m_dEdgeMargin,
                          rcViewport.width()-(m_dLogoSize+nFontHeight+m_dLineSpacing*2),
                          nFontHeight);

    QRectF rcMaterialTypeText = painter.boundingRect(rcMaterialType, Qt::TextWordWrap, st.dataSt.strMaterialType);
    painter.drawText(rcMaterialTypeText, st.dataSt.strMaterialType);

    QRectF rcBarcode(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                     m_dEdgeMargin+nFontHeight+m_dLineSpacing,
                     rcViewport.width()-(m_dLogoSize+nFontHeight+m_dLineSpacing*2),
                     nFontHeight);

    QRectF rcBarcodeText = painter.boundingRect(rcBarcode, Qt::TextWordWrap, st.dataSt.strBarCodeNo);
    painter.drawText(rcBarcodeText, st.dataSt.strBarCodeNo);

    QRectF rcProductName(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                         m_dEdgeMargin+nFontHeight*2+m_dLineSpacing*2,
                         rcViewport.width()-(m_dLogoSize+nFontHeight+m_dLineSpacing*2),
                         nFontHeight);

    QString strProductName = QString("%1  %2").arg(st.dataSt.strProductName).arg(st.dataSt.strFinishedProductName);
    QRectF rcProductNameText = painter.boundingRect(rcProductName, Qt::TextWordWrap, strProductName);
    painter.drawText(rcProductNameText, strProductName);

    /*
    QRectF rcFinishedProductName(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                                 m_dEdgeMargin+nFontHeight*3+m_dLineSpacing*3,
                                 rcViewport.width()-(m_dLogoSize+nFontHeight+m_dLineSpacing*2),
                                 nFontHeight);

    QRectF rcFinishedProductNameText = painter.boundingRect(rcFinishedProductName, Qt::TextWordWrap, st.dataSt.strFinishedProductName);
    painter.drawText(rcFinishedProductNameText, st.dataSt.strFinishedProductName);
    */

    QString strSize = QString("%1*%2*%3").arg(st.dataSt.nBoardHeight).arg(st.dataSt.nBoardWidth).arg(st.dataSt.nBoardThickness);
    QRectF rcSize(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                  m_dEdgeMargin+nFontHeight*3+m_dLineSpacing*3,
                  rcViewport.width()-(m_dLogoSize+nFontHeight+m_dLineSpacing*2),
                  nFontHeight);

    QRectF rcSizeText = painter.boundingRect(rcSize, Qt::TextWordWrap, strSize);
    painter.drawText(rcSizeText, strSize);

    // 绘制二维码
    QImage image;
    QR qr;
    qr.qrEncode(st.dataSt.strBarCodeNo, image);

    QRectF rcQRCode(m_dEdgeMargin+m_dLogoSize+nFontHeight+m_dLineSpacing*2,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+nOffset+2,
                    m_dQRCodeSize,
                    m_dQRCodeSize);

    painter.drawImage(rcQRCode, image);


    // 绘制板件编号
    QString strIndex = QString(tc->toUnicode("板材%1-%2")).arg(st.nSourceBoardIndex).arg(st.nBoardIndex);

    QRectF rcIndex(rcViewport.width()-m_dEdgeMargin,
                   m_dEdgeMargin,
                   m_dBoardSize,
                   nFontHeight);

    QRectF rcIndexText = painter.boundingRect(rcIndex, Qt::TextSingleLine, strIndex);
    painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcIndexText.width(),
                            rcIndexText.y(),
                            rcIndexText.width(),
                            rcIndexText.height()),
                     strIndex);

    // 绘制异形、拉槽文字

    std::map<QString, QVariant> paramsMap;
    QStringList sl = st.dataSt.strDissimilarShapes.split(';');
    for (int i = 0; i < sl.size(); ++i)
    {
        QString str = sl.at(i);
        if (!str.isEmpty())
        {
            QStringList slParam = str.split('=');
            if (slParam.size() >= 2)
            {
                QString strKey = slParam.at(0);
                QString strValue = slParam.at(1);
                bool ok = false;
                strValue.toDouble(&ok);
                if (ok)
                {
                    paramsMap[strKey] = strValue;
                }

            }
        }
    }

    if (paramsMap.find("ID") != paramsMap.end())
    {
        QString strID = paramsMap["ID"].toString();
        bool ok = false;
        int nID = strID.toInt(&ok);
        if (ok)
        {
            // 绘制异形文字
            QString strDissimilar(tc->toUnicode("异形"));

            QRectF rcDissimilar(rcViewport.width()-m_dEdgeMargin,
                                rcViewport.height()-nFontHeight*2-m_dLineSpacing*2-m_dEdgeMargin,
                                m_dBoardSize,
                                nFontHeight);

            QRectF rcDissimilarText = painter.boundingRect(rcDissimilar, Qt::TextWordWrap, strDissimilar);
            painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcDissimilarText.width(),
                                    rcDissimilarText.y(),
                                    rcDissimilarText.width(),
                                    rcDissimilarText.height()),
                             strDissimilar);

            // id == 11是拉槽
            if (nID == 7 || nID == 8 || nID == 13)
            {
                QString strSlot(tc->toUnicode("拉槽"));

                QRectF rcSlot(rcViewport.width()-m_dEdgeMargin,
                              rcViewport.height()-nFontHeight-m_dLineSpacing-m_dEdgeMargin,
                              m_dBoardSize,
                              nFontHeight);

                QRectF rcSlotText = painter.boundingRect(rcSlot, Qt::TextWordWrap, strSlot);
                painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcSlotText.width(),
                                        rcSlotText.y(),
                                        rcSlotText.width(),
                                        rcSlotText.height()),
                                 strSlot);
            }
        }
    }
}

void LabelDrawer::drawW5H3Label(QPainter& painter, const BoardSt& st)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    // 先算字体高度
    int nFontHeight = painter.fontMetrics().height();

    QRect rcViewport = painter.viewport();

    painter.fillRect(rcViewport, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::DashLine));
    painter.drawRect(QRectF(rcViewport.x()+4, rcViewport.y()+4, rcViewport.width()-8, rcViewport.height()-8));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::SolidLine));

    // 绘制左上角的客户信息
    QRectF rcClientInfo(m_dEdgeMargin,
                        m_dEdgeMargin,
                        rcViewport.width(),
                        nFontHeight);


    QRectF rcClientInfoText = painter.boundingRect(rcClientInfo, Qt::TextWordWrap, st.dataSt.strClientInformation);
    painter.drawText(rcClientInfoText, st.dataSt.strClientInformation);

    // 绘制左上角的条码编号
    QRectF rcBarcode(m_dEdgeMargin,
                     m_dEdgeMargin+nFontHeight+m_dLineSpacing,
                     rcViewport.width()*2/3,
                     nFontHeight);

    QRectF rcBarcodeText = painter.boundingRect(rcBarcode, Qt::TextWordWrap, st.dataSt.strBarCodeNo);
    painter.drawText(rcBarcodeText, st.dataSt.strBarCodeNo);

    // 绘制左上角的成品名称
    QRectF rcFinishedProductName(m_dEdgeMargin,
                                 m_dEdgeMargin+nFontHeight*2+m_dLineSpacing*2,
                                 rcViewport.width(),
                                 nFontHeight);

    QRectF rcFinishedProductNameText = painter.boundingRect(rcFinishedProductName, Qt::TextWordWrap, st.dataSt.strFinishedProductName);
    painter.drawText(rcFinishedProductNameText, st.dataSt.strFinishedProductName);


    // 绘制第二块内容，视口左下角的矩形框、板件尺寸和封边量
    QRectF rcBoardH(m_dEdgeMargin,
                    m_dEdgeMargin+nFontHeight*3+m_dLineSpacing*3,
                    m_dBoardSize,
                    nFontHeight);
    QString strBoardH = QString("%1").arg(st.dataSt.nBoardHeight);
    QRectF rcHText = painter.boundingRect(rcBoardH, Qt::TextWordWrap, strBoardH);
    painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcHText.width())/2.0,
                            rcHText.y(),
                            rcHText.width(),
                            rcHText.height()),
                     strBoardH);

    QRectF rcBox(m_dEdgeMargin,
                 m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4,
                 m_dBoardSize,
                 m_dBoardSize);
    painter.drawRect(rcBox);

    QRectF rcBoardW(m_dEdgeMargin+m_dBoardSize+m_dLineSpacing,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4,
                    nFontHeight,
                    m_dBoardSize);

    QString strBoardW = QString("%1").arg(st.dataSt.nBoardWidth);
    QRectF rcWText = painter.boundingRect(rcBoardW, Qt::TextWordWrap, strBoardW);

    QTransform transform;
    transform.translate(rcBoardW.center().x(), rcBoardW.center().y());
    transform.rotate(-90.0);
    //painter.setWorldTransform(transform);
    painter.setTransform(transform);

    painter.drawText(QRectF(-rcWText.width()/2,
                            -rcWText.height()/2,
                            rcWText.width(),
                            rcWText.height()),
                     strBoardW);

    painter.resetTransform();

    // 获取封边量
    if (!st.dataSt.strEdgeWeight.isEmpty() && !st.dataSt.strEdgeWeight.isNull())
    {
        QStringList sl = st.dataSt.strEdgeWeight.split(' ');
        if (sl.size() >= 4)
        {
            // 绘制封边量，第一个放底部，第二个放右侧，第三个放上方，第四个放左侧
            QString strBottom = sl.at(0);
            QString strRight = sl.at(1);
            QString strTop = sl.at(2);
            QString strLeft = sl.at(3);

            QRectF rcBttom(m_dEdgeMargin,
                           rcBox.bottom()-nFontHeight,
                           m_dBoardSize,
                           nFontHeight);

            QRectF rcBottomText = painter.boundingRect(rcBttom, Qt::TextWordWrap, strBottom);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcBottomText.width())/2.0,
                                    rcBottomText.y(),
                                    rcBottomText.width(),
                                    rcBottomText.height()),
                             strBottom);

            QRectF rcRight(m_dEdgeMargin+m_dBoardSize-nFontHeight,
                           rcBox.top(),
                           nFontHeight,
                           m_dBoardSize);

            QRectF rcRightText = painter.boundingRect(rcRight, Qt::TextWordWrap, strRight);

            QTransform transformR;
            transformR.translate(rcRight.center().x(), rcRight.center().y());
            transformR.rotate(-90.0);
            painter.setTransform(transformR);

            painter.drawText(QRectF(-rcRightText.width()/2,
                                    -rcRightText.height()/2,
                                    rcRightText.width(),
                                    rcRightText.height()),
                             strRight);

            painter.resetTransform();

            /*
            QRectF rcRight(m_dEdgeMargin+m_dBoardSize/2.0,
                           m_dEdgeMargin+m_dLogoSize+m_dLineSpacing/2.0+nFontHeight+m_dBoardSize/2.0+nOffset,
                           m_dBoardSize,
                           nFontHeight);

            QRectF rcRightText = painter.boundingRect(rcRight, Qt::TextWordWrap, strRight);

            painter.drawText(rcRightText, strRight);
            */

            QRectF rcTop(m_dEdgeMargin,
                         rcBox.top(),
                         m_dBoardSize,
                         nFontHeight);

            QRectF rcTopText = painter.boundingRect(rcTop, Qt::TextWordWrap, strTop);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcTopText.width())/2.0,
                                    rcTopText.y(),
                                    rcTopText.width(),
                                    rcTopText.height()),
                             strTop);


            // 左侧
            QRectF rcLeft(m_dEdgeMargin,
                          rcBox.top(),
                          nFontHeight,
                          m_dBoardSize);

            QRectF rcLeftText = painter.boundingRect(rcLeft, Qt::TextWordWrap, strLeft);

            QTransform transformL;
            transformL.translate(rcLeft.center().x(), rcLeft.center().y());
            transformL.rotate(90.0);
            painter.setTransform(transformL);

            painter.drawText(QRectF(-rcLeftText.width()/2,
                                    -rcLeftText.height()/2,
                                    rcLeftText.width(),
                                    rcLeftText.height()),
                             strLeft);

            painter.resetTransform();

            /*
            QRectF rcLeft(m_dEdgeMargin,
                          m_dEdgeMargin+m_dLogoSize+m_dLineSpacing/2.0+nFontHeight+m_dBoardSize/2.0+nOffset,
                          m_dBoardSize,
                          nFontHeight);

            QRectF rcLeftText = painter.boundingRect(rcLeft, Qt::TextWordWrap, strLeft);

            painter.drawText(rcLeftText, strLeft);
            */
        }
    }

    // 绘制板件编号
    QString strIndex = QString(tc->toUnicode("板材%1-%2")).arg(st.nSourceBoardIndex).arg(st.nBoardIndex);

    QRectF rcIndex(m_dEdgeMargin+rcViewport.width()/3,
                   m_dEdgeMargin,
                   rcViewport.width()/3,
                   nFontHeight);

    QRectF rcIndexText = painter.boundingRect(rcIndex, Qt::TextSingleLine, strIndex);
    painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcIndexText.width(),
                            rcIndexText.y(),
                            rcIndexText.width(),
                            rcIndexText.height()),
                     strIndex);

    // 绘制产品名称
    QRectF rcProductName(rcViewport.width()-m_dEdgeMargin,
                         m_dEdgeMargin+nFontHeight+m_dLineSpacing,
                         rcViewport.width(),
                         nFontHeight);

    QRectF rcProductNameText = painter.boundingRect(rcProductName, Qt::TextWordWrap, st.dataSt.strProductName);
    painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcProductNameText.width(),
                            rcProductNameText.y(),
                            rcProductNameText.width(),
                            rcProductNameText.height()),
                     st.dataSt.strProductName);


    // 绘制尺寸信息
    QString strSize = QString("%1*%2*%3").arg(st.dataSt.nBoardHeight).arg(st.dataSt.nBoardWidth).arg(st.dataSt.nBoardThickness);
    QRectF rcSize(rcViewport.width()-m_dEdgeMargin,
                  m_dEdgeMargin+nFontHeight*2+m_dLineSpacing*2,
                  rcViewport.width(),
                  nFontHeight);

    QRectF rcSizeText = painter.boundingRect(rcSize, Qt::TextWordWrap, strSize);
    painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcSizeText.width(),
                            rcSizeText.y(),
                            rcSizeText.width(),
                            rcSizeText.height()),
                     strSize);


    // 绘制二维码
    QImage image;
    QR qr;
    qr.qrEncode(st.dataSt.strBarCodeNo, image);

    QRectF rcQRCode(m_dEdgeMargin+m_dBoardSize+nFontHeight*2+m_dLineSpacing*2,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4,
                    m_dQRCodeSize,
                    m_dQRCodeSize);

    painter.drawImage(rcQRCode, image);


    // 绘制异形、拉槽文字
    std::map<QString, QVariant> paramsMap;
    QStringList sl = st.dataSt.strDissimilarShapes.split(';');
    for (int i = 0; i < sl.size(); ++i)
    {
        QString str = sl.at(i);
        if (!str.isEmpty())
        {
            QStringList slParam = str.split('=');
            if (slParam.size() >= 2)
            {
                QString strKey = slParam.at(0);
                QString strValue = slParam.at(1);
                bool ok = false;
                strValue.toDouble(&ok);
                if (ok)
                {
                    paramsMap[strKey] = strValue;
                }

            }
        }
    }

    if (paramsMap.find("ID") != paramsMap.end())
    {
        QString strID = paramsMap["ID"].toString();
        bool ok = false;
        int nID = strID.toInt(&ok);
        if (ok)
        {
            // 绘制异形文字
            QString strDissimilar(tc->toUnicode("异形"));

            QRectF rcDissimilar(rcQRCode.left(),
                                rcQRCode.top()-nFontHeight-m_dLineSpacing,
                                m_dQRCodeSize,
                                nFontHeight);

            QRectF rcDissimilarText = painter.boundingRect(rcDissimilar, Qt::TextWordWrap, strDissimilar);
            painter.drawText(QRectF(rcQRCode.left()+(rcQRCode.width()-rcDissimilarText.width())/2.0,
                                    rcDissimilarText.top(),
                                    rcDissimilarText.width(),
                                    rcDissimilarText.height()),
                             strDissimilar);

            // id == 11是拉槽
            if (nID == 7 || nID == 8 || nID == 13)
            {
                QString strSlot(tc->toUnicode("拉槽"));

                QRectF rcSlot(rcViewport.width()-m_dEdgeMargin,
                              rcQRCode.top()-nFontHeight-m_dLineSpacing,
                              m_dQRCodeSize,
                              nFontHeight);

                QRectF rcSlotText = painter.boundingRect(rcSlot, Qt::TextWordWrap, strSlot);
                painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcSlotText.width(),
                                         rcSlotText.top(),
                                         rcSlotText.width(),
                                         rcSlotText.height()),
                                 strSlot);
            }
        }
    }
}

void LabelDrawer::drawW4H3Label(QPainter& painter, const BoardSt& st)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QRect rcViewport = painter.viewport();

    painter.fillRect(rcViewport, QBrush(QColor(255, 255, 255)));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::DashLine));
    painter.drawRect(QRectF(rcViewport.x()+4, rcViewport.y()+4, rcViewport.width()-8, rcViewport.height()-8));

    painter.setPen(QPen(QBrush(Qt::black), 2.0, Qt::SolidLine));


    // 先算字体高度
    int nFontHeight = painter.fontMetrics().height();

    // 绘制左上角的客户信息
    QRectF rcClientInfo(m_dEdgeMargin,
                        m_dEdgeMargin,
                        rcViewport.width(),
                        nFontHeight);


    QRectF rcClientInfoText = painter.boundingRect(rcClientInfo, Qt::TextWordWrap, st.dataSt.strClientInformation);
    painter.drawText(rcClientInfoText, st.dataSt.strClientInformation);

    // 绘制第一块内容，视口左下角的矩形框、板件尺寸和封边量
    QRectF rcBoardH(m_dEdgeMargin,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*3,
                    m_dBoardSize,
                    nFontHeight);

    QString strBoardH = QString("%1").arg(st.dataSt.nBoardHeight);
    QRectF rcHText = painter.boundingRect(rcBoardH, Qt::TextWordWrap, strBoardH);
    painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcHText.width())/2.0,
                            rcHText.y(),
                            rcHText.width(),
                            rcHText.height()),
                     strBoardH);

    QRectF rcBox(m_dEdgeMargin,
                 m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcHText.height(),
                 m_dBoardSize,
                 m_dBoardSize);
    painter.drawRect(rcBox);

    QRectF rcBoardW(m_dEdgeMargin+m_dBoardSize+m_dLineSpacing,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcHText.height(),
                    nFontHeight,
                    m_dBoardSize);

    QString strBoardW = QString("%1").arg(st.dataSt.nBoardWidth);
    QRectF rcWText = painter.boundingRect(rcBoardW, Qt::TextWordWrap, strBoardW);

    QTransform transform;
    transform.translate(rcBoardW.center().x(), rcBoardW.center().y());
    transform.rotate(-90.0);
    //painter.setWorldTransform(transform);
    painter.setTransform(transform);

    painter.drawText(QRectF(-rcWText.width()/2,
                            -rcWText.height()/2,
                            rcWText.width(),
                            rcWText.height()),
                     strBoardW);

    painter.resetTransform();

    // 获取封边量
    if (!st.dataSt.strEdgeWeight.isEmpty() && !st.dataSt.strEdgeWeight.isNull())
    {
        QStringList sl = st.dataSt.strEdgeWeight.split(' ');
        if (sl.size() >= 4)
        {
            // 绘制封边量，第一个放底部，第二个放右侧，第三个放上方，第四个放左侧
            QString strBottom = sl.at(0);
            QString strRight = sl.at(1);
            QString strTop = sl.at(2);
            QString strLeft = sl.at(3);

            QRectF rcBttom(m_dEdgeMargin,
                           m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+m_dBoardSize+rcWText.height()-nFontHeight,
                           m_dBoardSize,
                           nFontHeight);

            QRectF rcBottomText = painter.boundingRect(rcBttom, Qt::TextWordWrap, strBottom);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcBottomText.width())/2.0,
                                    rcBottomText.y(),
                                    rcBottomText.width(),
                                    rcBottomText.height()),
                             strBottom);

            QRectF rcRight(m_dEdgeMargin+m_dBoardSize-nFontHeight,
                           m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcWText.height(),
                           nFontHeight,
                           m_dBoardSize);

            QRectF rcRightText = painter.boundingRect(rcRight, Qt::TextWordWrap, strRight);

            QTransform transformR;
            transformR.translate(rcRight.center().x(), rcRight.center().y());
            transformR.rotate(-90.0);
            painter.setTransform(transformR);

            painter.drawText(QRectF(-rcRightText.width()/2,
                                    -rcRightText.height()/2,
                                    rcRightText.width(),
                                    rcRightText.height()),
                             strRight);

            painter.resetTransform();

            QRectF rcTop(m_dEdgeMargin,
                         m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcWText.height(),
                         m_dBoardSize,
                         nFontHeight);

            QRectF rcTopText = painter.boundingRect(rcTop, Qt::TextWordWrap, strTop);
            painter.drawText(QRectF(m_dEdgeMargin+(m_dBoardSize-rcTopText.width())/2.0,
                                    rcTopText.y(),
                                    rcTopText.width(),
                                    rcTopText.height()),
                             strTop);


            // 左侧
            QRectF rcLeft(m_dEdgeMargin,
                          m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcWText.height(),
                          nFontHeight,
                          m_dBoardSize);

            QRectF rcLeftText = painter.boundingRect(rcLeft, Qt::TextWordWrap, strLeft);

            QTransform transformL;
            transformL.translate(rcLeft.center().x(), rcLeft.center().y());
            transformL.rotate(90.0);
            painter.setTransform(transformL);

            painter.drawText(QRectF(-rcLeftText.width()/2,
                                    -rcLeftText.height()/2,
                                    rcLeftText.width(),
                                    rcLeftText.height()),
                             strLeft);

            painter.resetTransform();
        }
    }

    // 绘制条码编号、柜体名称、成品名称、尺寸信息

    QRectF rcBarcode(m_dEdgeMargin,
                     m_dEdgeMargin+nFontHeight+m_dLineSpacing,
                     rcViewport.width()-(nFontHeight+m_dLineSpacing*2),
                     nFontHeight);

    QRectF rcBarcodeText = painter.boundingRect(rcBarcode, Qt::TextWordWrap, st.dataSt.strBarCodeNo);
    painter.drawText(rcBarcodeText, st.dataSt.strBarCodeNo);

    QRectF rcProductName(m_dEdgeMargin,
                         m_dEdgeMargin+nFontHeight*2+m_dLineSpacing*2,
                         rcViewport.width()-(nFontHeight+m_dLineSpacing*2),
                         nFontHeight);

    QRectF rcProductNameText = painter.boundingRect(rcProductName, Qt::TextWordWrap, st.dataSt.strProductName);
    painter.drawText(rcProductNameText, st.dataSt.strProductName);


    QRectF rcFinishedProductName(m_dEdgeMargin,
                                 m_dEdgeMargin+nFontHeight*2+m_dLineSpacing*2,
                                 rcViewport.width()-(nFontHeight+m_dLineSpacing*2),
                                 nFontHeight);

    QRectF rcFinishedProductNameText = painter.boundingRect(rcFinishedProductName, Qt::TextWordWrap, st.dataSt.strFinishedProductName);
    painter.drawText(QRectF(rcViewport.width()-rcFinishedProductNameText.width()-m_dEdgeMargin,
                            rcFinishedProductNameText.y(),
                            rcFinishedProductNameText.width(),
                            rcFinishedProductNameText.height()),
                     st.dataSt.strFinishedProductName);


    QString strSize = QString("%1*%2*%3").arg(st.dataSt.nBoardHeight).arg(st.dataSt.nBoardWidth).arg(st.dataSt.nBoardThickness);
    QRectF rcSize(m_dEdgeMargin,
                  m_dEdgeMargin+nFontHeight*3+m_dLineSpacing*3,
                  rcViewport.width()-(nFontHeight+m_dLineSpacing*2),
                  nFontHeight);

    QRectF rcSizeText = painter.boundingRect(rcSize, Qt::TextWordWrap, strSize);
    painter.drawText(rcSizeText, strSize);

    // 绘制二维码
    QImage image;
    QR qr;
    qr.qrEncode(st.dataSt.strBarCodeNo, image);

    QRectF rcQRCode(rcViewport.width()-m_dQRCodeSize-m_dEdgeMargin,
                    m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcHText.height(),
                    m_dQRCodeSize,
                    m_dQRCodeSize);

    painter.drawImage(rcQRCode, image);


    // 绘制板件编号
    QString strIndex = QString(tc->toUnicode("板材%1-%2")).arg(st.nSourceBoardIndex).arg(st.nBoardIndex);

    QRectF rcIndex(rcViewport.width()-m_dEdgeMargin,
                   m_dEdgeMargin,
                   m_dBoardSize,
                   nFontHeight);

    QRectF rcIndexText = painter.boundingRect(rcIndex, Qt::TextSingleLine, strIndex);
    painter.drawText(QRectF(rcViewport.width()-m_dEdgeMargin-rcIndexText.width(),
                            rcIndexText.y(),
                            rcIndexText.width(),
                            rcIndexText.height()),
                     strIndex);

    // 绘制异形、拉槽文字

    std::map<QString, QVariant> paramsMap;
    QStringList sl = st.dataSt.strDissimilarShapes.split(';');
    for (int i = 0; i < sl.size(); ++i)
    {
        QString str = sl.at(i);
        if (!str.isEmpty())
        {
            QStringList slParam = str.split('=');
            if (slParam.size() >= 2)
            {
                QString strKey = slParam.at(0);
                QString strValue = slParam.at(1);
                bool ok = false;
                strValue.toDouble(&ok);
                if (ok)
                {
                    paramsMap[strKey] = strValue;
                }

            }
        }
    }

    if (paramsMap.find("ID") != paramsMap.end())
    {
        QString strID = paramsMap["ID"].toString();
        bool ok = false;
        int nID = strID.toInt(&ok);
        if (ok)
        {
            // 绘制异形文字
            QString strDissimilar(tc->toUnicode("异形"));

            QRectF rcDissimilar(m_dEdgeMargin+m_dBoardSize + nFontHeight + m_dLineSpacing*20,
                                m_dEdgeMargin+nFontHeight*4+m_dLineSpacing*4+rcHText.height(),
                                m_dBoardSize,
                                nFontHeight);

            painter.drawText(rcDissimilar, strDissimilar);

            // id == 11是拉槽
            if (nID == 7 || nID == 8 || nID == 13)
            {
                QString strSlot(tc->toUnicode("拉槽"));

                QRectF rcSlot(m_dEdgeMargin+m_dBoardSize + nFontHeight + m_dLineSpacing*20,
                              rcDissimilar.y()+m_dBoardSize-nFontHeight,
                              m_dBoardSize,
                              nFontHeight);

                painter.drawText(rcSlot, strSlot);
            }
        }
    }
}
