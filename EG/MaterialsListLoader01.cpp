#include "MaterialsListLoader01.h"
#include "ExcelEngine.h"
#include <QException>
#include <QTextCodec>

MaterialsListLoader01::MaterialsListLoader01()
{
    CExcelDataLoader::setStartRow(2);
}

MaterialsListLoader01::~MaterialsListLoader01()
{

}

int MaterialsListLoader01::recogniseDocumentFormat()
{
    try
    {
        if (m_excelEngine.GetRowCount() <= 1)
            return 1;

        if (m_excelEngine.GetColumnCount() < 20)
            return 2;

        QTextCodec *tc = QTextCodec::codecForName("GBK");
        bool isRightFormat = false;
        QString strTmp;
        QVariant var = m_excelEngine.GetCellData(1, 1);
        strTmp = var.toString().remove(' ');
        if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("订单号") || strTmp == "订单号"))
        {
            var = m_excelEngine.GetCellData(1, 3);
            strTmp = var.toString().remove(' ');
            if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("条码编号") || strTmp == "条码编号"))
            {
                var = m_excelEngine.GetCellData(1, 6);
                strTmp = var.toString().remove(' ');
                if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("开料长度") || strTmp == "开料长度"))
                {
                    var = m_excelEngine.GetCellData(1, 9);
                    strTmp = var.toString().remove(' ');
                    if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("需切数量") || strTmp == "需切数量"))
                    {
                        var = m_excelEngine.GetCellData(1, 14);
                        strTmp = var.toString().remove(' ');
                        if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("异型") || strTmp == "异型"))
                        {
                            var = m_excelEngine.GetCellData(1, 16);
                            strTmp = var.toString().remove(' ');
                            if (!var.isNull() && var.isValid() && (strTmp == tc->toUnicode("客户信息") || strTmp == "客户信息"))
                            {
                                isRightFormat = true;
                            }
                            else
                            {
                                return 2;
                            }
                        }
                        else
                        {
                            return 2;
                        }
                    }
                    else
                    {
                        return 2;
                    }
                }
                else
                {
                    return 2;
                }
            }
            else
            {
                return 2;
            }
        }
        else
        {
            return 2;
        }


        if (isRightFormat)
        {
            return 0;
        }
        else
        {
            return 2;
        }
    }
    catch (QException& e)
    {
        qFatal("%s", e.what());
        return 3;
    }
}

bool MaterialsListLoader01::findDataByNo(QString strNo, DataSt& st)
{
    try
    {
        if (m_vecData.empty())
        {
            getRecord();
        }

        bool isFound = false;
        std::vector<DataSt>::iterator beg = m_vecData.begin();
        std::vector<DataSt>::iterator end = m_vecData.end();
        for (std::vector<DataSt>::iterator it = beg; it != end; ++it)
        {
            DataSt dst = *it;
            if (dst.strBarCodeNo == strNo)
            {
                st = dst;
                isFound = true;
                break;
            }
        }

        return isFound;
    }
    catch (QException& e)
    {
        qFatal("%s", e.what());
        return false;
    }
}

std::vector<DataSt>& MaterialsListLoader01::getRecord()
{
    try
    {
        if (m_vecData.empty())
        {
            UINT nRowNum = m_excelEngine.GetRowCount();
            UINT nColumnNum = m_excelEngine.GetColumnCount() >= 17 ? 17 : m_excelEngine.GetColumnCount();
            bool ok = false;
            int nValue = 0;
            for (UINT i = m_nStartRow; i <= nRowNum; ++i)
            {
                bool canAddData = true;
                DataSt dataSt;
                dataSt.nRotate = 0;
                for (UINT j = 1; j <= nColumnNum; ++j)
                {
                    QVariant var = m_excelEngine.GetCellData(i, j);
                    if (!var.isNull() && var.isValid())
                    {
                        if (j == 1 && var.toString() == "END")
                        {
                            canAddData = false;
                            break;
                        }

                        if (j == 1)
                        {
                            dataSt.strOrdersNo = var.toString();
                        }
                        else if (j == 2)
                        {
                            dataSt.strBarCodeNo = var.toString();
                        }
                        else if (j == 3)
                        {
                            dataSt.strProductName = var.toString();
                        }
                        else if (j == 4)
                        {
                            dataSt.strFinishedProductName = var.toString();
                        }
                        else if (j== 5)
                        {
                            dataSt.strMaterialType = var.toString();
                        }
                        else if (j == 6)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nBoardHeight = nValue;
                            }
                            else
                            {
                                dataSt.nBoardHeight = 0;
                            }
                        }
                        else if (j == 7)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);

                            if (ok)
                            {
                                dataSt.nBoardWidth = nValue;
                            }
                            else
                            {
                                dataSt.nBoardWidth = 0;
                            }
                        }
                        else if (j == 8)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nBoardThickness = nValue;
                            }
                            else
                            {
                                dataSt.nBoardThickness = 0;
                            }
                        }
                        else if (j == 9)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nBoardNum = nValue;
                            }
                            else
                            {
                                dataSt.nBoardNum = 0;
                            }
                        }
                        else if (j == 10)
                        {
                            dataSt.strEdgeWeight = var.toString();
                        }
                        else if (j == 11)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nFinishedProductHeight = nValue;
                            }
                            else
                            {
                                dataSt.nFinishedProductHeight = 0;
                            }
                        }
                        else if (j == 12)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nFinishedProductWidth = nValue;
                            }
                            else
                            {
                                dataSt.nFinishedProductWidth = 0;
                            }
                        }
                        else if (j == 13)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nFinishedProductThickness = nValue;
                            }
                            else
                            {
                                dataSt.nFinishedProductThickness = 0;
                            }
                        }
                        else if (j == 14)
                        {
                            dataSt.strDissimilarShapes = var.toString();
                        }
                        else if (j == 15)
                        {
                            ok = false;
                            nValue = var.toInt(&ok);
                            if (ok)
                            {
                                dataSt.nRotate = nValue;
                            }
                            else
                            {
                                dataSt.nRotate = 0;
                            }
                        }
                        else if (j == 16)
                        {
                            dataSt.strClientInformation = var.toString();
                        }
                        else if (j == 17)
                        {
                            dataSt.strFranchiseStore = var.toString();
                        }
                    }
                }
                if (canAddData)
                    m_vecData.push_back(dataSt);
            }
        }
        return m_vecData;
    }
    catch (QException& e)
    {
        qFatal("%s", e.what());
        m_vecData.clear();
        return m_vecData;
    }
}
