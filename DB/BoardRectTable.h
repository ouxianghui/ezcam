#ifndef BOARDRECTTABLE_H
#define BOARDRECTTABLE_H

#include "db_global.h"
#include <QString>
#include <vector>
#include <string>
#include <QSqlError>

typedef struct BroadRectSt {
    QString strBoardRectID;
    QString strSourceBoardID;
    QString strBoardRectName;
    QString strBarcodeNo;
    int nIdx;
    double dLtX;
    double dLtY;
    double dRbX;
    double dRbY;
    double dNcStartPtX;
    double dNcStartPtY;
    BroadRectSt()
    {
        strBoardRectID = "";
        strSourceBoardID = "";
        strBoardRectName = "";
        strBarcodeNo = "";
        nIdx = 0;
        dLtX = 0.0;
        dLtY = 0.0;
        dRbX = 0.0;
        dRbY = 0.0;
        dNcStartPtX = 0.0;
        dNcStartPtY = 0.0;
    }

    // 升序排序
    bool operator < (const BroadRectSt& st)
    {
        return this->nIdx < st.nIdx;
    }

}BoardRectSt;

class DBSHARED_EXPORT BoardRectTable
{
public:
    BoardRectTable();
    ~BoardRectTable();

    static bool addBoardRects(std::vector<BoardRectSt>& vecRect);
    static bool getBoardRectsBySourceBoardID(const QString& strSourceBoardID, std::vector<BoardRectSt>& vecRect);
    static bool updateBoardRectIndexByID(const QString& strBoardRectID, int nNewIndex);
    static bool updateBoardCuttingPointByID(const QString& strBoardRectID, double dNcStartPtX, double dNcStartPtY);
    static bool deleteBoardRectsBySourceBoardID(const QString& strSourceBoardID);
private:
    static std::string BoardRectTable::errorMsg(const QSqlError& error);
};

#endif // BOARDRECTTABLE_H
