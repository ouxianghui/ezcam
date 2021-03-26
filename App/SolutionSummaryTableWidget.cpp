#include "SolutionSummaryTableWidget.h"
#include <QHeaderView>
#include <QTextCodec>

SolutionSummaryTableWidget::SolutionSummaryTableWidget(QWidget *parent)
    : QTableWidget(parent)
{
    this->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

SolutionSummaryTableWidget::~SolutionSummaryTableWidget()
{

}

void SolutionSummaryTableWidget::setTableHeader(const QStringList& sl)
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

void SolutionSummaryTableWidget::removeRowData()
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

void SolutionSummaryTableWidget::displayData(const CuttingSolutionSt& csSt, const SourceBoardSt& sbSt, int nSpace)
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");
    int nRow = 1;

    //插入新数据
    for (int r = 0; r < nRow; ++r)  //行
    {
        this->insertRow(r); //插入新行
        for (int c = 0; c < this->columnCount(); ++c )  //列
        {
            if (c == 0)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%2").arg(csSt.dTotalTargetArea/1000000.0f).arg(tc->toUnicode("㎡")));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 1)
            {
                QString strMsg = QString(tc->toUnicode("%1%2%3%4%5 %6 张"))
                        .arg(csSt.nSourceBoardWidth)
                        .arg(tc->toUnicode("×"))
                        .arg(csSt.nSourceBoardHeight)
                        .arg(tc->toUnicode("×"))
                        .arg(csSt.nSourceBoardThickness)
                        .arg(csSt.nTotalSourceBoardNum);
                QTableWidgetItem* pItem = new QTableWidgetItem(strMsg);
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 2)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%2").arg(sbSt.dUtilizationArea/1000000.0f).arg(tc->toUnicode("㎡")));
                        pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 3)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%").arg(sbSt.dUtilizationRate*100));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 4)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1mm").arg(nSpace));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
            else if (c == 5)
            {
                QTableWidgetItem* pItem = new QTableWidgetItem(QString("%1%").arg(csSt.dTotalUtilizationRate*100));
                pItem->setTextAlignment(Qt::AlignCenter);
                this->setItem(r, c, pItem);
            }
        }
    }
}
