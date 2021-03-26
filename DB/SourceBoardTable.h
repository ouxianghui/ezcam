#ifndef SOURCEBOARDTABLE_H
#define SOURCEBOARDTABLE_H

#include "db_global.h"
#include <QString>
#include <vector>
#include "Rect.h"


typedef struct SourceBoardSt {
    QString strBoardID;
    QString strSolutionID;
    int nWidth;
    int nHeight;
    int nThickness;
    int nIdx;
    int nNum;
    double dUtilizationArea;
    double dUtilizationRate;
    QString strDxfFileName;

    // 他用，料板中包含的小板件
    std::vector<rbp::Rect> vecRect;

    SourceBoardSt()
    {
        strBoardID = "";
        strSolutionID = "";
        nWidth = 0;
        nHeight = 0;
        nThickness = 0;
        nIdx = 0;
        nNum = 0;
        dUtilizationArea = 0.0;
        dUtilizationRate = 0.0;
        strDxfFileName = "";
        vecRect.clear();
    }
}SourceBoardSt;

class DBSHARED_EXPORT SourceBoardTable
{
public:
    SourceBoardTable();
    ~SourceBoardTable();

    static bool addBoards(std::vector<SourceBoardSt>& vecData);
    static bool getBoardByID(const QString& strID, SourceBoardSt& srcSt);
    static bool getBoardsBySolutionID(const QString& strID, std::vector<SourceBoardSt>& vecData);
    static bool deleteBoardsBySolutionID(const QString& strID);

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // SOURCEBOARDTABLE_H
