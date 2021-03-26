#ifndef LABELDRAWER_H
#define LABELDRAWER_H

#include "eg_global.h"
#include <QObject>
#include <QImage>
#include "Typedef.h"

class EGSHARED_EXPORT LabelDrawer
{
public:
    LabelDrawer(double dEdgeMargin = 11.0,
                double dLineSpacing = 4.0,
                double dLogoSize = 50.0,
                double dBoardSize = 50.0,
                double dQRCodeSize = 50.0);
    ~LabelDrawer();

    // 生成宽6cm，高4cm的标签图像
    QImage generateW6H4LabelImage(const BoardSt& st, int nLTContext);
    QImage generateW5H3LabelImage(const BoardSt& st);
    QImage generateW4H3LabelImage(const BoardSt& st);

    // 绘制宽6cm，高4cm的标签
    void drawW6H4Label(QPainter& painter, const BoardSt& st, int nLTContext);
    void drawW5H3Label(QPainter& painter, const BoardSt& st);
    void drawW4H3Label(QPainter& painter, const BoardSt& st);

    void setEdgeMargin(double dMargin);
    void setLineSpacing(double dSpacing);
    void setLogoSize(double dLogoSize);
    void setBoardSize(double dBoardSize);
    void setQRCodeSize(double dQRCodeSize);

private:
    double m_dEdgeMargin;
    double m_dLineSpacing;

    double m_dLogoSize;
    double m_dBoardSize;
    double m_dQRCodeSize;

    QImage m_logoImage;
};

#endif // LABELDRAWER_H
