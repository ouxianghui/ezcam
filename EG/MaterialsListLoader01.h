#ifndef MATERIALSLISTLOADER01_H
#define MATERIALSLISTLOADER01_H

#include "eg_global.h"
#include "ExcelDataLoader.h"

class EGSHARED_EXPORT MaterialsListLoader01 : public CExcelDataLoader
{
public:
    MaterialsListLoader01();
    ~MaterialsListLoader01();

    void setStartRow(UINT nStartRow = 8);
    bool openExcel(UINT nSheet = 1, bool visible = false);
    bool openExcel(QString strFileName, UINT nSheet = 1, bool visible = false);

    // 返回值：0，正确的格式；1，文件内容为空；2，格式不正确；3，未知错误
    int recogniseDocumentFormat();
    virtual bool findDataByNo(QString strNo, DataSt& st);
    virtual std::vector<DataSt>& getRecord();
};

#endif // MATERIALSLISTLOADER01_H
