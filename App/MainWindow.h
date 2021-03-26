#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeWidget>
#include <QLabel>
#include <QToolBar>
#include <QToolButton>
#include <QProgressDialog>
#include "DataSt.h"
#include "GraphicsViewer.h"
#include "WorkspaceTreeWidget.h"
#include "SourceBoardListWidget.h"
#include "ExcelDataLoader.h"
#include "MaterialTableWidget.h"
#include "Rect.h"
#include "DatabaseManager.h"
#include "MaterialListTable.h"
#include "CuttingSolutionTable.h"
#include "SourceBoardTable.h"
#include "BoardRectTable.h"
#include "ProductBoardTable.h"
#include "MaterialGroupTable.h"
#include "CentralWidget.h"
#include "SolutionSummaryTableWidget.h"
#include "ViewPort.h"
#include "Global.h"
#include "Typedef.h"
#include "QProgressHud.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //method
    enum {GUILLOTINE, MAXRECTS};

    //heuristic
    enum {NONE, TL, BAF, BSSF, BLSF, MINW, MINH, HEURISTIC_NUM};

    //sort
    enum {SORT_NONE, WIDTH, HEIGHT, SORT_AREA, SORT_MAX, SORT_NUM};

    //rotation
    enum {NEVER, ONLY_WHEN_NEEDED, H2_WIDTH_H, WIDTH_GREATHER_HEIGHT, WIDTH_GREATHER_2HEIGHT, W2_HEIGHT_W, HEIGHT_GREATHER_WIDTH, HEIGHT_GREATHER_2WIDTH, ROTATION_NUM};

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void createWidgets();

private slots:
    void on_actionNewProject_triggered();
    void on_actionOpenProject_triggered();
    void on_actionSystemSettings_triggered();
    void on_actionImportMaterialFile_triggered();
    void on_actionPacking_triggered();
    void on_actionSaveProject_triggered();
    void on_actionExportCNC_triggered();
    void on_actionImportDXF_triggered();
    void on_actionExportDXF_triggered();
    void on_actionPrintDrawing_triggered();
    void on_actionPrintLabel_triggered();
    void on_actionBatchPrintLabel_triggered();
    void on_actionPrintSettings_triggered();

    void onWorkSpaceTreeItemClicked(QTreeWidgetItem* pItem, int nColumn);
    void onWorkSpaceTreeItemDoubleClicked(QTreeWidgetItem* pItem, int column);
    void onDeleteMaterialList(QString strMaterialListName);
    void onDeleteCuttingSolution(QString strSolutionID);
    void onItemSelectionChanged();

    void on_actionExportAllBoardCNC_triggered();

    void on_actionQuit_triggered();

    void checkDongleTimeout();

    void on_actionAbout_triggered();

protected:
    void closeEvent(QCloseEvent*);
    void contextMenuEvent(QContextMenuEvent*);
    bool getDissimilarParametersByBarcodeNo(const QString& strBarcodeNo, std::map<QString, QVariant>& paramsMap);

private:
    void readSettings();
    void saveSettings();

    bool isMaterialFileAlreadyExistsInProjectDir(const QString& strFilePathName);

    bool copyMaterialFile2ProjectDir(const QString& strFilePathName);

    bool openMaterialFile(const QString& strFilePathName, std::vector<DataSt>& vecData);

    void splitMaterialList(const QString& strKey, const std::vector<DataSt>& vecData);

    std::vector<Solution> packing(const std::vector<DataSt>& vecData);

    void guillotinePacking(int nSourceMaterialBoardWidth,
                           int nSourceMaterialBoardHeight,
                           int nBoardSpacing,
                           const Solution& solution,
                           const std::vector<rbp::RectSize>& vecRects,
                           bool bAllBoardsSupportRotate,
                           std::vector<Solution>& vecResult,
                           int& nMinBoardNum);

    void maxRectsPacking(int nSourceMaterialBoardWidth,
                         int nSourceMaterialBoardHeight,
                         int nBoardSpacing,
                         const Solution& solution,
                         const std::vector<rbp::RectSize>& vecRects,
                         bool bAllBoardsSupportRotate,
                         std::vector<Solution>& vecResult,
                         int& nMinBoardNum);

    void skylinePacking(int nSourceMaterialBoardWidth,
                         int nSourceMaterialBoardHeight,
                         int nBoardSpacing,
                         const Solution& solution,
                         const std::vector<rbp::RectSize>& vecRects,
                         bool bAllBoardsSupportRotate,
                         std::vector<Solution>& vecResult,
                         int& nMinBoardNum);

    // 板块尺寸合法性检查
    bool materialBoardSizeTest(int nMaxWidth, int nMaxHeight, int nSpace, const std::vector<DataSt>& vecData);

    // 料单合法性检查（板块尺寸的合法性）
    bool materialListTest();

    bool materialTest(const std::vector<DataSt>& vecData);

    // 仅在优化排版后调用，生成各个料板的dxf文件
    bool generateDxf(const QString& strFilePathName,
                     const std::vector<RectF>& vecRect,
                     int nWidth,
                     int nHeight,
                     int nDepth = 0);

    void arrangeBoards(int nSortMethod, int nRotateCondition, std::vector<rbp::RectSize>& vecSize);

    void swapRectSizeWH(rbp::RectSize& rcSz);

public slots:
    void resetActionToolButtonClicked(bool checked = false);
    void modifyCuttingPointToolButtonClicked(bool checked = false);
    void editNCOrderToolButtonClicked(bool checked = false);
    void selectBoardToolButtonClicked(bool checked = false);
    void onUpdateDissimilar(const QString& strMaterialListName, const QString& strProductBoardID, const QString& strParameters);

private:
    Ui::MainWindow *ui;
    QToolBar* m_pCADToolBar;
    ViewPort* m_pViewPort;

    QDockWidget* m_pWorkSpaceDockWidget;
    QDockWidget* m_pSourceBoardListDockWidget;
    WorkspaceTreeWidget* m_pWorkSpaceTreeWidget;
    SourceBoardListWidget* m_pSourceBoardListWidget;
    CentralWidget* m_pCentralWidget;
    SolutionSummaryTableWidget* m_pSolutionSummaryTableWidget;

    QToolButton* m_pResetActionButton;
    QToolButton* m_pHidenInformationLayerButton;
    QToolButton* m_pHidenCuttingPointLayerButton;
    QToolButton* m_pModifyCuttingPointToolButton;
    QToolButton* m_pEditNCOrderToolButton;
    QToolButton* m_pSelectBoardToolButton;
    QToolButton* m_pResetViewportToolButton;

    CExcelDataLoader* m_pExcelDataLoader;

    // 待开料单及其子料单的显示控件
    MaterialTableWidget* m_pMaterialTableWidget;

    // 按材料类型、厚度拆分后的料单数据（工程加载后常驻内存，用于“待开料单”显示和优化排版）
    std::map<QString, std::map<QString, std::vector<DataSt>>> m_materialMap;

    // key:方案ID
    std::map<QString, Solution> m_solutionMap;

    // key:原料板ID
    std::map<QString, std::vector<BoardRectSt>> m_boardRectsMap;
};

#endif // MAINWINDOW_H
