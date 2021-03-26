#ifndef TYPEDEF
#define TYPEDEF

#include <string>
#include <QString>
#include "DataSt.h"
#include "CuttingSolutionTable.h"
#include "SourceBoardTable.h"
#include "MaterialGroupTable.h"

typedef struct RectF
{
    double x;
    double y;
    double width;
    double height;
    std::string barcodeNo;
    std::string name;
    int rotate;
    bool bRotated;
    double ctx;
    double cty;
    RectF()
    {
        x = 0.0;
        y = 0.0;
        width = 0.0;
        height = 0.0;
        barcodeNo = "";
        name = "";
        rotate = 0;
        bRotated = false;
        ctx = 0.0;
        cty = 0.0;
    }
}RectF;

typedef struct DissimilarSt
{
    int nID;
    QString strPathName;

    bool operator < (const DissimilarSt& st)
    {
        return this->nID < st.nID;
    }
}DissimilarSt;

typedef struct BoardSt
{
    // 大板序号
    int nSourceBoardIndex;

    // 小板序号
    int nBoardIndex;

    DataSt dataSt;

} BoardSt;

typedef struct Solution {
    CuttingSolutionSt solutionSt;
    std::vector<SourceBoardSt> vecSourceBoard;
}Solution;

typedef struct NCExporterSt {
    QString strMaterialFileName;
    MaterialGroupSt groupSt;
    CuttingSolutionSt solutionSt;
    SourceBoardSt BoardSt;
}NCExporterSt;

const double EPSINON = 1E-8;

const int PREVIEW_LABEL_SIZE = 240;

#endif // TYPEDEF

