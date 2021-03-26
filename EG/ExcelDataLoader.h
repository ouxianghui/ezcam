#ifndef CEXCELDATALOADER_H
#define CEXCELDATALOADER_H

#include "eg_global.h"
#include <QString>
#include <vector>
#include "DataSt.h"
#include "ExcelEngine.h"

//typedef unsigned int UINT;

class  EGSHARED_EXPORT CExcelDataLoader
{
public:
    CExcelDataLoader();
    CExcelDataLoader(QString strFileName);
    ~CExcelDataLoader();

    void setStartRow(UINT nStartRow = 8);
    bool openExcel(UINT nSheet = 1, bool visible = false);
    bool openExcel(QString strFileName, UINT nSheet = 1, bool visible = false);

    // 返回值：0，正确的格式；1，文件内容为空；2，格式不正确；3，未知错误
    virtual int recogniseDocumentFormat() = 0;
    virtual bool findDataByNo(QString strNo, DataSt& st) = 0;
    virtual std::vector<DataSt>& getRecord() = 0;

    void close();

protected:
    UINT m_nStartRow;
    QString m_strFileName;
    std::vector<DataSt> m_vecData;
    ExcelEngine m_excelEngine;
};

#endif // CEXCELDATALOADER_H
