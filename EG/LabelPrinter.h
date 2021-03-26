#ifndef LABELpRINTER_H
#define LABELpRINTER_H

#include "eg_global.h"
#include <QObject>
#include <QPrinter>
#include <QPainter>
#include <QPixmap>
#include <vector>
#include "Typedef.h"
#include "LabelDrawer.h"


class EGSHARED_EXPORT LabelPrinter
{
public:
    LabelPrinter(QPrinter* printer,
                 const std::vector<BoardSt>& vecSt,
                 int nTemplate = 0,
                 int nLTContext = 0,
                 double dEdgeMargin = 10.0,
                 double dLineSpacing = 4.0,
                 double dLogoSize = 50.0,
                 double dBoardSize = 50.0,
                 double dQRCodeSize = 50.0);

    ~LabelPrinter();

    void run();

    void setPrinter(QPrinter* printer);
    void setLabelTemplate(int nTemplate);
    void setLeftTopContext(int nLTContext);
    void setEdgeMargin(double dMargin);
    void setLineSpacing(double dSpacing);
    void setLogoSize(double dLogoSize);
    void setBoardSize(double dBoardSize);
    void setQRCodeSize(double dQRCodeSize);

private:
    const std::vector<BoardSt>& m_vecSt;
    int m_nTemplate;
    int m_nLTContext;
    LabelDrawer* m_pDrawer;
    QPrinter* m_pPrinter;
    QPainter* m_pPainter;
    QImage m_logoImage;
};

#endif // LABELpRINTER_H
