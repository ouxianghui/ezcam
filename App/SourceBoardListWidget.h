#ifndef SOURCEBOARDLISTWIDGET_H
#define SOURCEBOARDLISTWIDGET_H

#include <QTableWidget>
#include "SourceBoardTable.h"

class SourceBoardListWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit SourceBoardListWidget(QWidget *parent = 0);
    ~SourceBoardListWidget();

    void setTableHeader(const QStringList& sl);
    void displayData(const std::vector<SourceBoardSt>& vecData);
    void removeRowData();

signals:

public slots:
};

#endif // SOURCEBOARDLISTWIDGET_H
