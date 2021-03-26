#ifndef CNCGENERATOR_H
#define CNCGENERATOR_H

#include "eg_global.h"
#include <QMap>
#include <QVariant>
#include "RS.h"
#include "RPolylineData.h"
#include "RVector.h"
#include "RArc.h"
#include "RDocument.h"
#include "RDocumentInterface.h"

class EGSHARED_EXPORT CNCGenerator
{
public:
    CNCGenerator(int nCuttingSpeed,
                 int nProcessSpeed,
                 int nTerminalSpeed,
                 double dSafetyHeight,
                 double dCuttingDepth,
                 double dKnifeDiameter,
                 const QString& slotKnifeType,
                 int nUseDrills,
                 const QMap<QString, QVariant>& drillsMap,
                 const QMap<QString, QVariant>& holesMap,
                 double dSourceBoardW,
                 double dSourceBoardH,
                 const QString& knifeChangeMode,
                 const QString& strNCFileHeader,
                 const QString& strNCFileTail);

    ~CNCGenerator();

    /* return :
     * 0, 成功
     * 1, 在折线中插入下刀点失败：原折线不是闭合的
     * 2, 在折线中插入下刀点失败：圆折线线段数量小于3
     * 3, 在折线中插入下刀点失败：下刀点不在折线上
     * 4, 在折线中插入下刀点失败：不允许下刀点在圆弧上
     * 5, 在折线中插入下刀点失败：未知错误
     * 6, 动态转换实体失败
     * 7, 拉槽刀具T2无法加工拉槽
     * 8, 拉槽刀具T3无法加工拉槽
     * 9, 无法打开一个nc文件用以写入G代码
     */
    int generateCNC(RDocument& document, const QString& strSourceBoardID, const QString strOutputPathName);

    bool generateCNC(RDocumentInterface* documentInterface, const QString& strSourceBoardID, const QString strOutputPathName);

private:
    /* 函数返回值:
     * 0, 成功重建折线
     * 1, 折线不是闭合的
     * 2, 折线线段数量小于3
     * 3, 下刀点不在折线上
     * 4, 下刀点在圆弧上
     */
    int rebuildPolylineDataWithCuttingPoint(RPolylineData& data, const RVector& ptCutting);
    bool generateArc(const RVector& pt1, const RVector& pt2, double dBulge, RArc& arc);

    int processGeneralInnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline);
    int processID007InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap);
    int processID008InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap);
    int processID013InnerShape(QTextStream& ncStream, QSharedPointer<RPolyline> orgPolyline, std::map<QString, QVariant>& paramsMap);


private:
    int m_nCuttingSpeed;
    int m_nProcessSpeed;
    int m_nTerminalSpeed;
    double m_dSafetyHeight;
    double m_dCuttingDepth;
    double m_dKnifeDiameter;
    QString _slotKnifeType;
    int m_nUseDrills;
    QMap<QString, QVariant> _drillsMap;
    QMap<QString, QVariant> _holesMap;
    double m_dSourceBoardW;
    double m_dSourceBoardH;
    QString _knifeChangeMode;
    QString m_strNCFileHeader;
    QString m_strNCFileTail;

    QString _currentKnife;
};

#endif // CNCGENERATOR_H
