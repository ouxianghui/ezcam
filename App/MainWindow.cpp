#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include "SourceBoardListWidget.h"
#include <QDockWidget>
#include <QLabel>
#include <QSettings>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QFile>
#include <QDir>
#include <QFileInfo>
#include <QException>
#include <QProgressDialog>
#include <QTimer>
#include <QSharedPointer>
#include "DialogNewProject.h"
#include "DialogSystemSettings.h"
#include "SystemConfigManager.h"
#include "ProjectConfigManager.h"
#include "MaterialsListLoader01.h"
#include "XbelWriter.h"
#include "XbelReader.h"
#include "Global.h"
#include "RDxfServices.h"
#include "RDocument.h"
#include "RDxfExporter.h"
#include "RDxfExporterFactory.h"
#include "RMemoryStorage.h"
#include "RSpatialIndexNavel.h"
#include "RAddObjectOperation.h"
#include "RMixedOperation.h"
#include "RModifyObjectOperation.h"
#include "RDeleteObjectOperation.h"
#include "RDeleteSelectionOperation.h"
#include "RDeleteAllEntitiesOperation.h"
#include "RLayer.h"
#include "RPolylineEntity.h"
#include "RPolylineData.h"
#include "RPolyline.h"
#include "RLineEntity.h"
#include "RArcEntity.h"
#include "RCircleEntity.h"
#include "RTextEntity.h"
#include "RTextData.h"
#include "RVector.h"
#include "RBlockReferenceEntity.h"
#include "CuttingPointSearcher.h"
#include "RPointData.h"
#include "RPointEntity.h"
#include "DialogDissimilarShapes.h"
#include "DissimilarLibraryOperator.h"
#include "BitmapExportDialog.h"
#include "DialogPrintBoards.h"
#include "DialogPrintSettings.h"
#include "CNCBatchExporter.h"
#include "R6PlusOperator.h"
#include "DialogAbout.h"

#include "GuillotineBinPack.h"
#include "MaxRectsBinPack.h"
#include "SkylineBinPack.h"

#include "EG.h"

static bool areaCmpAsc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    int nArea1 = rcSz1.width*rcSz1.height;
    int nArea2 = rcSz2.width*rcSz2.height;

    return nArea1 < nArea2;
}

static bool areaCmpDesc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    int nArea1 = rcSz1.width*rcSz1.height;
    int nArea2 = rcSz2.width*rcSz2.height;

    return nArea1 > nArea2;
}

static bool widthCmpAsc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    return rcSz1.width < rcSz2.width;
}

static bool widthCmpDesc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    return rcSz1.width > rcSz2.width;
}

static bool heightCmpAsc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    return rcSz1.height < rcSz2.height;
}

static bool heightCmpDesc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    return rcSz1.height > rcSz2.height;
}

bool maxCmpAsc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    int first = rcSz1.height > rcSz1.width ?
                rcSz1.height : rcSz1.width;
    int second = rcSz2.height > rcSz2.width ?
                rcSz2.height : rcSz2.width;
    if(first == second)
    {
        return areaCmpAsc(rcSz1, rcSz2);
    }
    else
    {
        return first > second;
    }
}

bool maxCmpDesc(const rbp::RectSize& rcSz1, const rbp::RectSize& rcSz2)
{
    int first = rcSz1.height > rcSz1.width ?
                rcSz1.height : rcSz1.width;
    int second = rcSz2.height > rcSz2.width ?
                rcSz2.height : rcSz2.width;
    if(first == second)
    {
        return areaCmpDesc(rcSz1, rcSz2);
    }
    else
    {
        return first < second;
    }
}

static bool utilizationRateAscCmp(const Solution& solution1, const Solution& solution2)
{
    return solution1.vecSourceBoard.back().dUtilizationRate < solution2.vecSourceBoard.back().dUtilizationRate;
}

static bool utilizationRateDscCmp(const Solution& solution1, const Solution& solution2)
{
    return solution1.vecSourceBoard.back().dUtilizationRate > solution2.vecSourceBoard.back().dUtilizationRate;
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pCADToolBar(NULL),
    m_pViewPort(NULL),
    m_pWorkSpaceDockWidget(NULL),
    m_pSourceBoardListDockWidget(NULL),
    m_pWorkSpaceTreeWidget(NULL),
    m_pSourceBoardListWidget(NULL),
    m_pExcelDataLoader(NULL),
    m_pMaterialTableWidget(NULL),
    m_pSolutionSummaryTableWidget(NULL)
{
    ui->setupUi(this);

    this->setWindowTitle("EZCam");

    this->showMaximized();

    ui->mainToolBar->setIconSize(QSize(64, 48));
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonIconOnly);
    this->setTabShape(QTabWidget::Triangular);

    createWidgets();

    //QTimer *timer = new QTimer(this);
    //connect(timer, &QTimer::timeout, this, &MainWindow::checkDongleTimeout);
    //timer->start(2000);
}

MainWindow::~MainWindow()
{
    if (m_pExcelDataLoader)
    {
        m_pExcelDataLoader->close();
    }
    delete ui;
}

void MainWindow::checkDongleTimeout()
{
    QTextCodec *tc = QTextCodec::codecForName("GBK");

    R6PlusOperator& op = R6PlusOperator::GetInstance();
    if (op.checkDongle() != 0)
    {
        int nValue = QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("找不到加密锁，程序即将关闭"), QMessageBox::Retry, QMessageBox::Discard);
        switch (nValue)
        {
        case QMessageBox::Discard:
        {
            exit(-1);
            break;
        }
        case QMessageBox::Retry:
        {
            if (op.verrifyDongle() != 0)
            {
            }
            break;
        }
        default:
            break;
        }
    }
}

void MainWindow::createWidgets()
{
    /*m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->addTab(new QWidget(this), tr("Materials List"));
    m_pTabWidget->setTabShape(QTabWidget::Triangular);
    m_pTabWidget->setTabPosition(QTabWidget::North);
    m_pTabWidget->setCurrentIndex(0);

    m_pTabWidget->setStyleSheet("QTabWidget{background-color:rgb(85, 170, 255);"
                                "Times New Roman;"
                                "font-size:18px;"
                                "selection-color:pink;"
                                "background-image:url();"
                                "color: white;"
                                "border-radius: 1px;"
                                "border: 1px groove gray;"
                                "border-style: inset;}");

    connect(m_pTabWidget, &QTabWidget::currentChanged, this, &MainWindow::onCurrentTabChanged);

    this->setCentralWidget(m_pTabWidget);*/


    QTextCodec *tc = QTextCodec::codecForName("GBK");

    // 工作区相关控件
    m_pWorkSpaceDockWidget = new QDockWidget(tc->toUnicode("工作区"), this);
    m_pWorkSpaceDockWidget->setObjectName("EZCam-Workspace");
    m_pWorkSpaceDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);
    m_pWorkSpaceDockWidget->setFixedWidth(320);

    m_pWorkSpaceTreeWidget = new WorkspaceTreeWidget(this);
    connect(m_pWorkSpaceTreeWidget, &WorkspaceTreeWidget::itemClicked, this, &MainWindow::onWorkSpaceTreeItemClicked);
    connect(m_pWorkSpaceTreeWidget, &WorkspaceTreeWidget::itemDoubleClicked, this, &MainWindow::onWorkSpaceTreeItemDoubleClicked);
    connect(m_pWorkSpaceTreeWidget, &WorkspaceTreeWidget::deleteMaterialList, this, &MainWindow::onDeleteMaterialList);
    connect(m_pWorkSpaceTreeWidget, &WorkspaceTreeWidget::deleteCuttingSolution, this, &MainWindow::onDeleteCuttingSolution);

    QTreeWidgetItem* pHeaderItem = new QTreeWidgetItem();
    pHeaderItem->setTextAlignment(0, Qt::AlignLeft);
    pHeaderItem->setText(0, "");
    m_pWorkSpaceTreeWidget->setHeaderItem(pHeaderItem);


    // 添加工程名作为树的根节点
    QTreeWidgetItem* pRootItem = new QTreeWidgetItem();
    pRootItem->setData(0, ITEM_TYPE, ITEM_TYPE_ROOT);
    pRootItem->setData(0, XBEL_TARGET, "folder");

    m_pWorkSpaceTreeWidget->addTopLevelItem(pRootItem);

    //connect(m_pWorkSpaceTreeWidget, &QTreeWidget::itemExpanded, this, &MainWindow::OnItemExpanded);
    //connect(m_pWorkSpaceTreeWidget, &QTreeWidget::itemCollapsed, this, &MainWindow::OnItemCollapsed);

    m_pWorkSpaceDockWidget->setWidget(m_pWorkSpaceTreeWidget);
    this->addDockWidget(Qt::LeftDockWidgetArea, m_pWorkSpaceDockWidget, Qt::Horizontal);


    // 大料板列表相关控件
    m_pSourceBoardListDockWidget = new QDockWidget(tc->toUnicode("料板列表"), this);
    m_pSourceBoardListDockWidget->setObjectName("EZCam-SourceBoardList");
    m_pSourceBoardListDockWidget->setAllowedAreas(Qt::AllDockWidgetAreas);

    m_pSourceBoardListWidget = new SourceBoardListWidget(this);
    connect(m_pSourceBoardListWidget, &SourceBoardListWidget::itemSelectionChanged, this, &MainWindow::onItemSelectionChanged);
    QStringList slHeaderList;
    slHeaderList << tc->toUnicode("利用面积") << tc->toUnicode("利用率") << tc->toUnicode("数量");
    m_pSourceBoardListWidget->setTableHeader(slHeaderList);
    m_pSourceBoardListDockWidget->setWidget(m_pSourceBoardListWidget);

    this->addDockWidget(Qt::LeftDockWidgetArea, m_pSourceBoardListDockWidget, Qt::Horizontal);

    // 待开料单及其子料单列表显示控件
    m_pMaterialTableWidget = new MaterialTableWidget(this);
    connect(m_pMaterialTableWidget, &MaterialTableWidget::updateDissimilar, this, &MainWindow::onUpdateDissimilar);
    QStringList slTableHeader;
    slTableHeader << tc->toUnicode("订单号")
                  << tc->toUnicode("条码编号")
                  << tc->toUnicode("产品名称")
                  << tc->toUnicode("成品名称")
                  << tc->toUnicode("材料类型")
                  << tc->toUnicode("长度")
                  << tc->toUnicode("宽度")
                  << tc->toUnicode("厚度")
                  << tc->toUnicode("数量")
                  << tc->toUnicode("封边量")
                  << tc->toUnicode("成品长度")
                  << tc->toUnicode("成品宽度")
                  << tc->toUnicode("成品厚度")
                  << tc->toUnicode("异形")
                  << tc->toUnicode("旋转")
                  << tc->toUnicode("客户信息")
                  << tc->toUnicode("加盟店");

    m_pMaterialTableWidget->setTableHeader(slTableHeader);

    QFontMetrics& metrics = this->fontMetrics();
    int nAverageWidth = metrics.averageCharWidth();

    m_pMaterialTableWidget->setColumnWidth(5, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(6, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(7, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(8, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(9, nAverageWidth*16);
    m_pMaterialTableWidget->setColumnWidth(10, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(11, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(12, nAverageWidth*10);
    m_pMaterialTableWidget->setColumnWidth(14, nAverageWidth*6);

    // 优化排版显示相关控件
    m_pCentralWidget = new CentralWidget(this);
    m_pCentralWidget->setFrameShape(QFrame::Box);

    m_pSolutionSummaryTableWidget = new SolutionSummaryTableWidget(m_pCentralWidget);
    QStringList slSolutionHeader;
    slSolutionHeader << tc->toUnicode("板件总面积")
                     << tc->toUnicode("料板用量")
                     << tc->toUnicode("当前料板利用面积")
                     << tc->toUnicode("当前料板利用率")
                     << tc->toUnicode("板件间距")
                     << tc->toUnicode("总利用率");

    m_pSolutionSummaryTableWidget->setTableHeader(slSolutionHeader);

    m_pViewPort = new ViewPort(m_pCentralWidget);
    m_pViewPort->setFrameShape(QFrame::Box);

    m_pCADToolBar = new QToolBar(this);
    m_pCADToolBar->setIconSize(QSize(24, 24));
    m_pCADToolBar->setOrientation(Qt::Vertical);
    m_pCADToolBar->setAutoFillBackground(true);
    m_pCADToolBar->setStyleSheet("QToolBar {"
                                 "border: 1px solid black;"
                                 "spacing: 3px;"
                                 "}"
                                 "QToolBar::handle {"
                                 "image: url(handle.png);"
                                 "}");

    // Reset Action
    m_pResetActionButton = new QToolButton(m_pCADToolBar);
    m_pResetActionButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pResetActionButton->setIcon(QIcon(":/icons/reset_32.png"));
    m_pResetActionButton->setToolTip(tc->toUnicode("重置"));
    connect(m_pResetActionButton, &QToolButton::clicked, m_pViewPort, &ViewPort::resetActionToolButtonClicked);
    connect(m_pResetActionButton, &QToolButton::clicked, this, &MainWindow::resetActionToolButtonClicked);
    m_pCADToolBar->addWidget(m_pResetActionButton);

    m_pHidenInformationLayerButton = new QToolButton(m_pCADToolBar);
    m_pHidenInformationLayerButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pHidenInformationLayerButton->setIcon(QIcon(":/icons/edit_nc_order_32_1.png"));
    m_pHidenInformationLayerButton->setToolTip(tc->toUnicode("显示/隐藏信息"));
    connect(m_pHidenInformationLayerButton, &QToolButton::clicked, m_pViewPort, &ViewPort::hidenInformationLayerToolButtonClicked);
    m_pCADToolBar->addWidget(m_pHidenInformationLayerButton);

    m_pHidenCuttingPointLayerButton = new QToolButton(m_pCADToolBar);
    m_pHidenCuttingPointLayerButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pHidenCuttingPointLayerButton->setIcon(QIcon(":/icons/hiden_cut_point_32.png"));
    m_pHidenCuttingPointLayerButton->setToolTip(tc->toUnicode("显示/隐藏下刀点"));
    connect(m_pHidenCuttingPointLayerButton, &QToolButton::clicked, m_pViewPort, &ViewPort::hidenCuttingPointLayerToolButtonClicked);
    m_pCADToolBar->addWidget(m_pHidenCuttingPointLayerButton);

    m_pModifyCuttingPointToolButton = new QToolButton(m_pCADToolBar);
    m_pModifyCuttingPointToolButton->setCheckable(true);
    m_pModifyCuttingPointToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pModifyCuttingPointToolButton->setIcon(QIcon(":/icons/modify_cut_point_32.png"));
    m_pModifyCuttingPointToolButton->setToolTip(tc->toUnicode("修改下刀点"));
    connect(m_pModifyCuttingPointToolButton, &QToolButton::clicked, m_pViewPort, &ViewPort::modifyCuttingPointToolButtonClicked);
    connect(m_pModifyCuttingPointToolButton, &QToolButton::clicked, this, &MainWindow::modifyCuttingPointToolButtonClicked);
    m_pCADToolBar->addWidget(m_pModifyCuttingPointToolButton);

    m_pEditNCOrderToolButton = new QToolButton(m_pCADToolBar);
    m_pEditNCOrderToolButton->setCheckable(true);
    m_pEditNCOrderToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pEditNCOrderToolButton->setIcon(QIcon(":/icons/edit_nc_order_32_2.png"));
    m_pEditNCOrderToolButton->setToolTip(tc->toUnicode("编辑切割顺序"));
    connect(m_pEditNCOrderToolButton, &QToolButton::clicked, m_pViewPort, &ViewPort::editNCOrderToolButtonClicked);
    connect(m_pEditNCOrderToolButton, &QToolButton::clicked, this, &MainWindow::editNCOrderToolButtonClicked);
    m_pCADToolBar->addWidget(m_pEditNCOrderToolButton);

    m_pSelectBoardToolButton = new QToolButton(m_pCADToolBar);
    m_pSelectBoardToolButton->setCheckable(true);
    m_pSelectBoardToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pSelectBoardToolButton->setIcon(QIcon(":/icons/select_board_32.png"));
    m_pSelectBoardToolButton->setToolTip(tc->toUnicode("选择板件"));
    connect(m_pSelectBoardToolButton, &QToolButton::clicked, m_pViewPort, &ViewPort::selectBoardToolButtonClicked);
    connect(m_pSelectBoardToolButton, &QToolButton::clicked, this, &MainWindow::selectBoardToolButtonClicked);
    m_pCADToolBar->addWidget(m_pSelectBoardToolButton);

    m_pCADToolBar->addSeparator();

    m_pResetViewportToolButton = new QToolButton(m_pCADToolBar);
    m_pResetViewportToolButton->setCheckable(false);
    m_pResetViewportToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
    m_pResetViewportToolButton->setIcon(QIcon(":/icons/reset_viewport_64.png"));
    m_pResetViewportToolButton->setToolTip(tc->toUnicode("还原视口"));
    connect(m_pResetViewportToolButton, &QToolButton::clicked, m_pViewPort, &ViewPort::resetViewportToolButtonClicked);

    m_pCADToolBar->addWidget(m_pResetViewportToolButton);

    m_pCADToolBar->show();

    m_pCentralWidget->setLayoutWidgets(m_pSolutionSummaryTableWidget, m_pViewPort, m_pCADToolBar);

    this->setCentralWidget(m_pMaterialTableWidget);

    readSettings();

    // 让工作目录树DockWidget显示在前面
    this->tabifyDockWidget(m_pSourceBoardListDockWidget, m_pWorkSpaceDockWidget);
}


void MainWindow::closeEvent(QCloseEvent* event)
{
    saveSettings();

    if (m_pViewPort)
    {
        if (m_pViewPort->isDocumentModified())
        {
            QTextCodec *tc = QTextCodec::codecForName("GBK");
            int nValue = QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("程序即将关闭，部分文件已经被修改但未保存，是否保存修改？"), QMessageBox::Yes, QMessageBox::No, QMessageBox::Cancel);
            if (QMessageBox::Yes == nValue)
            {
                m_pViewPort->saveDocument();
            }
            else if (QMessageBox::Yes == nValue)
            {
            }
            else if (QMessageBox::Cancel == nValue)
            {
                event->ignore();
            }
        }
    }

    //QMainWindow::closeEvent(event);
}

void MainWindow::contextMenuEvent(QContextMenuEvent* event)
{
    QMainWindow::contextMenuEvent(event);
}

void MainWindow::readSettings()
{
    try
    {
        QSettings settings("Xitech", "EZCam");
        restoreGeometry(settings.value("EZCam/Geometry").toByteArray());
        restoreState(settings.value("EZCam/Workspace").toByteArray());
    }
    catch (QException& e)
    {
        qCritical("MainWindow::readSettings() exception: %s", e.what());
    }
}

void MainWindow::saveSettings()
{
    /*ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    if (mgr.isProjectOpened())
    {
        QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);

        if (pRootItem)
        {
            QStringList sl;
            int nNum = pRootItem->childCount();
            QTreeWidgetItem* pItem = NULL;
            for (int i = 0; i < nNum ; ++i)
            {
                pItem = pRootItem->child(i);
                if (pItem)
                {
                    QString strMaterialFileName = pItem->text(0);
                    sl.push_back(strMaterialFileName);
                }
            }
            mgr.addMaterialFiles(sl);
        }

        QString strXbelFilePathName = QString("%1/%2.xbel").arg(mgr.getProjectPath()).arg(mgr.getProjectName());

        // 保存工程目录树的状态到xbel文件中
        XbelWriter xbelWriter(m_pWorkSpaceTreeWidget);

        QFile xbelFile(strXbelFilePathName);
        if (xbelFile.open(QFile::ReadWrite | QFile::Text))
        {
            xbelWriter.writeFile(&xbelFile);
            xbelFile.close();
        }
        else
        {
            qWarning("QXmlStream Bookmarks:%s",
                     tr("Cannot write file %1:%2.")
                     .arg(strXbelFilePathName)
                     .arg(xbelFile.errorString()).toStdString().c_str());

        }
    }*/

    try
    {
        QSettings settings("Xitech", "EZCam");
        settings.setValue("EZCam/Geometry", saveGeometry());
        settings.setValue("EZCam/Workspace", saveState());
    }
    catch (QException& e)
    {
        qCritical("MainWindow::saveSettings() exception: %s", e.what());
    }
}

bool MainWindow::isMaterialFileAlreadyExistsInProjectDir(const QString& strFilePathName)
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    QFileInfo info(strFilePathName);
    QString strMaterialFilePathName = QString("%1/%2/%3").arg(mgr.getProjectPath()).arg(tr("material")).arg(info.fileName());

    QFileInfo inf(strMaterialFilePathName);

    if (inf.exists())
    {
        return true;
    }

    return false;
}


bool MainWindow::copyMaterialFile2ProjectDir(const QString& strFilePathName)
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        // 拷贝一份料单文件到新建的料单目录下
        QFile file(strFilePathName);

        if (file.exists())
        {
            QFileInfo info(strFilePathName);
            QString strNewFile = QString("%1/%2/%3").arg(mgr.getProjectPath()).arg(tr("material")).arg(info.fileName());

            // 拷贝成功
            if (file.copy(strFilePathName, strNewFile))
            {
                return true;
            }
        }

        return false;
    }
    catch (QException& e)
    {
        qCritical("MainWindow::copyMaterialFile2ProjectDir() exception: %s", e.what());

        return false;
    }
}

bool MainWindow::openMaterialFile(const QString& strFilePathName, std::vector<DataSt>& vecData)
{
    try
    {
        m_pExcelDataLoader = new MaterialsListLoader01();

        if (m_pExcelDataLoader->openExcel(strFilePathName))
        {
            vecData = m_pExcelDataLoader->getRecord();

            // 先检查板件尺寸的合法性
            if (!materialTest(vecData))
            {
                return false;
            }

            QFileInfo info(strFilePathName);

            // 用料单文件名（不带后缀）作为map的key
            QString strKey = info.baseName();

            // 保存原始料单到数据库
            if (!MaterialListTable::addMaterialList(strKey, vecData))
            {
                qCritical("Add material list to database failed.\n");
                return false;
            }

            if (vecData.size() > 0)
            {
                QString strMaterialListID = vecData.at(0).strMaterialListID;

                // 按材料类型、厚度拆分料单
                splitMaterialList(strKey, vecData);

                QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);

                if (pRootItem)
                {
                    pRootItem->setExpanded(true);

                    // 添加料单名作为一级子节点
                    QTreeWidgetItem* pMaterialListItem = new QTreeWidgetItem();
                    pMaterialListItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_LIST);
                    pMaterialListItem->setData(0, MATERIAL_LIST_ID, strMaterialListID);
                    pMaterialListItem->setData(0, MATERIAL_LIST_NAME, strKey);
                    pMaterialListItem->setData(0, XBEL_TARGET, "folder");
                    pMaterialListItem->setText(0, strKey);
                    pRootItem->addChild(pMaterialListItem);
                    pMaterialListItem->setExpanded(true);

                    std::map<QString, std::vector<DataSt>>& materialGroupMap = m_materialMap[strKey];

                    // 添加分组子节点
                    std::map<QString, std::vector<DataSt>>::iterator mgIt = materialGroupMap.begin();
                    for (; mgIt != materialGroupMap.end(); ++mgIt)
                    {
                        MaterialGroupSt groupSt;
                        groupSt.strMaterialGroupName = mgIt->first;
                        groupSt.strMaterialListID = strMaterialListID;
                        if (MaterialGroupTable::addGroup(groupSt))
                        {
                            // 添加分组子节点
                            QTreeWidgetItem* pMaterialGroupItem = new QTreeWidgetItem();
                            pMaterialGroupItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_GROUP);
                            pMaterialGroupItem->setData(0, MATERIAL_GROUP_ID, QVariant(groupSt.strMaterialGroupID));
                            pMaterialGroupItem->setData(0, MATERIAL_GROUP_NAME, QVariant(groupSt.strMaterialGroupName));
                            pMaterialGroupItem->setData(0, XBEL_TARGET, "folder");
                            pMaterialGroupItem->setText(0, groupSt.strMaterialGroupName);
                            pMaterialListItem->addChild(pMaterialGroupItem);
                            pMaterialGroupItem->setExpanded(true);
                        }
                        else
                        {
                            qCritical("Add material group to database failed.\n");
                        }
                    }

                    pRootItem->addChild(pMaterialListItem);
                }
            }

            if (m_pExcelDataLoader)
            {
                m_pExcelDataLoader->close();
                delete m_pExcelDataLoader;
                m_pExcelDataLoader = NULL;
            }

            return true;
        }

        if (m_pExcelDataLoader)
        {
            m_pExcelDataLoader->close();
            delete m_pExcelDataLoader;
            m_pExcelDataLoader = NULL;
        }

        return false;
    }
    catch (QException& e)
    {
        qCritical("MainWindow::openMaterialFile() exception: %s", e.what());

        return false;
    }
}

void  MainWindow::splitMaterialList(const QString& strKey, const std::vector<DataSt>& vecData)
{
    try
    {
        if (m_materialMap.find(strKey) != m_materialMap.end())
        {
            return;
        }

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        std::map<QString, std::vector<DataSt>> subMaterialMap;

        subMaterialMap.clear();

        // 按材料类型和厚度拆分料单
        if (smgr.getSplitMaterialsListByType() == 1 && smgr.getSplitMaterialsListByThickness() == 1)
        {
            // 先按材料类型分类
            std::map<QString, std::vector<DataSt>> typeMap;
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                if (typeMap.find(it->strMaterialType) != typeMap.end())
                {
                    std::vector<DataSt>& vecData = typeMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
                else
                {
                    typeMap[it->strMaterialType] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = typeMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
            }

            // 接下来按材料厚度分类
            std::map<QString, std::vector<DataSt>>::iterator it = typeMap.begin();
            for (; it != typeMap.end(); ++it)
            {
                // 从按材料类型分类的结果中依次取出std::vector<DataSt>进行按材料厚度分类
                std::vector<DataSt>& vecDataSt = it->second;
                for (std::vector<DataSt>::iterator vecIt = vecDataSt.begin(); vecIt != vecDataSt.end(); ++vecIt)
                {
                    // 在厚度map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                    // 材料名称+材料厚度作为key
                    QString strKey = QString("%1-%2mm").arg(it->first).arg(vecIt->nBoardThickness);
                    if (subMaterialMap.find(strKey) != subMaterialMap.end())
                    {
                        std::vector<DataSt>& vecData = subMaterialMap[strKey];
                        vecData.push_back(*vecIt);
                    }
                    else
                    {
                        subMaterialMap[strKey] = std::vector<DataSt>();
                        std::vector<DataSt>& vecData = subMaterialMap[strKey];
                        vecData.push_back(*vecIt);
                    }
                }
            }
        }
        // 按材料类型拆分料单
        else if (smgr.getSplitMaterialsListByType() == 1 && smgr.getSplitMaterialsListByThickness() == 0)
        {
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                if (subMaterialMap.find(it->strMaterialType) != subMaterialMap.end())

                {
                    std::vector<DataSt>& vecData = subMaterialMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[it->strMaterialType] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[it->strMaterialType];
                    vecData.push_back(*it);
                }
            }
        }
        // 按材料厚度拆分料单
        else if (smgr.getSplitMaterialsListByType() == 0 && smgr.getSplitMaterialsListByThickness() == 1)
        {
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                // 材料厚度作为key
                QString strKey = QString("%1mm").arg(it->nBoardThickness);
                if (subMaterialMap.find(strKey) != subMaterialMap.end())
                {
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[strKey] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
            }
        }
        // 不进行料单拆分
        else if (smgr.getSplitMaterialsListByType() == 0 && smgr.getSplitMaterialsListByThickness() == 0)
        {
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            QString strKey = mgr.getProjectName();
            for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
            {
                // 在类型map中不存在，先添加一个map再向map写记录，否则直接向已经存在的map中写记录
                // 材料厚度作为key
                if (subMaterialMap.find(strKey) != subMaterialMap.end())
                {
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
                else
                {
                    subMaterialMap[strKey] = std::vector<DataSt>();
                    std::vector<DataSt>& vecData = subMaterialMap[strKey];
                    vecData.push_back(*it);
                }
            }
        }

        m_materialMap[strKey] = subMaterialMap;
    }
    catch (QException& e)
    {
        qCritical("MainWindow::splitMaterialList() exception: %s", e.what());
    }
}

void MainWindow::swapRectSizeWH(rbp::RectSize& rcSz)
{
    int tmp = rcSz.width;
    rcSz.width = rcSz.height;
    rcSz.height = tmp;

    if (rcSz.bRotated)
    {
        rcSz.bRotated = false;
    }
    else
    {
        rcSz.bRotated = true;
    }
}

void MainWindow::arrangeBoards(int nSortMethod, int nRotateCondition, std::vector<rbp::RectSize>& vecSize)
{
    switch(nRotateCondition)
    {
    case H2_WIDTH_H:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.height > rcSz.width && rcSz.width*2 > rcSz.height)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }
    case WIDTH_GREATHER_HEIGHT:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.width > rcSz.height)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }
    case WIDTH_GREATHER_2HEIGHT:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.width > 2*rcSz.height)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }
    case W2_HEIGHT_W:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.width > rcSz.height && rcSz.height*2 > rcSz.width)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }
    case HEIGHT_GREATHER_WIDTH:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.height > rcSz.width)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }
    case HEIGHT_GREATHER_2WIDTH:
    {
        std::vector<rbp::RectSize>::iterator it = vecSize.begin();
        std::vector<rbp::RectSize>::iterator end = vecSize.end();
        for (; it != end; ++it)
        {
            rbp::RectSize& rcSz = *it;
            if (rcSz.height > 2*rcSz.width)
            {
                swapRectSizeWH(rcSz);
            }
        }
        break;
    }

    default:
        break;
    }

    switch(nSortMethod)
    {
    case WIDTH:
    {
        std::sort(vecSize.begin(), vecSize.end(), widthCmpAsc);
        break;
    }
    case HEIGHT:
    {
        std::sort(vecSize.begin(), vecSize.end(), heightCmpAsc);
        break;
    }
    case SORT_AREA:
    {
        std::sort(vecSize.begin(), vecSize.end(), areaCmpAsc);
        break;
    }
    case SORT_MAX:
    {
        std::sort(vecSize.begin(), vecSize.end(), maxCmpAsc);
        break;
    }
    default:
        break;
    }
}

void MainWindow::guillotinePacking(int nSourceMaterialBoardWidth,
                                   int nSourceMaterialBoardHeight,
                                   int nBoardSpacing,
                                   const Solution& solution,
                                   const std::vector<rbp::RectSize>& vecRects,
                                   bool bAllBoardsSupportRotate,
                                   std::vector<Solution>& vecResult,
                                   int& nMinBoardNum)
{
    rbp::GuillotineBinPack packer;

    Solution sln = solution;

    std::set<double> setLastBoardUtilizationRate;
    std::set<double> setLastBoardUtilizationRateTmp;

    std::vector<Solution> vecResultTmp;

    std::vector<rbp::RectSize> vecSourceRect(vecRects.begin(), vecRects.end());

    int nRotationNum = bAllBoardsSupportRotate ? ROTATION_NUM : 1;

    for (int so = SORT_NONE; so < SORT_NUM; ++so)
    {
        for (int ro = NEVER; ro < nRotationNum; ++ro)
        {
            arrangeBoards(so, ro, vecSourceRect);

            SourceBoardSt sbSt;
            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 6; ++j)
                {
                    std::vector<rbp::RectSize> vecTmp = vecSourceRect;
                    sln.solutionSt.nTotalSourceBoardNum = 0;
                    sln.vecSourceBoard.clear();
                    sbSt.nIdx = 0;

                    while (vecTmp.size() > 0)
                    {
                        QApplication::processEvents();

                        packer.Init(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight);

                        packer.Insert(vecTmp, true, (rbp::GuillotineBinPack::FreeRectChoiceHeuristic)i, (rbp::GuillotineBinPack::GuillotineSplitHeuristic)j);

                        std::vector<rbp::Rect>& vecSrcRect = packer.GetUsedRectangles();

                        // 对各个方案中使用的原料板完全相同(料板上的所有小板块的位置、大小、编号均重合)的板块进行合并

                        bool bMerge = false;

                        std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                        for (; it != sln.vecSourceBoard.end(); ++it)
                        {
                            std::vector<rbp::Rect>& vecDstRect = it->vecRect;

                            int nSize = vecSrcRect.size();

                            // 首先，料板中包含的小板块数量得相同才有可能是两块完全一样的料板
                            if (nSize == vecDstRect.size())
                            {
                                bool bCoincide = true;
                                for (int n = 0; n < nSize; ++n)
                                {
                                    rbp::Rect& dstRect = vecDstRect.at(n);
                                    rbp::Rect& srcRect = vecSrcRect.at(n);
                                    if (dstRect.width != srcRect.width
                                            || dstRect.height != srcRect.height
                                            || dstRect.barcodeNo != srcRect.barcodeNo)
                                    {
                                        bCoincide = false;
                                        break;
                                    }
                                }

                                // 重合，则料板数量加1
                                if (bCoincide)
                                {
                                    bMerge = true;
                                    it->nNum++;
                                    break;
                                }
                            }
                        }

                        // 没有找到可以合并的料板，则新增一块料板
                        if (!bMerge)
                        {
                            sbSt.dUtilizationRate = packer.Occupancy();
                            sbSt.nWidth = sln.solutionSt.nSourceBoardWidth;
                            sbSt.nHeight = sln.solutionSt.nSourceBoardHeight;
                            sbSt.nThickness = sln.solutionSt.nSourceBoardThickness;
                            sbSt.nIdx++;
                            sbSt.vecRect.clear();
                            sbSt.vecRect = vecSrcRect;
                            sbSt.nNum = 1;

                            sln.vecSourceBoard.push_back(sbSt);
                        }
                    }

                    // 这个方案的料板利用面积（即整个方案的小板件面积和）
                    unsigned long nTotalUtilizationArea = 0.0f;
                    sln.solutionSt.nTotalSourceBoardNum = 0;
                    std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                    for (; it != sln.vecSourceBoard.end(); ++it)
                    {
                        SourceBoardSt& sbSt = *it;

                        sbSt.dUtilizationArea = 0.0f;
                        sbSt.dUtilizationRate = 0.0f;

                        // 这块料板的利用面积（即小板件的面积和）
                        it->dUtilizationArea = 0.0f;
                        std::vector<rbp::Rect>& vecRect = sbSt.vecRect;
                        std::vector<rbp::Rect>::iterator rcIt = vecRect.begin();
                        for (; rcIt != vecRect.end(); ++rcIt)
                        {
                            rbp::Rect& rc = *rcIt;
                            sbSt.dUtilizationArea += (rc.width-nBoardSpacing)*(rc.height-nBoardSpacing);
                        }

                        // 这块料板的利用率
                        sbSt.dUtilizationRate = sbSt.dUtilizationArea/(sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight);

                        nTotalUtilizationArea += (sbSt.dUtilizationArea*sbSt.nNum);

                        sln.solutionSt.nTotalSourceBoardNum += sbSt.nNum;
                    }

                    // 跳过空方案
                    if (sln.vecSourceBoard.size() > 0)
                    {
                        // 这个方案的总利用率
                        unsigned long nTotalArea  = sln.solutionSt.nTotalSourceBoardNum*sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight;
                        sln.solutionSt.dTotalUtilizationRate = (double)nTotalUtilizationArea/nTotalArea;

                        setLastBoardUtilizationRateTmp.clear();
                        setLastBoardUtilizationRateTmp.insert(sln.vecSourceBoard.back().dUtilizationRate);

                        vecResultTmp.clear();
                        vecResultTmp.push_back(sln);

                        //vecResult.push_back(sln);

                        // 消耗板块数量少的作为备选方案
                        if (sln.solutionSt.nTotalSourceBoardNum < nMinBoardNum)
                        {
                            vecResult.swap(vecResultTmp);
                            setLastBoardUtilizationRate.swap(setLastBoardUtilizationRateTmp);

                            nMinBoardNum = sln.solutionSt.nTotalSourceBoardNum;
                        }
                        // 消耗板块数量相等时，平均利用率不等的(为了丢弃完全一样的方案)，也作为备选方案
                        else if (sln.solutionSt.nTotalSourceBoardNum == nMinBoardNum)
                        {
                            // 最后一张料板的利用率不同的方案，也作为为选方案
                            if (setLastBoardUtilizationRate.find(sln.vecSourceBoard.back().dUtilizationRate) == setLastBoardUtilizationRate.end())
                            {
                                vecResult.push_back(sln);
                            }
                        }
                    }
                }
            }
        }
    }
}

void MainWindow::maxRectsPacking(int nSourceMaterialBoardWidth,
                                 int nSourceMaterialBoardHeight,
                                 int nBoardSpacing,
                                 const Solution& solution,
                                 const std::vector<rbp::RectSize>& vecRects,
                                 bool bAllBoardsSupportRotate,
                                 std::vector<Solution>& vecResult,
                                 int& nMinBoardNum)
{   
    rbp::MaxRectsBinPack packer;

    Solution sln = solution;

    std::set<double> setLastBoardUtilizationRate;
    std::set<double> setLastBoardUtilizationRateTmp;

    std::vector<Solution> vecResultTmp;

    std::vector<rbp::RectSize> vecSourceRect(vecRects.begin(), vecRects.end());

    int nRotationNum = bAllBoardsSupportRotate ? ROTATION_NUM : 1;

    for (int so = SORT_NONE; so < SORT_NUM; ++so)
    {
        for (int ro = NEVER; ro < nRotationNum; ++ro)
        {
            arrangeBoards(so, ro, vecSourceRect);

            SourceBoardSt sbSt;
            for (int i = 0; i < 5; ++i)
            {
                std::vector<rbp::RectSize> vecTmp = vecSourceRect;
                sln.solutionSt.nTotalSourceBoardNum = 0;
                sln.vecSourceBoard.clear();
                sbSt.nIdx = 0;

                while (vecTmp.size() > 0)
                {
                    QApplication::processEvents();

                    packer.Init(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight);

                    packer.Insert(vecTmp, (rbp::MaxRectsBinPack::FreeRectChoiceHeuristic)i);

                    std::vector<rbp::Rect>& vecSrcRect = packer.GetUsedRectangles();

                    // 对各个方案中使用的原料板完全相同(料板上的所有小板块的位置、大小、编号均重合)的板块进行合并

                    bool bMerge = false;

                    std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                    for (; it != sln.vecSourceBoard.end(); ++it)
                    {
                        std::vector<rbp::Rect>& vecDstRect = it->vecRect;

                        int nSize = vecSrcRect.size();

                        // 首先，料板中包含的小板块数量得相同才有可能是两块完全一样的料板
                        if (nSize == vecDstRect.size())
                        {
                            bool bCoincide = true;
                            for (int n = 0; n < nSize; ++n)
                            {
                                rbp::Rect& dstRect = vecDstRect.at(n);
                                rbp::Rect& srcRect = vecSrcRect.at(n);
                                if (dstRect.width != srcRect.width
                                        || dstRect.height != srcRect.height
                                        || dstRect.barcodeNo != srcRect.barcodeNo)
                                {
                                    bCoincide = false;
                                    break;
                                }
                            }

                            // 重合，则料板数量加1
                            if (bCoincide)
                            {
                                bMerge = true;
                                it->nNum++;
                                break;
                            }
                        }
                    }

                    // 没有找到可以合并的料板，则新增一块料板
                    if (!bMerge)
                    {
                        sbSt.dUtilizationRate = packer.Occupancy();
                        sbSt.nWidth = sln.solutionSt.nSourceBoardWidth;
                        sbSt.nHeight = sln.solutionSt.nSourceBoardHeight;
                        sbSt.nThickness = sln.solutionSt.nSourceBoardThickness;
                        sbSt.nIdx++;
                        sbSt.vecRect.clear();
                        sbSt.vecRect = vecSrcRect;
                        sbSt.nNum = 1;

                        sln.vecSourceBoard.push_back(sbSt);
                    }
                }

                // 这个方案的料板利用面积（即整个方案的小板件面积和）
                unsigned long nTotalUtilizationArea = 0.0f;
                sln.solutionSt.nTotalSourceBoardNum = 0;
                std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                for (; it != sln.vecSourceBoard.end(); ++it)
                {
                    SourceBoardSt& sbSt = *it;

                    sbSt.dUtilizationArea = 0.0f;
                    sbSt.dUtilizationRate = 0.0f;

                    // 这块料板的利用面积（即小板件的面积和）
                    it->dUtilizationArea = 0.0f;
                    std::vector<rbp::Rect>& vecRect = sbSt.vecRect;
                    std::vector<rbp::Rect>::iterator rcIt = vecRect.begin();
                    for (; rcIt != vecRect.end(); ++rcIt)
                    {
                        rbp::Rect& rc = *rcIt;
                        sbSt.dUtilizationArea += (rc.width-nBoardSpacing)*(rc.height-nBoardSpacing);
                    }

                    // 这块料板的利用率
                    sbSt.dUtilizationRate = sbSt.dUtilizationArea/(sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight);

                    nTotalUtilizationArea += (sbSt.dUtilizationArea*sbSt.nNum);

                    sln.solutionSt.nTotalSourceBoardNum += sbSt.nNum;
                }

                // 跳过空方案
                if (sln.vecSourceBoard.size() > 0)
                {
                    // 这个方案的总利用率
                    unsigned long nTotalArea  = sln.solutionSt.nTotalSourceBoardNum*sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight;
                    sln.solutionSt.dTotalUtilizationRate = (double)nTotalUtilizationArea/nTotalArea;

                    setLastBoardUtilizationRateTmp.clear();
                    setLastBoardUtilizationRateTmp.insert(sln.vecSourceBoard.back().dUtilizationRate);

                    vecResultTmp.clear();
                    vecResultTmp.push_back(sln);

                    //vecResult.push_back(sln);

                    // 消耗板块数量少的作为备选方案
                    if (sln.solutionSt.nTotalSourceBoardNum < nMinBoardNum)
                    {
                        vecResult.swap(vecResultTmp);
                        setLastBoardUtilizationRate.swap(setLastBoardUtilizationRateTmp);

                        nMinBoardNum = sln.solutionSt.nTotalSourceBoardNum;
                    }
                    // 消耗板块数量相等时，平均利用率不等的(为了丢弃完全一样的方案)，也作为备选方案
                    else if (sln.solutionSt.nTotalSourceBoardNum == nMinBoardNum)
                    {
                        // 最后一张料板的利用率不同的方案，也作为为选方案
                        if (setLastBoardUtilizationRate.find(sln.vecSourceBoard.back().dUtilizationRate) == setLastBoardUtilizationRate.end())
                        {
                            vecResult.push_back(sln);
                        }
                    }
                }
            }
        }
    }
}


void MainWindow::skylinePacking(int nSourceMaterialBoardWidth,
                                int nSourceMaterialBoardHeight,
                                int nBoardSpacing,
                                const Solution& solution,
                                const std::vector<rbp::RectSize>& vecRects,
                                bool bAllBoardsSupportRotate,
                                std::vector<Solution>& vecResult,
                                int& nMinBoardNum)
{
    rbp::SkylineBinPack packer;

    Solution sln = solution;

    std::set<double> setLastBoardUtilizationRate;
    std::set<double> setLastBoardUtilizationRateTmp;

    std::vector<Solution> vecResultTmp;

    std::vector<rbp::RectSize> vecSourceRect(vecRects.begin(), vecRects.end());

    int nRotationNum = bAllBoardsSupportRotate ? ROTATION_NUM : 1;

    for (int so = SORT_NONE; so < SORT_NUM; ++so)
    {
        for (int ro = NEVER; ro < nRotationNum; ++ro)
        {
            arrangeBoards(so, ro, vecSourceRect);

            SourceBoardSt sbSt;
            for (int i = 0; i < 2; ++i)
            {
                std::vector<rbp::RectSize> vecTmp = vecSourceRect;
                sln.solutionSt.nTotalSourceBoardNum = 0;
                sln.vecSourceBoard.clear();
                sbSt.nIdx = 0;

                while (vecTmp.size() > 0)
                {
                    QApplication::processEvents();

                    packer.Init(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight, false);

                    std::vector<rbp::Rect> vecSrcRect;
                    packer.Insert(vecTmp, vecSrcRect, (rbp::SkylineBinPack::LevelChoiceHeuristic)i);

                    // 对各个方案中使用的原料板完全相同(料板上的所有小板块的位置、大小、编号均重合)的板块进行合并

                    bool bMerge = false;

                    std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                    for (; it != sln.vecSourceBoard.end(); ++it)
                    {
                        std::vector<rbp::Rect>& vecDstRect = it->vecRect;

                        int nSize = vecSrcRect.size();

                        // 首先，料板中包含的小板块数量得相同才有可能是两块完全一样的料板
                        if (nSize == vecDstRect.size())
                        {
                            bool bCoincide = true;
                            for (int n = 0; n < nSize; ++n)
                            {
                                rbp::Rect& dstRect = vecDstRect.at(n);
                                rbp::Rect& srcRect = vecSrcRect.at(n);
                                if (dstRect.width != srcRect.width
                                        || dstRect.height != srcRect.height
                                        || dstRect.barcodeNo != srcRect.barcodeNo)
                                {
                                    bCoincide = false;
                                    break;
                                }
                            }

                            // 重合，则料板数量加1
                            if (bCoincide)
                            {
                                bMerge = true;
                                it->nNum++;
                                break;
                            }
                        }
                    }

                    // 没有找到可以合并的料板，则新增一块料板
                    if (!bMerge)
                    {
                        sbSt.dUtilizationRate = packer.Occupancy();
                        sbSt.nWidth = sln.solutionSt.nSourceBoardWidth;
                        sbSt.nHeight = sln.solutionSt.nSourceBoardHeight;
                        sbSt.nThickness = sln.solutionSt.nSourceBoardThickness;
                        sbSt.nIdx++;
                        sbSt.vecRect.clear();
                        sbSt.vecRect = vecSrcRect;
                        sbSt.nNum = 1;

                        sln.vecSourceBoard.push_back(sbSt);
                    }
                }

                // 这个方案的料板利用面积（即整个方案的小板件面积和）
                unsigned long nTotalUtilizationArea = 0.0f;
                sln.solutionSt.nTotalSourceBoardNum = 0;
                std::vector<SourceBoardSt>::iterator it = sln.vecSourceBoard.begin();
                for (; it != sln.vecSourceBoard.end(); ++it)
                {
                    SourceBoardSt& sbSt = *it;

                    sbSt.dUtilizationArea = 0.0f;
                    sbSt.dUtilizationRate = 0.0f;

                    // 这块料板的利用面积（即小板件的面积和）
                    it->dUtilizationArea = 0.0f;
                    std::vector<rbp::Rect>& vecRect = sbSt.vecRect;
                    std::vector<rbp::Rect>::iterator rcIt = vecRect.begin();
                    for (; rcIt != vecRect.end(); ++rcIt)
                    {
                        rbp::Rect& rc = *rcIt;
                        sbSt.dUtilizationArea += (rc.width-nBoardSpacing)*(rc.height-nBoardSpacing);
                    }

                    // 这块料板的利用率
                    sbSt.dUtilizationRate = sbSt.dUtilizationArea/(sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight);

                    nTotalUtilizationArea += (sbSt.dUtilizationArea*sbSt.nNum);

                    sln.solutionSt.nTotalSourceBoardNum += sbSt.nNum;
                }

                // 跳过空方案
                if (sln.vecSourceBoard.size() > 0)
                {
                    // 这个方案的总利用率
                    unsigned long nTotalArea  = sln.solutionSt.nTotalSourceBoardNum*sln.solutionSt.nSourceBoardWidth*sln.solutionSt.nSourceBoardHeight;
                    sln.solutionSt.dTotalUtilizationRate = (double)nTotalUtilizationArea/nTotalArea;

                    setLastBoardUtilizationRateTmp.clear();
                    setLastBoardUtilizationRateTmp.insert(sln.vecSourceBoard.back().dUtilizationRate);

                    vecResultTmp.clear();
                    vecResultTmp.push_back(sln);

                    //vecResult.push_back(sln);

                    // 消耗板块数量少的作为备选方案
                    if (sln.solutionSt.nTotalSourceBoardNum < nMinBoardNum)
                    {
                        vecResult.swap(vecResultTmp);
                        setLastBoardUtilizationRate.swap(setLastBoardUtilizationRateTmp);

                        nMinBoardNum = sln.solutionSt.nTotalSourceBoardNum;
                    }
                    // 消耗板块数量相等时，平均利用率不等的(为了丢弃完全一样的方案)，也作为备选方案
                    else if (sln.solutionSt.nTotalSourceBoardNum == nMinBoardNum)
                    {
                        // 最后一张料板的利用率不同的方案，也作为为选方案
                        if (setLastBoardUtilizationRate.find(sln.vecSourceBoard.back().dUtilizationRate) == setLastBoardUtilizationRate.end())
                        {
                            vecResult.push_back(sln);
                        }
                    }
                }
            }
        }
    }
}

std::vector<Solution> MainWindow::packing(const std::vector<DataSt>& vecData)
{
    try
    {
        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        int nBoardSpacing = smgr.getBoardSpacing();
        int nBoardTrimming = smgr.getSourceMaterialBoardTrimming();

        // 是否所有板件都支持旋转（取决于料单）
        bool bAllBoardsSupportRotate = true;

        Solution solution;
        std::vector<rbp::RectSize> vecSourceRect;
        rbp::RectSize size;
        for (std::vector<DataSt>::const_iterator it = vecData.begin(); it != vecData.end(); ++it)
        {
            for (int n = 0; n < it->nBoardNum; ++n)
            {
                // 加上板间距(即钻头宽度，切割时损耗），实际上就是板块增宽增厚，切割损耗后即是原板块尺寸
                size.width = it->nBoardWidth + nBoardSpacing;
                size.height = it->nBoardHeight + nBoardSpacing;

                size.barcodeNo = it->strBarCodeNo.toStdString();
                size.name = it->strFinishedProductName.toStdString();
                size.rotate = it->nRotate;
                vecSourceRect.push_back(size);

                solution.solutionSt.nSourceBoardThickness = it->nBoardThickness;
                solution.solutionSt.dTotalTargetArea += (it->nBoardWidth*it->nBoardHeight);

                // 检查是否支持旋转
                if (bAllBoardsSupportRotate && size.rotate != 1)
                {
                    bAllBoardsSupportRotate = false;
                }
            }
        }

        solution.solutionSt.nSourceBoardWidth = smgr.getSourceMaterialBoardWidth();
        solution.solutionSt.nSourceBoardHeight = smgr.getSourceMaterialBoardHeight();

        int nSourceMaterialBoardWidth = 0;
        int nSourceMaterialBoardHeight = 0;

        double dSpacing = nBoardSpacing;
        double dTrimming = nBoardTrimming;

        nSourceMaterialBoardWidth = solution.solutionSt.nSourceBoardWidth + (dSpacing/2.0-dTrimming)*2;
        nSourceMaterialBoardHeight = solution.solutionSt.nSourceBoardHeight + (dSpacing/2.0-dTrimming)*2;

        int nMaxSolutionNum = smgr.getMaxSolutionNum();

        // 初始时给一个较大的数值，使首次比较条件成立
        int nMinBoardNum = std::numeric_limits<int>::max();

        std::vector<Solution> vecResult;

        skylinePacking(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight, nBoardSpacing, solution, vecSourceRect, bAllBoardsSupportRotate, vecResult, nMinBoardNum);

        guillotinePacking(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight, nBoardSpacing, solution, vecSourceRect, bAllBoardsSupportRotate, vecResult, nMinBoardNum);

        maxRectsPacking(nSourceMaterialBoardWidth, nSourceMaterialBoardHeight, nBoardSpacing, solution, vecSourceRect, bAllBoardsSupportRotate, vecResult, nMinBoardNum);

        // 只用一张大料板时按利用率从大到小排序,否则按利用率从小到大排序
        if (nMinBoardNum == 1)
        {
            std::sort(vecResult.begin(), vecResult.end(), utilizationRateDscCmp);
        }
        else
        {
            std::sort(vecResult.begin(), vecResult.end(), utilizationRateAscCmp);
        }

        if ((int)vecResult.size() > nMaxSolutionNum)
        {
            std::vector<Solution> vecResultSelected(vecResult.begin(), vecResult.begin()+nMaxSolutionNum);
            return vecResultSelected;
        }
        else
        {
            return vecResult;
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::packing() exception: %s", e.what());
        return std::vector<Solution>();
    }
}

void MainWindow::on_actionNewProject_triggered()
{    
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        DialogNewProject dlg;
        if (dlg.exec() == QDialog::Accepted)
        {
            if (QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("现在添加料单？"),
                                         QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)

            {
                SystemConfigManager& sMgr = SystemConfigManager::GetInstance();

                QTextCodec *tc = QTextCodec::codecForName("GBK");

                QString strFilePathName = QFileDialog::getOpenFileName(this, tc->toUnicode("打开料单文件"),
                                                                       sMgr.getRecentlyUsedMaterialFileDir(),
                                                                       tr("Files (*.xls)"));
                if (!strFilePathName.isEmpty())
                {
                    QFileInfo info(strFilePathName);
                    if (info.exists() && info.isFile())
                    {
                        QString strPath = info.absoluteDir().path();
                        sMgr.setRecentlyUsedMaterialFileDir(strPath);

                        if(isMaterialFileAlreadyExistsInProjectDir(strFilePathName))
                        {
                            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("料单已经存在."), tc->toUnicode("知道了"));
                            return;
                        }

                        std::vector<DataSt> vecData;

                        if(!openMaterialFile(strFilePathName, vecData) || !copyMaterialFile2ProjectDir(strFilePathName))
                        {
                            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("添加料单失败."), tc->toUnicode("知道了"));
                            return;
                        }
                        else
                        {
                            int nBoardNum = 0;
                            std::vector<DataSt>::const_iterator it = vecData.begin();
                            for (; it != vecData.end(); ++it)
                            {
                                const DataSt& st = *it;
                                nBoardNum += st.nBoardNum;
                            }

                            QString strMsg = QString(tc->toUnicode("料单导入完成，共%1块板件")).arg(nBoardNum);
                            QMessageBox::information(this, tc->toUnicode("信息"), strMsg, tc->toUnicode("知道了"));
                        }

                        // 让工作目录树DockWidget显示在前面
                        this->tabifyDockWidget(m_pSourceBoardListDockWidget, m_pWorkSpaceDockWidget);

                        if (m_materialMap.size() > 0)
                        {
                            std::map<QString, std::map<QString, std::vector<DataSt>>>::iterator mapIt = m_materialMap.begin();
                            QString strMaterialListName = mapIt->first;
                            std::map<QString, std::vector<DataSt>>& groupMap = m_materialMap[strMaterialListName];
                            for (std::map<QString, std::vector<DataSt>>::iterator it = groupMap.begin(); it != groupMap.end(); ++it)
                            {
                                std::vector<DataSt>& vecData = it->second;
                                m_pMaterialTableWidget->setMaterialListName(strMaterialListName);
                                m_pMaterialTableWidget->displayData(vecData);
                            }

                            if (this->centralWidget() != m_pMaterialTableWidget)
                            {
                                this->takeCentralWidget();
                                this->setCentralWidget(m_pMaterialTableWidget);
                            }
                        }
                    }
                }
            }
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);
            if (pRootItem)
            {
                pRootItem->setText(0, mgr.getProjectName());
            }
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionNewProject_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionOpenProject_triggered()
{
    try
    {
        SystemConfigManager& sMgr = SystemConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        QString strFilePathName = QFileDialog::getOpenFileName(this, tc->toUnicode("打开工程"),
                                                               sMgr.getWorkingPath(),
                                                               tr("File (*.pro)"));
        if (!strFilePathName.isEmpty())
        {
            // 打开工程配置文件
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
            mgr.openConfigFile(strFilePathName);

            // 为了兼容3.5之前的版本，现在加了个功能：打开工程时自动检测工程目录下是否有shape文件夹，没有的话就自动添加一个
            QString strShapeDir = mgr.getProjectPath() + "/shape";

            QDir shapeDir(strShapeDir);
            if (!shapeDir.exists())
            {
                shapeDir.mkdir(strShapeDir);
            }

            // 获取数据库文件
            QString strDatabaseFile = QString("%1/%2.db").arg(mgr.getProjectPath()).arg(mgr.getProjectName());
            QFileInfo dbFileInfo(strDatabaseFile);
            if (dbFileInfo.exists() && dbFileInfo.isFile())
            {
                CDatabaseManager& dbMgr = CDatabaseManager::GetInstance();
                dbMgr.SetDriverName("QSQLITE");
                dbMgr.SetDatabasePathName(strDatabaseFile);

                QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);

                if (pRootItem)
                {
                    pRootItem->setExpanded(true);

                    // 让工作目录树DockWidget显示在前面
                    this->tabifyDockWidget(m_pSourceBoardListDockWidget, m_pWorkSpaceDockWidget);

                    // 设置工程名称
                    pRootItem->setText(0, mgr.getProjectName());

                    std::map<QString, std::vector<DataSt>> materialMap;

                    // 从数据库获取料单数据
                    if (!MaterialListTable::getMaterialLists(materialMap))
                    {
                        qCritical("Get material lists to database failed.\n");
                        return;
                    }

                    // 从数据库获取各个料单数据，依次拆分
                    for (std::map<QString, std::vector<DataSt>>::iterator it = materialMap.begin(); it != materialMap.end(); ++it)
                    {
                        const QString& strMaterialListName = it->first;
                        std::vector<DataSt>& vecData = it->second;
                        if (vecData.size() > 0)
                        {
                            // 取出料单ID
                            QString strMaterialListID = vecData.at(0).strMaterialListID;

                            // 按材料类型、厚度拆分料单
                            splitMaterialList(strMaterialListName, vecData);

                            // 添加料单名作为一级子节点
                            QTreeWidgetItem* pMaterialListItem = new QTreeWidgetItem();
                            pMaterialListItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_LIST);
                            pMaterialListItem->setData(0, MATERIAL_LIST_ID, strMaterialListID);
                            pMaterialListItem->setData(0, MATERIAL_LIST_NAME, strMaterialListName);
                            pMaterialListItem->setData(0, XBEL_TARGET, "folder");
                            pMaterialListItem->setText(0, strMaterialListName);
                            pRootItem->addChild(pMaterialListItem);
                            pMaterialListItem->setExpanded(true);

                            // 从数据库获取各个料单分组，构建工作目录树
                            std::vector<MaterialGroupSt> vecGroup;
                            if (MaterialGroupTable::getGroupsByMaterialListID(strMaterialListID, vecGroup))
                            {
                                for (std::vector<MaterialGroupSt>::iterator mgIt = vecGroup.begin(); mgIt != vecGroup.end(); ++mgIt)
                                {
                                    MaterialGroupSt& mgSt = *mgIt;

                                    // 添加分组子节点
                                    QTreeWidgetItem* pMaterialGroupItem = new QTreeWidgetItem();
                                    pMaterialGroupItem->setData(0, ITEM_TYPE, ITEM_TYPE_MATERIAL_GROUP);
                                    pMaterialGroupItem->setData(0, MATERIAL_GROUP_ID, QVariant(mgSt.strMaterialGroupID));
                                    pMaterialGroupItem->setData(0, MATERIAL_GROUP_NAME, QVariant(mgSt.strMaterialGroupName));
                                    pMaterialGroupItem->setData(0, XBEL_TARGET, "folder");
                                    pMaterialGroupItem->setText(0, mgSt.strMaterialGroupName);
                                    pMaterialListItem->addChild(pMaterialGroupItem);
                                    pMaterialGroupItem->setExpanded(true);

                                    // 添加方案子节点
                                    std::vector<CuttingSolutionSt> vecCsSt;
                                    if (CuttingSolutionTable::getSolutionsByMaterialGroupID(mgSt.strMaterialGroupID, vecCsSt))
                                    {
                                        for (std::vector<CuttingSolutionSt>::iterator csIt = vecCsSt.begin(); csIt != vecCsSt.end(); ++ csIt)
                                        {
                                            CuttingSolutionSt& csSt = *csIt;
                                            QTreeWidgetItem* pSolutionItem = new QTreeWidgetItem();
                                            pSolutionItem->setData(0, ITEM_TYPE, ITEM_TYPE_CUTTING_SOLUTION);
                                            pSolutionItem->setData(0, CUTTING_SOLUTION_ID, csSt.strSolutionID);
                                            pSolutionItem->setData(0, CUTTING_SOLUTION_NAME, csSt.strSolutionName);
                                            pSolutionItem->setData(0, XBEL_TARGET, "bookmark");
                                            pSolutionItem->setText(0, csSt.strSolutionName);

                                            pMaterialGroupItem->addChild(pSolutionItem);
                                            pSolutionItem->setExpanded(true);

                                            // 准备相应方案的板块用料数据
                                            Solution solution;
                                            solution.solutionSt = csSt;
                                            if (SourceBoardTable::getBoardsBySolutionID(csSt.strSolutionID, solution.vecSourceBoard))
                                            {
                                                // 记录方案和原料板
                                                m_solutionMap[csSt.strSolutionID] = solution;

                                                std::vector<SourceBoardSt>::iterator sbIt = solution.vecSourceBoard.begin();
                                                for (; sbIt != solution.vecSourceBoard.end(); ++sbIt)
                                                {
                                                    SourceBoardSt& sbSt = *sbIt;
                                                    std::vector<BoardRectSt> vecRects;
                                                    if (BoardRectTable::getBoardRectsBySourceBoardID(sbSt.strBoardID, vecRects))
                                                    {
                                                        // 记录小板板件外包矩形信息
                                                        m_boardRectsMap[sbSt.strBoardID] = vecRects;
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    if (m_materialMap.size() > 0)
                    {
                        std::map<QString, std::map<QString, std::vector<DataSt>>>::iterator mapIt = m_materialMap.begin();
                        QString strMaterialListName = mapIt->first;
                        std::map<QString, std::vector<DataSt>>& groupMap = m_materialMap[strMaterialListName];
                        for (std::map<QString, std::vector<DataSt>>::iterator it = groupMap.begin(); it != groupMap.end(); ++it)
                        {
                            std::vector<DataSt>& vecData = it->second;
                            m_pMaterialTableWidget->setMaterialListName(strMaterialListName);
                            m_pMaterialTableWidget->displayData(vecData);
                        }

                        if (this->centralWidget() != m_pMaterialTableWidget)
                        {
                            this->takeCentralWidget();
                            this->setCentralWidget(m_pMaterialTableWidget);
                        }
                    }

                    /*
            QString strXbelFilePathName = QString("%1/%2.xbel").arg(mgr.getProjectPath()).arg(mgr.getProjectName());

            // 保存工程目录树的状态到xbel文件中
            XbelReader xbelReader(m_pWorkSpaceTreeWidget);

            QFile xbelFile(strXbelFilePathName);
            if (xbelFile.open(QFile::ReadWrite | QFile::Text))
            {
                xbelReader.read(&xbelFile);
                xbelFile.close();
            }
            else
            {
                qWarning("QXmlStream Bookmarks:%s",
                         tr("Cannot write file %1:%2.")
                         .arg(strXbelFilePathName)
                         .arg(xbelFile.errorString()).toStdString().c_str());
            }
            */
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("打开工程失败: 无效数据库文件或数据库文件不存在."), tc->toUnicode("知道了"));
            }
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionOpenProject_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionSystemSettings_triggered()
{
    try
    {
        DialogSystemSettings dlg(m_pWorkSpaceTreeWidget, m_materialMap, this);
        if (dlg.exec() == QDialog::Accepted)
        {

        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionSystemSettings_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionImportMaterialFile_triggered()
{
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        if (mgr.isProjectOpened())
        {
            SystemConfigManager& sMgr = SystemConfigManager::GetInstance();

            QString strFilePathName = QFileDialog::getOpenFileName(this, tc->toUnicode("打开料单"),
                                                                   sMgr.getRecentlyUsedMaterialFileDir(),
                                                                   tr("Files (*.xls)"));
            if (!strFilePathName.isEmpty())
            {
                QFileInfo info(strFilePathName);
                if (info.exists() && info.isFile())
                {
                    QString strPath = info.absoluteDir().path();
                    sMgr.setRecentlyUsedMaterialFileDir(strPath);

                    if(isMaterialFileAlreadyExistsInProjectDir(strFilePathName))
                    {
                        QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("料单已经存在."), tc->toUnicode("知道了"));
                        return;
                    }

                    std::vector<DataSt> vecData;

                    if(!openMaterialFile(strFilePathName, vecData) || !copyMaterialFile2ProjectDir(strFilePathName))
                    {
                        QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("添加料单失败."), tc->toUnicode("知道了"));
                        return;
                    }
                    else
                    {
                        int nBoardNum = 0;
                        std::vector<DataSt>::const_iterator it = vecData.begin();
                        for (; it != vecData.end(); ++it)
                        {
                            const DataSt& st = *it;
                            nBoardNum += st.nBoardNum;
                        }

                        QString strMsg = QString(tc->toUnicode("料单导入完成，共%1块板件")).arg(nBoardNum);
                        QMessageBox::warning(this, tc->toUnicode("警告"), strMsg, tc->toUnicode("知道了"));
                    }
                }
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionImportMaterialFile_triggered() exception: %s", e.what());
    }
}

void MainWindow::onWorkSpaceTreeItemClicked(QTreeWidgetItem* pItem, int /*nColumn*/)
{
    try
    {
        if (pItem)
        {
            bool ok = false;
            int nItemType = pItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok && nItemType == ITEM_TYPE_MATERIAL_LIST)
            {
                m_pMaterialTableWidget->removeRowData();
                QString strMaterialListName = pItem->data(0, MATERIAL_LIST_NAME).toString();
                std::map<QString, std::vector<DataSt>>& groupMap = m_materialMap[strMaterialListName];
                for (std::map<QString, std::vector<DataSt>>::iterator it = groupMap.begin(); it != groupMap.end(); ++it)
                {
                    std::vector<DataSt>& vecData = it->second;
                    m_pMaterialTableWidget->setMaterialListName(strMaterialListName);
                    m_pMaterialTableWidget->displayData(vecData);
                }

                if (this->centralWidget() != m_pMaterialTableWidget)
                {
                    this->takeCentralWidget();
                    this->setCentralWidget(m_pMaterialTableWidget);
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::onItemClicked() exception: %s", e.what());
    }
}

void MainWindow::onWorkSpaceTreeItemDoubleClicked(QTreeWidgetItem* pItem, int /*column*/)
{
    try
    {
        if (pItem)
        {
            bool ok = false;
            int nItemType = pItem->data(0, ITEM_TYPE).toInt(&ok);
            if (ok && nItemType == ITEM_TYPE_CUTTING_SOLUTION)
            {
                QString strSolutionID = pItem->data(0, CUTTING_SOLUTION_ID).toString();
                if (m_solutionMap.find(strSolutionID) != m_solutionMap.end())
                {
                    Solution& solution = m_solutionMap[strSolutionID];

                    m_pSourceBoardListWidget->removeRowData();
                    this->tabifyDockWidget(m_pWorkSpaceDockWidget, m_pSourceBoardListDockWidget);
                    m_pSourceBoardListWidget->displayData(solution.vecSourceBoard);
                }
            }
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::onItemDoubleClicked() exception: %s", e.what());
    }
}

void MainWindow::onDeleteMaterialList(QString strMaterialListName)
{
    if (m_materialMap.find(strMaterialListName) != m_materialMap.end())
    {
        m_materialMap.erase(strMaterialListName);
        m_pMaterialTableWidget->removeRowData();
    }
}

void MainWindow::onDeleteCuttingSolution(QString strSolutionID)
{
    if (m_solutionMap.find(strSolutionID) != m_solutionMap.end())
    {
        m_solutionMap.erase(strSolutionID);
        m_pSourceBoardListWidget->removeRowData();
    }
}

void MainWindow::onItemSelectionChanged()
{
    QList<QTableWidgetSelectionRange> ranges = m_pSourceBoardListWidget->selectedRanges();

    if (ranges.size() == 0)
    {
        return;
    }

    int index = ranges.at(0).topRow();

    QTableWidgetItem* pItem = m_pSourceBoardListWidget->item(index, 0);

    if (pItem)
    {
        //QTextCodec *tc = QTextCodec::codecForName("GBK");
        QString strSolutionID = pItem->data(CUTTING_SOLUTION_ID).toString();
        QString strSourceBoardID = pItem->data(SOURCE_BOARD_ID).toString();
        if (!strSolutionID.isEmpty() && !strSourceBoardID.isEmpty())
        {
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            SystemConfigManager& smgr = SystemConfigManager::GetInstance();
            int nSpace = smgr.getBoardSpacing();

            Solution& solution = m_solutionMap[strSolutionID];
            std::vector<SourceBoardSt>& vecData = solution.vecSourceBoard;
            std::vector<SourceBoardSt>::iterator it = vecData.begin();
            for (; it != vecData.end(); ++it)
            {
                SourceBoardSt& boardSt = *it;
                if (boardSt.strBoardID == strSourceBoardID)
                {
                    m_pSolutionSummaryTableWidget->removeRowData();
                    m_pSolutionSummaryTableWidget->displayData(solution.solutionSt, boardSt, nSpace);

                    QString strDxfDir = QString("%1/%2").arg(mgr.getProjectPath()).arg(tr("dxf"));
                    QString strFilePathName = QString("%1/%2").arg(strDxfDir).arg(boardSt.strDxfFileName);
                    //strFilePathName = tc->fromUnicode(strFilePathName);
                    m_pViewPort->importFile(strFilePathName, "(*.dxf)", boardSt.strBoardID);

                    // 回复工具条按钮状态为未选中
                    m_pModifyCuttingPointToolButton->setChecked(false);
                    m_pEditNCOrderToolButton->setChecked(false);
                    m_pSelectBoardToolButton->setChecked(false);

                    if (this->centralWidget() != m_pCentralWidget)
                    {
                        this->takeCentralWidget();
                        this->setCentralWidget(m_pCentralWidget);
                    }

                }
            }
        }
    }
}

void MainWindow::on_actionPacking_triggered()
{
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
        if (!mgr.isProjectOpened())
        {
            return;
        }

        if (!materialListTest())
        {
            return;
        }

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        // 横向显示，板材的宽高和屏幕的宽高相反
        int nWidth = smgr.getSourceMaterialBoardWidth();
        int nHeight = smgr.getSourceMaterialBoardHeight();

        int nTrimming = smgr.getSourceMaterialBoardTrimming();
        int nSpacing = smgr.getBoardSpacing();
        int nCuttingDepth = smgr.getCuttingDepth();

        double dCuttingPointOffset = smgr.getObliqueDistance();

        QString strDxfDir = QString("%1/%2").arg(mgr.getProjectPath()).arg(tr("dxf"));

        int nSourceBoardWidth = nWidth;
        int nSourceBoardHeight = nHeight;

        QTreeWidgetItem* pRootItem = m_pWorkSpaceTreeWidget->topLevelItem(0);
        if (!pRootItem)
        {
            return;
        }

        QCursor orgCursor = this->cursor();
        this->setCursor(QCursor(Qt::WaitCursor));

        // 删除工程目录下子文件夹dxf中的所有dxf文件
        QStringList nameFilters;
        nameFilters << "*.dxf";
        QDir dxfDir(strDxfDir);
        QStringList dxfFiles = dxfDir.entryList(nameFilters, QDir::Files);
        for (int i = 0; i < dxfFiles.length(); ++i)
        {
            QString& str = dxfFiles[i];
            QFile file(strDxfDir + "/" + str);
            file.remove();
        }

        m_solutionMap.clear();
        m_boardRectsMap.clear();

        m_pSourceBoardListWidget->removeRowData();

        // 下刀点搜索器
        CuttingPointSearcher pointSearcher(dCuttingPointOffset);

        // 料单层
        int nMaterialListNum = pRootItem->childCount();
        for (int i = 0; i < nMaterialListNum; ++i)
        {
            QTreeWidgetItem* pMaterialListItem = pRootItem->child(i);
            if (!pMaterialListItem)
            {
                this->setCursor(orgCursor);
                return;
            }

            // 取出料单名称作为key，从m_materialMap取得所有分组
            QString strMaterialListName = pMaterialListItem->data(0, MATERIAL_LIST_NAME).toString();

            // 分组层
            int nGroupNum = pMaterialListItem->childCount();
            for (int j = 0; j < nGroupNum; ++j)
            {
                QTreeWidgetItem* pGroupItem = pMaterialListItem->child(j);
                if (!pGroupItem)
                {
                    this->setCursor(orgCursor);
                    return;
                }

                QString strGroupID = pGroupItem->data(0, MATERIAL_GROUP_ID).toString();

                // 方案层
                int nSolutionNum = pGroupItem->childCount();
                for (int s = 0; s < nSolutionNum; ++s)
                {
                    QTreeWidgetItem* pSolutionItem = pGroupItem->child(0);
                    if (!pSolutionItem)
                    {
                        this->setCursor(orgCursor);
                        return;
                    }

                    QString strSolutionID = pSolutionItem->data(0, CUTTING_SOLUTION_ID).toString();
                    if (!strSolutionID.isEmpty())
                    {
                        std::vector<SourceBoardSt> vecSbSt;
                        if (SourceBoardTable::getBoardsBySolutionID(strSolutionID, vecSbSt))
                        {
                            // 删除小板件外包矩形信息
                            std::vector<SourceBoardSt>::iterator sbIt = vecSbSt.begin();
                            for (; sbIt != vecSbSt.end(); ++sbIt)
                            {
                                SourceBoardSt& sbSt = *sbIt;
                                if (BoardRectTable::deleteBoardRectsBySourceBoardID(sbSt.strBoardID))
                                {
                                }
                            }
                            // 然后删除原料板信息
                            if (SourceBoardTable::deleteBoardsBySolutionID(strSolutionID))
                            {
                            }
                        }
                    }
                    if (!strGroupID.isEmpty())
                    {
                        // 最后删除相关方案信息
                        if (CuttingSolutionTable::deleteSolutionByMaterialGroupID(strGroupID))
                        {
                            pGroupItem->removeChild(pSolutionItem);
                            delete pSolutionItem;
                            pSolutionItem = NULL;
                        }
                    }
                }

                // 获取分组
                std::map<QString, std::vector<DataSt>>& groupMap = m_materialMap[strMaterialListName];

                // 取出分组名称作为key，从groupMap中取得料单数据
                QString strGroupName = pGroupItem->data(0, MATERIAL_GROUP_NAME).toString();
                std::vector<DataSt>& vecData = groupMap[strGroupName];

                if (!vecData.empty())
                {
                    QString strContext = QString(tc->toUnicode("正在优化排版料单'%1'的分组'%2' ...")).arg(strMaterialListName).arg(strGroupName);
                    ui->statusBar->showMessage(strContext);

                    QApplication::processEvents();

                    std::vector<Solution> vecSolution;

                    // 进行优化排版，获取方案
                    vecSolution = packing(vecData);

                    int nIndex = 0;
                    std::vector<Solution>::iterator sIt = vecSolution.begin();
                    for (; sIt != vecSolution.end(); ++sIt)
                    {
                        Solution& solution = *sIt;
                        solution.solutionSt.strMateriakGroupID = strGroupID;
                        solution.solutionSt.strSolutionName = QString(tc->toUnicode("方案-%1")).arg(++nIndex);
                        if (CuttingSolutionTable::addSolution(solution.solutionSt))
                        {
                            // 将通过addSolution()得到的strSolutionID更新到各个料板中，以便插入SourceBoardTable表
                            std::vector<SourceBoardSt>::iterator sbIt = solution.vecSourceBoard.begin();
                            for (; sbIt != solution.vecSourceBoard.end(); ++sbIt)
                            {
                                SourceBoardSt& sbSt = *sbIt;

                                sbSt.strSolutionID = solution.solutionSt.strSolutionID;
                                sbSt.strDxfFileName = QString("%2-%3.dxf").arg(sbSt.strSolutionID).arg(sbSt.nIdx);;
                            }

                            if (SourceBoardTable::addBoards(solution.vecSourceBoard))
                            {
                                QTreeWidgetItem* pSolutionItem = new QTreeWidgetItem();
                                pSolutionItem->setData(0, ITEM_TYPE, ITEM_TYPE_CUTTING_SOLUTION);
                                pSolutionItem->setData(0, CUTTING_SOLUTION_ID, solution.solutionSt.strSolutionID);
                                pSolutionItem->setData(0, CUTTING_SOLUTION_NAME, solution.solutionSt.strSolutionName);
                                pSolutionItem->setData(0, XBEL_TARGET, "bookmark");
                                pSolutionItem->setText(0, solution.solutionSt.strSolutionName);

                                pGroupItem->addChild(pSolutionItem);

                                double dHalfSpacing = nSpacing/2.0;

                                std::vector<SourceBoardSt>::iterator isbIt = solution.vecSourceBoard.begin();
                                for (; isbIt != solution.vecSourceBoard.end(); ++isbIt)
                                {
                                    SourceBoardSt& sbSt = *isbIt;

                                    // 先修正板块的位置，用于绘制
                                    double dTrimming = nTrimming;

                                    // 采用双精度浮点数来实现高精度图形绘制
                                    std::vector<RectF> vecRectF;

                                    // addBoards()函数执行后得到了sbid，在这个地方才写入数据库表'board_rect'
                                    std::vector<BoardRectSt> vecBoardRect;

                                    for (std::vector<rbp::Rect>::iterator rcIt = sbSt.vecRect.begin(); rcIt != sbSt.vecRect.end(); ++rcIt)
                                    {
                                        rbp::Rect& rc = *rcIt;

                                        RectF rcf;
                                        rcf.name = rc.name;
                                        rcf.barcodeNo = rc.barcodeNo;
                                        rcf.rotate = rc.rotate;
                                        rcf.bRotated = rc.bRotated;
                                        rcf.x = rc.x + dTrimming;
                                        rcf.y = rc.y + dTrimming;
                                        rcf.width = rc.width - nSpacing;
                                        rcf.height = rc.height - nSpacing;

                                        vecRectF.push_back(rcf);
                                    }

                                    pointSearcher.search(vecRectF);

                                    strContext = QString(tc->toUnicode("正在生成料单'%1'的分组'%2'的第%3个dxf文件 ...")).arg(strMaterialListName).arg(strGroupName).arg(sbSt.nIdx);

                                    ui->statusBar->showMessage(strContext);

                                    QApplication::processEvents();

                                    QString strFilePathName = QString("%1/%2-%3.dxf").arg(strDxfDir).arg(sbSt.strSolutionID).arg(sbSt.nIdx);
                                    generateDxf(strFilePathName, vecRectF, nWidth, nHeight, nCuttingDepth);

                                    // 搜索完下刀点，也将板件绘制到dxf文件上，接下来存储板件切割的顺序(nIdx)、下刀点位置、板件外包矩形的相对位置信息
                                    int nIdx = 0;
                                    for (std::vector<RectF>::iterator rcfIt = vecRectF.begin(); rcfIt != vecRectF.end(); ++rcfIt)
                                    {
                                        RectF& rcf = *rcfIt;

                                        BoardRectSt brSt;
                                        brSt.strSourceBoardID = sbSt.strBoardID;
                                        brSt.strBoardRectName = QString(rcf.name.c_str());
                                        brSt.strBarcodeNo = QString(rcf.barcodeNo.c_str());
                                        brSt.nIdx = ++nIdx;

                                        // 坐标点上移半个板间距，使板件落在其外包矩形的正中央
                                        brSt.dLtX = (double)(rcf.x-dHalfSpacing)/nSourceBoardWidth;
                                        brSt.dLtY = (double)(rcf.y-dHalfSpacing)/nSourceBoardHeight;
                                        brSt.dRbX = (double)((rcf.x-dHalfSpacing)+(rcf.width+nSpacing))/nSourceBoardWidth;
                                        brSt.dRbY = (double)((rcf.y-dHalfSpacing)+(rcf.height+nSpacing))/nSourceBoardHeight;
                                        brSt.dNcStartPtX = (rcf.ctx)/nSourceBoardWidth;
                                        brSt.dNcStartPtY = (rcf.cty)/nSourceBoardHeight;
                                        vecBoardRect.push_back(brSt);
                                    }

                                    // 记录小板板件外包矩形信息
                                    m_boardRectsMap[sbSt.strBoardID] = vecBoardRect;

                                    if (BoardRectTable::addBoardRects(vecBoardRect))
                                    {
                                    }
                                }
                            }
                        }

                        m_solutionMap[solution.solutionSt.strSolutionID] = solution;
                    }
                }
                else
                {
                    qCritical("Empty parameter 'std::vector<DataSt>& vecData' pass to function std::vector<Solution> MainWindow::packing(std::vector<DataSt>& vecData).\n");
                }

                // 准备相应方案的板块用料数据
            }
        }

        this->tabifyDockWidget(m_pSourceBoardListDockWidget, m_pWorkSpaceDockWidget);
        QString strContext(tc->toUnicode("完成"));
        ui->statusBar->showMessage(strContext, 5000);
        this->setCursor(orgCursor);
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionPacking_triggered() exception: %s", e.what());
    }
}

bool MainWindow::materialBoardSizeTest(int nMaxWidth, int nMaxHeight, int nSpacing, const std::vector<DataSt>& vecData)
{
    bool isLeagl = true;

    std::vector<DataSt>::const_iterator it = vecData.begin();
    for (; it != vecData.end(); ++it)
    {
        const DataSt& st = *it;

        // 小板件的宽、高都小于最大值才合法，否则，看该小板件是否允许旋转
        if (st.nBoardWidth+nSpacing > nMaxWidth || st.nBoardHeight+nSpacing > nMaxHeight)
        {
            // 旋转后是否合法
            if (st.nRotate == 1)
            {
                if (st.nBoardWidth+nSpacing > nMaxHeight || st.nBoardHeight+nSpacing > nMaxWidth)
                {
                    isLeagl = false;
                    break;
                }
            }
            else
            {
                isLeagl = false;
                break;
            }
        }
    }

    return isLeagl;
}

bool MainWindow::materialListTest()
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    if (mgr.isProjectOpened() == true)
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        int nSourceBoardWidth = smgr.getSourceMaterialBoardWidth();
        int nSourceBoardHeight = smgr.getSourceMaterialBoardHeight();

        int nBoardTrimming = smgr.getSourceMaterialBoardTrimming();
        int nBoardSpacing = smgr.getBoardSpacing();

        int nMaxWidth = 0;
        int nMaxHeight = 0;

        double dSpacing = nBoardSpacing;
        double dTrimming = nBoardTrimming;

        // 调整大料板宽度
        nMaxWidth = nSourceBoardWidth + (dSpacing/2.0-dTrimming)*2;
        nMaxHeight = nSourceBoardHeight + (dSpacing/2.0-dTrimming)*2;

        std::map<QString, std::vector<DataSt>> materialMap;
        if (MaterialListTable::getMaterialLists(materialMap))
        {
            std::map<QString, std::vector<DataSt>>::iterator mapIt = materialMap.begin();
            for (; mapIt != materialMap.end(); ++mapIt)
            {
                if (!materialBoardSizeTest(nMaxWidth, nMaxHeight, nBoardSpacing, mapIt->second))
                {
                    QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("不合法的板件尺寸或切割参数."), tc->toUnicode("知道了"));
                    return false;
                }
            }
        }
    }

    return true;
}

bool MainWindow::materialTest(const std::vector<DataSt>& vecData)
{
    ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();
    if (mgr.isProjectOpened() == true)
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        SystemConfigManager& smgr = SystemConfigManager::GetInstance();

        int nSourceBoardWidth = smgr.getSourceMaterialBoardWidth();
        int nSourceBoardHeight = smgr.getSourceMaterialBoardHeight();

        int nBoardTrimming = smgr.getSourceMaterialBoardTrimming();
        int nBoardSpacing = smgr.getBoardSpacing();

        int nMaxWidth = 0;
        int nMaxHeight = 0;

        double dSpacing = nBoardSpacing;
        double dTrimming = nBoardTrimming;

        // 调整大料板宽度
        nMaxWidth = nSourceBoardWidth + (dSpacing/2.0-dTrimming)*2;
        nMaxHeight = nSourceBoardHeight + (dSpacing/2.0-dTrimming)*2;

        if (!materialBoardSizeTest(nMaxWidth, nMaxHeight, nBoardSpacing, vecData))
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("不合法的板件尺寸或切割参数."), tc->toUnicode("知道了"));
            return false;
        }
    }

    return true;
}

// 绘制小板件（不含锯缝，是准确的成品小板件）
bool MainWindow::generateDxf(const QString& strFilePathName,
                             const std::vector<RectF>& vecRect,
                             int nWidth,
                             int nHeight,
                             int nDepth)
{
    RMemoryStorage* pStorage = new RMemoryStorage();
    RSpatialIndexNavel* pSpatialIndex = new RSpatialIndexNavel();
    RDocument* pDocument = new RDocument(*pStorage, *pSpatialIndex);

    // 添加板件轮廓
    RMixedOperation* op = new RMixedOperation();

    std::vector<RectF>::const_iterator it = vecRect.begin();
    for (; it != vecRect.end(); ++it)
    {
        const RectF& rc = *it;

        if  (it == vecRect.begin())
        {
            RVector lt(0, 0, nDepth);

            QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                        new RPolylineEntity(pDocument, RPolylineData()));

            RVector lb(nHeight, -0, nDepth);
            RVector rb(nHeight, -nWidth, nDepth);
            RVector rt(0, -nWidth, nDepth);
            polylineEntity->appendVertex(lt);
            polylineEntity->appendVertex(lb);
            polylineEntity->appendVertex(rb);
            polylineEntity->appendVertex(rt);
            polylineEntity->setClosed(true);
            polylineEntity->setColor(RColor(0, 128, 255));
            //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
            polylineEntity->setLineweight(RLineweight::Weight050);

            op->addObject(polylineEntity, false);
        }

        /*
        RVector lt(rc.x, -rc.y, nDepth);
        RVector lb(rc.x, -(rc.y+rc.height), nDepth);
        RVector rb(rc.x+rc.width, -(rc.y+rc.height), nDepth);
        RVector rt(rc.x+rc.width, -rc.y, nDepth);
        */

        RVector lt(rc.y, -rc.x, nDepth);
        RVector lb(rc.y, -(rc.x+rc.width), nDepth);
        RVector rb(rc.y+rc.height, -(rc.x+rc.width), nDepth);
        RVector rt(rc.y+rc.height, -rc.x, nDepth);

        bool hasDissimilar = false;

        // -2 只是用来初始化这个变量
        int nID = -2;
        std::map<QString, QVariant> paramsMap;

        if (getDissimilarParametersByBarcodeNo(rc.barcodeNo.c_str(), paramsMap))
        {
            if (paramsMap.find("ID") != paramsMap.end())
            {
                QString strID = paramsMap["ID"].toString();
                bool ok = false;
                nID = strID.toInt(&ok);
                if (ok)
                {
                    // 有异形就绘制异形，否则绘制多边形（矩形）
                    if (DissimilarLibraryOperator::drawRPolyline(pDocument, op, lt, nDepth, rc.bRotated, nID, paramsMap))
                    {
                        hasDissimilar = true;
                    }
                }
            }
        }

        // nID == -1 为用户自定义异形
        if (nID == -1)
        {
            ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

            QString projectPath = mgr.getProjectPath();

            QString fileName = projectPath + QString("/shape/%1.dxf").arg(rc.barcodeNo.c_str());

            RVector center((lt.x+rb.x)/2.0, (lt.y+rb.y)/2.0);
            double rotationDegrees = -90.0;
            if (rc.bRotated)
            {
                rotationDegrees = -180.0;
            }

            EG::copyShapes2Document(fileName, center, rotationDegrees, false, false, *pDocument);
        }
        // 非异形
        else if (!hasDissimilar)
        {
            RPolyline polyline;

            polyline.appendVertex(lt);
            polyline.appendVertex(lb);
            polyline.appendVertex(rb);
            polyline.appendVertex(rt);
            polyline.setClosed(true);

            QSharedPointer<RPolylineEntity> polylineEntity = QSharedPointer<RPolylineEntity>(
                        new RPolylineEntity(pDocument, RPolylineData(polyline)));
            polylineEntity->setColor(RColor(0, 128, 255));
            //polylineEntity->setLinetypeId(pDocument->getLinetypeId("dashed"));
            polylineEntity->setLineweight(RLineweight::Weight050);

            op->addObject(polylineEntity, false);
        }
    }

    op->apply(*pDocument);

    RLayer::Id id = pDocument->getCurrentLayerId();
    QSharedPointer<RLayer> layer0 = pDocument->queryLayerDirect(id);
    if (!layer0.isNull())
    {
        layer0->setLocked(true);
    }

    // 如果没有图层“1”，就添加一个名字为“1”的图层，该图层用来绘制板件信息
    if (!pDocument->hasLayer("1"))
    {
        QSharedPointer<RLayer> pLayer = QSharedPointer<RLayer>(new RLayer(pDocument, "1"));
        pLayer->setLinetypeId(pDocument->getLinetypeId("continuous"));

        RAddObjectOperation* opAddLayer = NULL;
        opAddLayer = new RAddObjectOperation(pLayer);
        opAddLayer->apply(*pDocument);
    }

    // 添加板件信息（为避免频繁apply()，不放在上面的for里面和添加板件轮廓一起添加）
    RLayer::Id layerId = pDocument->getLayerId("1");
    if (layerId != RLayer::INVALID_ID)
    {
        RLayer::Id currentLayerId = pDocument->getCurrentLayerId();

        if (layerId != currentLayerId)
            pDocument->setCurrentLayer(layerId);

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        RAddObjectOperation* opAddInfo = new RAddObjectOperation();

        for (it = vecRect.begin(); it != vecRect.end(); ++it)
        {
            const RectF& rc = *it;

            /*
            RVector lt(rc.x, -rc.y, 0.0);
            RVector rb(rc.x+rc.width, -(rc.y+rc.height), 0.0);
            */

            RVector lt(rc.y, -rc.x, 0.0);
            RVector rb(rc.y+rc.height, -(rc.x+rc.width), 0.0);

            RBox box(lt, rb);
            RTextData textData(RVector(),
                               box.getCenter(),
                               30.0f,
                               0.0f,
                               RS::VAlignMiddle,
                               RS::HAlignCenter,
                               RS::LeftToRight,
                               RS::Exact,
                               1.0f,
                               "",
                               tc->toUnicode("微软雅黑"),
                               false,
                               false,
                               0.0f,
                               false);

            QSharedPointer<RTextEntity> textEntity = QSharedPointer<RTextEntity>(new RTextEntity(pDocument, textData));
            std::string strCode;
            if (rc.barcodeNo.length() >= 4)
            {
                strCode = rc.barcodeNo.substr(rc.barcodeNo.length()-4, 4);
            }
            else
            {
                strCode = rc.barcodeNo;
            }

            RBox textBox = textEntity->getBoundingBox();

            QString strMsg;

            if (rc.bRotated)
            {
                strMsg = QString("(%1)%2 %3x%4").arg(strCode.c_str()).arg(rc.name.c_str()).arg(rc.width).arg(rc.height);
            }
            else
            {
                strMsg = QString("(%1)%2 %3x%4").arg(strCode.c_str()).arg(rc.name.c_str()).arg(rc.height).arg(rc.width);
            }

            textEntity->setText(strMsg);

            if (rc.height < rc.width)
            {
                textEntity->rotate(-M_PI_2, textBox.getCenter());
            }

            textBox = textEntity->getBoundingBox();

            double dH = 30.0f;
            while(!box.containsBox(textBox) && dH > 0)
            {
                dH -= 2.0f;
                textEntity->setTextHeight(dH);
                textBox = textEntity->getBoundingBox();
            }

            opAddInfo->addObject(textEntity, false);
        }

        opAddInfo->apply(*pDocument);
    }

    id = pDocument->getCurrentLayerId();
    QSharedPointer<RLayer> layer1 = pDocument->queryLayerDirect(id);
    if (!layer1.isNull())
    {
        layer1->setLocked(true);
    }

    // 如果没有图层“2”，就添加一个名字为“2”的图层，该图层用来绘制板序号和下刀点
    if (!pDocument->hasLayer("2"))
    {
        QSharedPointer<RLayer> pLayer = QSharedPointer<RLayer>(new RLayer(pDocument, "2"));
        pLayer->setLinetypeId(pDocument->getLinetypeId("continuous"));

        RAddObjectOperation* opAddLayer = NULL;
        opAddLayer = new RAddObjectOperation(pLayer);
        opAddLayer->apply(*pDocument);
    }

    // 添加板件信息（为避免频繁apply()，不放在上面的for里面和添加板件轮廓一起添加）
    layerId = pDocument->getLayerId("2");
    if (layerId != RLayer::INVALID_ID)
    {
        RLayer::Id currentLayerId = pDocument->getCurrentLayerId();

        if (layerId != currentLayerId)
            pDocument->setCurrentLayer(layerId);

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        RAddObjectOperation* opAddPoint = new RAddObjectOperation();
        int nOrder = 0;
        for (it = vecRect.begin(); it != vecRect.end(); ++it)
        {
            const RectF& rc = *it;

            RVector lt(rc.y+30, -(rc.x+30), nDepth);

            RTextData textData(RVector(),
                               lt,
                               30.0f,
                               0.0f,
                               RS::VAlignMiddle,
                               RS::HAlignCenter,
                               RS::LeftToRight,
                               RS::Exact,
                               1.0f,
                               "",
                               tc->toUnicode("微软雅黑"),
                               false,
                               false,
                               0.0f,
                               false);

            QSharedPointer<RTextEntity> textEntity = QSharedPointer<RTextEntity>(new RTextEntity(pDocument, textData));

            QString strMsg = QString("%1").arg(++nOrder);
            textEntity->setText(strMsg);

            opAddPoint->addObject(textEntity, false);

            RPointData pointData(RVector(rc.cty, -rc.ctx));
            QSharedPointer<RPointEntity> pointEntity = QSharedPointer<RPointEntity>(new RPointEntity(pDocument, pointData));

            opAddPoint->addObject(pointEntity, false);
        }

        opAddPoint->apply(*pDocument);
    }

    pDocument->setCurrentLayer("0");

    RDxfExporter* pDxfExporter = new RDxfExporter(*pDocument);
    pDxfExporter->exportFile(strFilePathName, "");
    pDocument->setFileName(strFilePathName);
    pDxfExporter->exportDocument();

    delete pDxfExporter;
    pDxfExporter = NULL;

    delete pDocument;
    pDocument = NULL;

    return true;
}

void MainWindow::resetActionToolButtonClicked(bool /*checked*/)
{
    m_pHidenCuttingPointLayerButton->setEnabled(true);

    if (m_pModifyCuttingPointToolButton->isChecked())
    {
        m_pModifyCuttingPointToolButton->setChecked(false);
    }

    if (m_pEditNCOrderToolButton->isChecked())
    {
        m_pEditNCOrderToolButton->setChecked(false);
    }

    if (m_pSelectBoardToolButton->isChecked())
    {
        m_pSelectBoardToolButton->setChecked(false);
    }

}

void MainWindow::modifyCuttingPointToolButtonClicked(bool checked)
{
    if (checked)
    {
        m_pHidenCuttingPointLayerButton->setEnabled(false);
        m_pEditNCOrderToolButton->setChecked(false);
        m_pSelectBoardToolButton->setChecked(false);
    }
    else
    {
        m_pHidenCuttingPointLayerButton->setEnabled(true);
    }
}

void MainWindow::editNCOrderToolButtonClicked(bool checked)
{
    if (checked)
    {
        m_pModifyCuttingPointToolButton->setChecked(false);
        m_pSelectBoardToolButton->setChecked(false);
    }
    else
    {

    }
}

void MainWindow::selectBoardToolButtonClicked(bool checked)
{
    if (checked)
    {
        m_pEditNCOrderToolButton->setChecked(false);
        m_pModifyCuttingPointToolButton->setChecked(false);
    }
    else
    {
    }
}

void MainWindow::onUpdateDissimilar(const QString& strMaterialListName, const QString& strBarcodeNo, const QString& strParameters)
{
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");
        if (ProductBoardTable::updateDissimilarParametersByBarcodeNo(strBarcodeNo, strParameters))
        {
            bool isFound = false;

            // 更新 m_materialMap
            std::map<QString, std::vector<DataSt>>& groupMap = m_materialMap[strMaterialListName];
            for (std::map<QString, std::vector<DataSt>>::iterator it = groupMap.begin(); it != groupMap.end(); ++it)
            {
                std::vector<DataSt>& vecData = it->second;
                for (std::vector<DataSt>::iterator vecIt = vecData.begin(); vecIt != vecData.end(); ++vecIt)
                {
                    DataSt& dataSt = *vecIt;
                    if (dataSt.strBarCodeNo == strBarcodeNo)
                    {
                        dataSt.strDissimilarShapes = strParameters;
                        isFound = true;
                        break;
                    }
                }
                if (isFound)
                {
                    break;
                }
            }
        }
        else
        {
            QMessageBox::warning(this, tc->toUnicode("警告"), tc->toUnicode("更新异形数据库失败."), QMessageBox::Ok);
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionImportMaterialFile_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionSaveProject_triggered()
{
    if (m_pViewPort)
    {
        m_pViewPort->saveCurrentDocument();
    }
}

void MainWindow::on_actionExportCNC_triggered()
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        if (mgr.isProjectOpened())
        {
            if (this->centralWidget() == m_pCentralWidget)
            {
                if (m_pViewPort)
                {
                    m_pViewPort->exportCurrentDocument2NC();
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先选择一张料板."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionImportMaterialFile_triggered() exception: %s", e.what());
    }
}

bool MainWindow::getDissimilarParametersByBarcodeNo(const QString& strBarcodeNo, std::map<QString, QVariant>& paramsMap)
{
    std::map<QString, std::map<QString, std::vector<DataSt>>>::iterator materialIt = m_materialMap.begin();
    for (; materialIt != m_materialMap.end(); ++materialIt)
    {
        std::map<QString, std::vector<DataSt>>& groupMap = materialIt->second;
        std::map<QString, std::vector<DataSt>>::iterator groupIt = groupMap.begin();
        for (; groupIt != groupMap.end(); ++groupIt)
        {
            std::vector<DataSt>& vecData = groupIt->second;
            for (std::vector<DataSt>::iterator vecIt = vecData.begin(); vecIt != vecData.end(); ++vecIt)
            {
                DataSt& dataSt = *vecIt;
                if (dataSt.strBarCodeNo == strBarcodeNo)
                {
                    paramsMap.clear();

                    QStringList sl = dataSt.strDissimilarShapes.split(';');
                    for (int i = 0; i < sl.size(); ++i)
                    {
                        QString str = sl.at(i);
                        if (!str.isEmpty())
                        {
                            QStringList slParam = str.split('=');
                            if (slParam.size() >= 2)
                            {
                                QString strKey = slParam.at(0);
                                QString strValue = slParam.at(1);
                                bool ok = false;
                                strValue.toDouble(&ok);
                                if (ok)
                                {
                                    paramsMap[strKey] = strValue;
                                }
                                else
                                {
                                    return false;
                                }
                            }
                        }
                    }

                    if (paramsMap.size() > 0)
                    {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

void MainWindow::on_actionImportDXF_triggered()
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        if (mgr.isProjectOpened())
        {
            if (this->centralWidget() == m_pCentralWidget)
            {
                m_pViewPort->replaceCurrentDocument();
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先选择一张料板."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionImportMaterialFile_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionExportDXF_triggered()
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        if (mgr.isProjectOpened())
        {
            if (this->centralWidget() == m_pCentralWidget)
            {
                if (m_pViewPort)
                {
                    m_pViewPort->saveCurrentDocumentAs();
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先选择一张料板."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionImportMaterialFile_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionPrintDrawing_triggered()
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        if (mgr.isProjectOpened())
        {
            if (this->centralWidget() == m_pCentralWidget)
            {
                if (m_pViewPort)
                {
                    m_pViewPort->exportCurrentDocument2Bitmap();
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先选择一张料板."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionPrintDrawing_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionPrintLabel_triggered()
{
    try
    {
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        QTextCodec *tc = QTextCodec::codecForName("GBK");

        if (mgr.isProjectOpened())
        {
            if (this->centralWidget() == m_pCentralWidget)
            {
                if (m_pViewPort)
                {
                    m_pViewPort->printSelectedBoardLabel();
                }
            }
            else
            {
                QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先选择一张料板."), tc->toUnicode("知道了"));
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionPrintLabel_triggered() exception: %s", e.what());
    }
}


void MainWindow::on_actionPrintSettings_triggered()
{
    DialogPrintSettings dlg;
    if (QDialog::Accepted == dlg.exec())
    {
    }
}

void MainWindow::on_actionBatchPrintLabel_triggered()
{
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        // 打开工程配置文件
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        if (mgr.isProjectOpened())
        {
            DialogPrintBoards dlg(this);
            if (QDialog::Accepted == dlg.exec())
            {
            }
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionBatchPrintLabel_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionExportAllBoardCNC_triggered()
{
    try
    {
        QTextCodec *tc = QTextCodec::codecForName("GBK");

        QCursor orgCursor = this->cursor();
        setCursor(QCursor(Qt::WaitCursor));

        ui->statusBar->showMessage(tc->toUnicode("正在导出NC文件......"));

        ui->statusBar->showMessage(tc->toUnicode("完成"), 5000);

        setCursor(orgCursor);

        // 打开工程配置文件
        ProjectConfigManager& mgr = ProjectConfigManager::GetInstance();

        if (mgr.isProjectOpened())
        {
            SystemConfigManager& smgr = SystemConfigManager::GetInstance();

            QString strRecentlyUsedNCDir = smgr.getRecentlyUsedExportNCFileDir();
            QString strNCExportDir = QFileDialog::getExistingDirectory(NULL, tc->toUnicode("选择CNC文件保存目录"), strRecentlyUsedNCDir);
            smgr.setRecentlyUsedDExportNCFileDir(strNCExportDir);

            ui->statusBar->showMessage(tc->toUnicode("正在导出NC文件......"));

            CNCBatchExporter::exportCNC(strNCExportDir);

            ui->statusBar->showMessage(tc->toUnicode("完成"), 5000);
        }
        else
        {
            QMessageBox::information(this, tc->toUnicode("信息"), tc->toUnicode("请先打开工程或新建一个工程."), tc->toUnicode("知道了"));
        }
    }
    catch (QException& e)
    {
        qCritical("MainWindow::on_actionBatchPrintLabel_triggered() exception: %s", e.what());
    }
}

void MainWindow::on_actionQuit_triggered()
{
    MainWindow::close();
}

void MainWindow::on_actionAbout_triggered()
{
    DialogAbout dlg;
    if (dlg.exec() == QDialog::Accepted)
    {

    }
}
