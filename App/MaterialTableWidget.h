#ifndef MATERIALTABLEWIDGET_H
#define MATERIALTABLEWIDGET_H

#include <QObject>
#include <QTableWidget>
#include <vector>
#include <QStringList>
#include <QContextMenuEvent>
#include <QMenu>
#include <QAction>
#include "DataSt.h"

class MaterialTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit MaterialTableWidget(QWidget *parent = 0);
    ~MaterialTableWidget();

    void setTableHeader(const QStringList& sl);
    void setMaterialListName(const QString& strMaterialListName);
    void displayData(const std::vector<DataSt>& vecData);
    void removeRowData();

protected:
    void contextMenuEvent(QContextMenuEvent* event);
    void onActionAddDissimilar(bool checked = false);
    void onActionAddCustomDissimilar(bool checked = false);
    void onActionDelDissimilar(bool checked = false);
    void extractParameters(const QString& str, std::map<QString, QVariant>& paramsMap);

signals:
    void updateDissimilar(const QString& strMaterialListName, const QString& strBarcodeNo, const QString& strParameters);

public slots:

private:
    QMenu* m_pMenu;
    QAction* m_pActionAddDissimilar;
    QAction* m_pActionAddCustomDissimilar;
    QAction* m_pActionDelDissimilar;
    QString m_strMaterialListName;
};

#endif // MATERIALTABLEWIDGET_H
