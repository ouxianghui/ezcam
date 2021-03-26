#include "SourceBoardListWidget.h"
#include <QHeaderView>
#include <QTextCodec>
#include "Global.h"

SourceBoardListWidget::SourceBoardListWidget(QWidget *parent)
    : QTableWidget(parent)
{
    this->setEditTriggers(QTableWidget::NoEditTriggers);
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(SingleSelection);
}

SourceBoardListWidget::~SourceBoardListWidget()
{

}

void SourceBoardListWidget::setTableHeader(const QStringList& sl)
{
    // 先把table的内容清空
    int nColumn = this->columnCount();
    this->clear();
    for (int n = 0; n < nColumn; ++n)
    {
        this->removeColumn(0);
    }

    //重新创建表头
    this->setColumnCount(sl.size());
    this->setHorizontalHeaderLabels(sl);
}

void SourceBoardListWidget::removeRowData()
{
    int nRow = this->rowCount();
    int nCol = this->columnCount();

    for (int i = 0; i < nRow; ++i)
    {
        for (int j = 0; j < nCol; ++j)
        {
            QTableWidgetItem* pItem = this->item(i, j);
            delete pItem;
            pItem = NULL;
        }
        this->removeRow(0);
    }

    this->clearContents();
}

void SourceBoardListWidget::displayData(const std::vector<SourceBoardSt>& vecData)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    int nRow = vecData.size();

    //插入新数据
    for (int r = 0; r < nRow; ++r)  //行
    {
        const SourceBoardSt& dataSt = vecData.at(r);

        this->insertRow(r); //插入新行
        for (int c = 0; c < this->columnCount(); ++c )  //列
        {
            if (c == 0)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%2").arg(dataSt.dUtilizationArea/1000000.0f).arg(tc->toUnicode("㎡")));
                pItem->setData(CUTTING_SOLUTION_ID, dataSt.strSolutionID);
                pItem->setData(SOURCE_BOARD_ID, dataSt.strBoardID);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 1)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%").arg(dataSt.dUtilizationRate*100));
                pItem->setData(CUTTING_SOLUTION_ID, dataSt.strSolutionID);
                pItem->setData(SOURCE_BOARD_ID, dataSt.strBoardID);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 2)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1").arg(dataSt.nNum));
                pItem->setData(CUTTING_SOLUTION_ID, dataSt.strSolutionID);
                pItem->setData(SOURCE_BOARD_ID, dataSt.strBoardID);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
        }
    }
}
