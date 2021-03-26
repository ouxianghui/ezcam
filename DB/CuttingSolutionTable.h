#ifndef CUTTINGSOLUTIONTABLE_H
#define CUTTINGSOLUTIONTABLE_H

#include "db_global.h"
#include <QString>
#include <vector>
#include "SourceBoardTable.h"

typedef struct CuttingSolutionSt {
    QString strSolutionID;
    QString strSolutionName;
    QString strMateriakGroupID;
    double dTotalTargetArea;
    int nSourceBoardWidth;
    int nSourceBoardHeight;
    int nSourceBoardThickness;
    int nTotalSourceBoardNum;
    double dTotalUtilizationRate;

    CuttingSolutionSt()
    {
        strSolutionID = "";
        strSolutionName = "";
        strMateriakGroupID = "";
        dTotalTargetArea = 0.0;
        nSourceBoardWidth = 0;
        nSourceBoardHeight = 0;
        nSourceBoardThickness = 0;
        nTotalSourceBoardNum = 0;
        dTotalUtilizationRate = 0.0;
    }
}CuttingSolutionSt;

class DBSHARED_EXPORT CuttingSolutionTable
{
public:
    CuttingSolutionTable();
    ~CuttingSolutionTable();

    static bool addSolution(CuttingSolutionSt& st);
    static bool addSolutions(std::vector<CuttingSolutionSt>& vecData);
    static bool getSolutionsByMaterialGroupID(const QString& strGroupID, std::vector<CuttingSolutionSt>& vecData);
    static bool deleteSolutionByMaterialGroupID(const QString& strGroupID);
    static bool deleteSolutionByID(const QString& strID);

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // CUTTINGSOLUTIONTABLE_H
