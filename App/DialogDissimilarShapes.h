#ifndef DIALOGDISSIMILARSHAPES_H
#define DIALOGDISSIMILARSHAPES_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLabel>
#include <QTableWidget>
#include <map>
#include <QVariant>
#include "DissimilarPreviewer.h"
#include "Typedef.h"

namespace Ui {
class DialogDissimilarShapes;
}

class DialogDissimilarShapes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDissimilarShapes(const QString& strName, double dWidth, double dHeight, int specifiedID = -2, QWidget *parent = 0);
    ~DialogDissimilarShapes();

    void setSpecifiedID(int nID);
    void createWidgets();
    bool getDissimilarParameters(QString& strParams);

protected:
    void readDissimilarFromLibrary();
    void displayDisimilars2ListWidget(const std::vector<DissimilarSt>& vecSt);
    void displayParameters2TableWidget(int nID, const QStringList& slKeys, std::map<QString, QVariant>& paramsMap);
    void showMessageOnLabel(int nID);
    bool verrifyParameters(int nID, std::map<QString, QVariant>& paramsMap);

private slots:
    void onLastPageButtonClicked(bool checked = false);
    void onNextPageButtonClicked(bool checked = false);
    void onApplyButtonClicked(bool checked = false);
    void onOkButtonClicked(bool checked = false);
    void onCancelButtonClicked(bool checked = false);
    void onDissimilarItemClicked(QListWidgetItem* pItem);
    void onParametersTableWidgetItemChanged(QTableWidgetItem* pItem);

private:
    Ui::DialogDissimilarShapes *ui;
    QTableWidget* m_pTableWidgetParameters;
    QLabel* m_pLabelcomment;
    QListWidget* m_pListWidgetDissimilar;
    QToolButton* m_pToolButtonLastPage;
    QToolButton* m_pToolButtonNextPage;
    QToolButton* m_pToolButtonApply;
    QLineEdit* m_pLineEditCurrentPageIndex;
    QLabel* m_pLabelTotalPage;
    DissimilarPreviewer* m_pDissimilarPreviewer;
    QPushButton* m_pPushButtonOk;
    QPushButton* m_pPushButtonCancel;

    std::vector<DissimilarSt> m_vecDissimilarSt;
    std::size_t m_nPageSize;
    std::size_t m_nCurrentPageIndex;
    std::size_t m_nTotalPage;

    QString m_strName;
    double m_dWidth;
    double m_dHeight;
};

#endif // DIALOGDISSIMILARSHAPES_H
