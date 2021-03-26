#ifndef CFORMPRINTER_H
#define CFORMPRINTER_H

#include <QString>
#include <QStringList>
#include <QPrinter>
#include <QPainter>
#include <QFont>
#include <QImage>

class CFormPrinter
{
public:
    CFormPrinter();
    ~CFormPrinter();
    void init(const QString& strCompanyName, const QString& strClientName,
                 const QString& strClientAddress, const QString& strClientPhoneNumber);
    void setCompanyName(const QString& strCompanyName);
    void setClientName(const QString& strClientName);
    void setClientPhoneNumber(const QString& strClientAddress);
    void setClientAddress(const QString& strClientPhoneNumber);
    void printForm(const QStringList& stringList);
    void setWeight(const QString& strWeight);
    void setTitleFont(const QFont& font);
    void setBodyFont(const QFont& font);
    void setLineSpace(int nSpace);
    void setPageSpace(int nSpace);
    void setPageOrientation(int nOrientation);

    QPrinter* getPrinter();

protected:
    void paginate(QPainter* painter, QList<QStringList> *pages, const QStringList& entries);
    int entryHeight(QPainter* painter, const QString& entry);
    void printPages(QPainter* painter, const QList<QStringList>& pages);
    void printPage(QPainter* painter, const QStringList& entries, int pageNumber);
    QRect printHeader(QPainter* painter, int pageNumber);
    QRect printFooter(QPainter* painter, const QString& text);
    void printBox(QPainter* painter, const QString& str, const QFont& font, const QBrush& brush);

private:
    QPrinter* m_pPrinter;
    QFont m_titleFont;
    QFont m_bodyFont;
    QFont m_footerFont;
    int m_nLineSpace;
    int m_nPageSpace;
    int m_nPageOrientation;

    QString m_strCompanyName;
    QString m_strClientName;
    QString m_strClientAddress;
    QString m_strClientPhoneNumber;
    QString m_strTotalWeight;
    int m_nTotalPages;
};

#endif // CFORMPRINTER_H
