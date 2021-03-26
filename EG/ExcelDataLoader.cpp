#include "ExcelDataLoader.h"
#include "ExcelEngine.h"
#include <QException>

CExcelDataLoader::CExcelDataLoader() : m_nStartRow(8)
{
    m_vecData.clear();
}

CExcelDataLoader::~CExcelDataLoader()
{

}

CExcelDataLoader::CExcelDataLoader(QString strFileName)
{
    m_strFileName = strFileName;
}

void CExcelDataLoader::setStartRow(UINT nStartRow)
{
    m_nStartRow = nStartRow;
}

bool CExcelDataLoader::openExcel(unsigned int nSheet, bool visible)
{
    if (m_strFileName.isEmpty())
    {
        return false;
    }
    else
    {
        m_vecData.clear();
        return m_excelEngine.Open(m_strFileName, nSheet, visible);
    }
}

bool CExcelDataLoader::openExcel(QString strFileName, unsigned int nSheet, bool visible)
{
    if (strFileName.isEmpty())
    {
        return false;
    }
    else
    {
        m_strFileName = strFileName;
        m_vecData.clear();
        return m_excelEngine.Open(m_strFileName, nSheet, visible);
    }
}

void CExcelDataLoader::close()
{
    m_excelEngine.Close();
}
