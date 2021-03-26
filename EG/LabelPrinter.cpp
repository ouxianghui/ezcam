#include "LabelPrinter.h"
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QTextCodec>
#include "Typedef.h"
#include "SystemConfigManager.h"
#include "QR.h"
#include "LabelDrawer.h"

LabelPrinter::LabelPrinter(QPrinter* printer,
                           const std::vector<BoardSt>& vecSt,
                           int nTemplate,
                           int nLTContext,
                           double dEdgeMargin,
                           double dLineSpacing,
                           double dLogoSize,
                           double dBoardSize,
                           double dQRCodeSize)
    : m_pPrinter(printer),
      m_vecSt(vecSt),
      m_nTemplate(nTemplate),
      m_nLTContext(nLTContext)
{
    m_pDrawer = new LabelDrawer(dEdgeMargin, dLineSpacing, dLogoSize, dBoardSize, dQRCodeSize);

    m_pPainter = new QPainter();

    QString strDir = QApplication::applicationDirPath();
    QString strFileName = QString("/logo/logo.png");
    QString strFilePathName = strDir + strFileName;

    m_logoImage.load(strFilePathName);
}

LabelPrinter::~LabelPrinter()
{
    if (m_pDrawer)
    {
        delete m_pDrawer;
        m_pDrawer = NULL;
    }

    if (m_pPainter)
    {
        delete m_pPainter;
        m_pPainter = NULL;
    }
}

void LabelPrinter::setPrinter(QPrinter* printer)
{
    m_pPrinter = printer;
}

void LabelPrinter::setLabelTemplate(int nTemplate)
{
    m_nTemplate = nTemplate;
}

void LabelPrinter::setLeftTopContext(int nLTContext)
{
    m_nLTContext = nLTContext;
}

void LabelPrinter::setEdgeMargin(double dMargin)
{
    if (m_pDrawer)
    {
        m_pDrawer->setEdgeMargin(dMargin);
    }
}

void LabelPrinter::setLineSpacing(double dSpacing)
{
    if (m_pDrawer)
    {
        m_pDrawer->setLineSpacing(dSpacing);
    }
}

void LabelPrinter::setLogoSize(double dLogoSize)
{
    if (m_pDrawer)
    {
        m_pDrawer->setLogoSize(dLogoSize);
    }
}

void LabelPrinter::setBoardSize(double dBoardSize)
{
    if (m_pDrawer)
    {
        m_pDrawer->setBoardSize(dBoardSize);
    }
}

void LabelPrinter::setQRCodeSize(double dQRCodeSize)
{
    if (m_pDrawer)
    {
        m_pDrawer->setQRCodeSize(dQRCodeSize);
    }
}

void LabelPrinter::run()
{
    if (!m_pPrinter || !m_pDrawer || !m_pPainter)
    {
        return;
    }

    m_pPainter->begin(m_pPrinter);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    if (m_nTemplate == 0)
    {
        QFont font(tc->toUnicode("黑体"), 10, QFont::Bold);
        m_pPainter->setFont(font);
    }
    else if (m_nTemplate == 1)
    {
        QFont font(tc->toUnicode("黑体"), 9, QFont::Bold);
        m_pPainter->setFont(font);
    }
    else if (m_nTemplate == 2)
    {
        QFont font(tc->toUnicode("黑体"), 8, QFont::Bold);
        m_pPainter->setFont(font);
    }

    m_pPainter->setRenderHint(QPainter::Antialiasing);

    std::vector<BoardSt>::const_iterator it = m_vecSt.begin();

    for (; it != m_vecSt.end(); ++it)
    {
        const BoardSt& st = *it;
        if (it != m_vecSt.begin())
        {
            m_pPrinter->newPage();
        }

        // 打印一页
        if (m_nTemplate == 0)
        {
            m_pDrawer->drawW5H3Label(*m_pPainter, st);
        }
        else if (m_nTemplate == 1)
        {
            m_pDrawer->drawW6H4Label(*m_pPainter, st, m_nLTContext);
        }
        else if (m_nTemplate == 2)
        {
            m_pDrawer->drawW4H3Label(*m_pPainter, st);
        }
    }

    m_pPainter->end();
}
