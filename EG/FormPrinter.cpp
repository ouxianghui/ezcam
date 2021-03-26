#include "FormPrinter.h"
#include <QPrintPreviewDialog>
#include <QPageSetupDialog>
#include <QTextDocument>
#include <QFileDialog>
#include <QPrintDialog>

const int MediumGap = 4;
const int LargeGap = 6;
const int SmallGap = 2;

CFormPrinter::CFormPrinter()
    : m_pPrinter(NULL),
      m_nLineSpace(4),
      m_nPageSpace(4),
      m_nTotalPages(0),
      m_nPageOrientation(0)
{
    m_pPrinter = new QPrinter();
    m_pPrinter->setPageSize(QPrinter::Custom);
    m_pPrinter->setPageSizeMM(QSizeF(75, 60));
    m_pPrinter->setFullPage(true);

    if (m_nPageOrientation == 0)
    {
        m_pPrinter->setOrientation(QPrinter::Portrait);
    }
    else
    {
        m_pPrinter->setOrientation(QPrinter::Landscape);
    }

    m_pPrinter->setOutputFormat(QPrinter::NativeFormat);
}

CFormPrinter::~CFormPrinter()
{

}

void CFormPrinter::init(const QString& strCompanyName, const QString& strClientName,
                        const QString& strClientAddress, const QString& strClientPhoneNumber)
{
    m_strCompanyName = strCompanyName;
    m_strClientName = strClientName;
    m_strClientAddress = strClientAddress;
    m_strClientPhoneNumber = strClientPhoneNumber;
}

void CFormPrinter::setCompanyName(const QString& strCompanyName)
{
    m_strCompanyName = strCompanyName;
}

void CFormPrinter::setClientName(const QString& strClientName)
{
    m_strClientName = strClientName;
}

void CFormPrinter::setClientAddress(const QString& strClientAddress)
{
    m_strClientAddress = strClientAddress;
}

void CFormPrinter::setClientPhoneNumber(const QString& strClientPhoneNumber)
{
    m_strClientPhoneNumber = strClientPhoneNumber;
}

void CFormPrinter::printForm(const QStringList& entries)
{
    // QPrintDialog printDialog(m_pPrinter, this);
    // if (printDialog.exec())
    // {
    QPainter painter(m_pPrinter);
    QList<QStringList> pages;
    paginate(&painter, &pages, entries);
    m_nTotalPages = pages.size();
    printPages(&painter, pages);
    // }
}

void CFormPrinter::setWeight(const QString& strWeight)
{
    m_strTotalWeight = strWeight;
}

void CFormPrinter::setTitleFont(const QFont& font)
{
    m_titleFont = font;
}

void CFormPrinter::setBodyFont(const QFont& font)
{
    m_bodyFont = font;
}

void CFormPrinter::setLineSpace(int nSpace)
{
    m_nLineSpace = nSpace;
}

void CFormPrinter::setPageSpace(int nSpace)
{
    m_nPageSpace = nSpace;
}

void CFormPrinter::setPageOrientation(int nOrientation)
{
    m_nPageOrientation = nOrientation;

    if (m_nPageOrientation == 0)
    {
        m_pPrinter->setOrientation(QPrinter::Portrait);
    }
    else
    {
        m_pPrinter->setOrientation(QPrinter::Landscape);
    }
}

QPrinter* CFormPrinter::getPrinter()
{
    return m_pPrinter;
}

void CFormPrinter::paginate(QPainter* painter, QList<QStringList>* pages, const QStringList& entries)
{
    painter->setFont(m_titleFont);

    QString strCompanyName = m_strCompanyName.isEmpty() ? "公司名称" : m_strCompanyName;
    int n1stLineHeight = entryHeight(painter, strCompanyName);

    painter->setFont(m_bodyFont);
    QString strClientName = m_strClientName.isEmpty() ? "客户名称" : m_strClientName;
    int n2ndLineHeight = entryHeight(painter, strClientName);

    QString strClientAddress = m_strClientAddress.isEmpty() ? "客户地址" : m_strClientAddress;
    int n3rdLineHeight = entryHeight(painter, strClientAddress);

    QString strWeight = m_strTotalWeight.isEmpty() ? "1234567890kg" : m_strTotalWeight;
    int n4thLineHeight = entryHeight(painter, strWeight);

    QStringList currentPage;
    int pageHeight = painter->window().height() - n1stLineHeight - n2ndLineHeight - n3rdLineHeight - n4thLineHeight - 3*m_nLineSpace;
    int y = 0;
    foreach (QString entry, entries)
    {
        painter->setFont(m_bodyFont);
        int height = entryHeight(painter, entry);
        if (y + height > pageHeight && !currentPage.empty())
        {
            pages->append(currentPage);
            currentPage.clear();
            y = 0;
        }
        currentPage.append(entry);
        //y += height + MediumGap;
        y += height + m_nLineSpace;
    }
    if (!currentPage.empty())
        pages->append(currentPage);
}

int CFormPrinter::entryHeight(QPainter* painter, const QString& entry)
{
    // 打印纸宽度减去一行前后两个页边距，等于可用的文本宽度
    int textWidth = painter->window().width() - 2*m_nPageSpace;
    int maxHeight = painter->window().height();
    QRect bodyRect = painter->boundingRect(m_nPageSpace, m_nLineSpace, textWidth, maxHeight, Qt::TextWordWrap, entry);
    return (bodyRect.height());
}

void CFormPrinter::printPages(QPainter* painter, const QList<QStringList>& pages)
{
    int firstPage = m_pPrinter->fromPage() - 1;
    if (firstPage >= pages.size())
        return;
    if (firstPage == -1)
        firstPage = 0;
    int lastPage = m_pPrinter->toPage() - 1;
    if (lastPage == -1 || lastPage >= pages.size())
        lastPage = pages.size() - 1;
    int numPages = lastPage - firstPage + 1;
    for (int i = 0; i < m_pPrinter->numCopies(); ++i)
    {
        for (int j = 0; j < numPages; ++j)
        {
            if (i != 0 || j != 0)
                m_pPrinter->newPage();
            int index;
            if (m_pPrinter->pageOrder() == QPrinter::FirstPageFirst)
            {
                index = firstPage + j;
            }
            else
            {

                index = lastPage - j;
            }
            printPage(painter, pages[index], index + 1);
        }
    }
}

void CFormPrinter::printPage(QPainter* painter, const QStringList& entries, int pageNumber)
{
    painter->save();
    painter->translate(m_nPageSpace, m_nLineSpace);

    painter->setFont(m_titleFont);

    // printHeader()返回的是头部所在的矩形
    QRect headerRc = printHeader(painter, pageNumber);
    int nStartY = m_nLineSpace + headerRc.bottom();

    painter->setFont(m_bodyFont);
    QRect footerRc = printFooter(painter, m_strTotalWeight);

    painter->setFont(m_bodyFont);
    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * m_nPageSpace;
    int maxHeight = painter->window().height();
    int avgWidth = textWidth/3;

    painter->drawRect(QRect(m_nPageSpace, nStartY, textWidth-2*m_nPageSpace, maxHeight-nStartY-m_nLineSpace*3-footerRc.height()));

    int i = 0;
    foreach (QString entry, entries)
    {
        QStringList fields = entry.split(":");
        if (fields.size() >= 3)
        {
            QString strName = fields[0];
            QString strSize = fields[1];
            QString strNum = fields[2];

            QRect rect = painter->boundingRect(m_nPageSpace, m_nLineSpace, textWidth, maxHeight, Qt::TextWordWrap, strName);
            int width = rect.width() > avgWidth ? avgWidth : rect.width();
            QRect nameRect(m_nPageSpace+(avgWidth-width)/2, nStartY+rect.height()*i+m_nLineSpace*2, width, rect.height());
            painter->drawText(nameRect, Qt::AlignHCenter | Qt::AlignBottom, strName);

            rect = painter->boundingRect(m_nPageSpace, m_nLineSpace, textWidth, maxHeight, Qt::TextWordWrap, strNum);
            width = rect.width() > avgWidth ? avgWidth : rect.width();
            QRect numRect(2*avgWidth+m_nPageSpace+(avgWidth-width)/2, nStartY+rect.height()*i+m_nLineSpace*2, width, rect.height());
            painter->drawText(numRect, Qt::AlignHCenter | Qt::AlignBottom, strNum);

            rect = painter->boundingRect(m_nPageSpace, m_nLineSpace, textWidth, maxHeight, Qt::TextWordWrap, strSize);
            width = rect.width() > avgWidth ? avgWidth : rect.width();
            QRect sizeRect(avgWidth+m_nPageSpace+(avgWidth-width)/2, nStartY+rect.height()*i+m_nLineSpace*2, width, rect.height());
            painter->drawText(sizeRect, Qt::AlignHCenter | Qt::AlignBottom, strSize);

        }
        ++i;
        //painter->translate(0, MediumGap);
    }

    painter->restore();

}

QRect CFormPrinter::printHeader(QPainter* painter, int pageNumber)
{
    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * m_nPageSpace;
    int maxHeight = painter->window().height();

    painter->setFont(m_bodyFont);
    QString strPageNumber = QString("%1/%2").arg(pageNumber).arg(m_nTotalPages);

    QRect pageNumRect = painter->boundingRect(m_nPageSpace, m_nLineSpace,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, strPageNumber);
    painter->drawText(QRect(textWidth-pageNumRect.width()-m_nPageSpace, pageNumRect.y(), pageNumRect.width()-m_nPageSpace, pageNumRect.height()),
                      Qt::TextWordWrap, strPageNumber);


    painter->setFont(m_titleFont);
    QRect companyNameRect = painter->boundingRect(m_nPageSpace, m_nLineSpace,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, m_strCompanyName);
    int nLineY = pageNumRect.bottom()+ m_nLineSpace;
    painter->drawText(QRect((textWidth-pageNumRect.width()-companyNameRect.width())/2, companyNameRect.y(), companyNameRect.width(), companyNameRect.height()),
                      Qt::TextWordWrap, m_strCompanyName);


    painter->drawLine(QPoint(m_nPageSpace, nLineY), QPoint(textWidth-m_nPageSpace, nLineY));


    painter->setFont(m_bodyFont);
    QRect clientNameRect = painter->boundingRect(m_nPageSpace, m_nLineSpace + nLineY,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, m_strClientName);


    painter->drawText(clientNameRect, Qt::TextWordWrap, m_strClientName);


    QRect phoneNumberRect = painter->boundingRect(m_nPageSpace, m_nLineSpace + nLineY,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, m_strClientPhoneNumber);

    painter->drawText(QRect(textWidth-phoneNumberRect.width()-m_nPageSpace, clientNameRect.y(), phoneNumberRect.width(), phoneNumberRect.height()),
                      Qt::TextWordWrap, m_strClientPhoneNumber);


    QRect clientAddressRect = painter->boundingRect(m_nPageSpace, m_nLineSpace + clientNameRect.bottom(),
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, m_strClientAddress);

    painter->drawText(clientAddressRect, Qt::TextWordWrap, m_strClientAddress);

    return QRect(m_nPageSpace, m_nLineSpace, textWidth, clientAddressRect.bottom());
}

QRect CFormPrinter::printFooter(QPainter* painter, const QString& text)
{
    painter->setFont(m_bodyFont);

    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * m_nPageSpace;
    int maxHeight = painter->window().height();

    int nTextHeight = entryHeight(painter, text);

    QRect textRect = painter->boundingRect(m_nPageSpace, maxHeight - (2*m_nLineSpace + nTextHeight),
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, text);
    painter->drawText(QRect(textWidth-textRect.width()-m_nPageSpace, textRect.y(), textRect.width(), textRect.height()),
                      Qt::TextWordWrap, text);

    return textRect;
}

void CFormPrinter::printBox(QPainter* painter, const QString& str, const QFont& font, const QBrush& brush)
{
    painter->setFont(font);
    int boxWidth = painter->window().width();
    int textWidth = boxWidth - 2 * SmallGap;
    int maxHeight = painter->window().height();
    QRect textRect = painter->boundingRect(SmallGap, SmallGap,
                                           textWidth, maxHeight,
                                           Qt::TextWordWrap, str);
    int boxHeight = textRect.height() + 2 * SmallGap;
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(brush);
    painter->drawRect(0, 0, boxWidth, boxHeight);
    painter->drawText(textRect, Qt::TextWordWrap, str);
    painter->translate(0, boxHeight);
}
