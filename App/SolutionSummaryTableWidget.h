#ifndef SOLUTIONSUMMARYTABLEWIDGET_H
#define SOLUTIONSUMMARYTABLEWIDGET_H

#include <QTableWidget>
#include "CuttingSolutionTable.h"

class SolutionSummaryTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit SolutionSummaryTableWidget(QWidget *parent = 0);
    ~SolutionSummaryTableWidget();

    void setTableHeader(const QStringList& sl);
    void displayData(const CuttingSolutionSt& csSt, const SourceBoardSt& sbSt, int nSpace);
    void removeRowData();

signals:

public slots:
};

#endif // SOLUTIONSUMMARYTABLEWIDGET_H
