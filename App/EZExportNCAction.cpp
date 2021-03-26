#include "EZExportNCAction.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QTextCodec>
#include <QMap>
#include <QVariant>
#include "RDocument.h"
#include "RDocumentInterface.h"
#include "BoardRectTable.h"
#include "SystemConfigManager.h"
#include "RPolyline.h"
#include "RPolylineData.h"
#include "RPolylineEntity.h"
#include "RArc.h"
#include "RArcData.h"
#include "RArcEntity.h"
#include "RLine.h"
#include "RLineData.h"
#include "RLineEntity.h"
#include "RCircle.h"
#include "RCircleData.h"
#include "RCircleEntity.h"
#include "RShape.h"
#include "RMixedOperation.h"
#include "RAddObjectOperation.h"
#include "RPoint.h"
#include "RPointData.h"
#include "RPointEntity.h"
#include "PolylineEquidistant.h"
#include "CNCGenerator.h"


static bool cmp(QSharedPointer<REntity> e1, QSharedPointer<REntity> e2)
{
    RBox box1 = e1->getBoundingBox();
    RBox box2 = e2->getBoundingBox();
    double dArea1 = box1.getWidth()*box2.getHeight();
    double dArea2 = box2.getWidth()*box2.getHeight();

    return dArea1 < dArea2;
}

EZExportNCAction::EZExportNCAction(const QString& strSourceBoardID, RGuiAction* pGuiAction)
    : m_strSourceBoardID(strSourceBoardID),
      RActionAdapter(pGuiAction)
{

}

EZExportNCAction::~EZExportNCAction()
{

}

void EZExportNCAction::beginEvent()
{
    if (!documentInterface)
    {
        this->terminate();
        return;
    }

    RDocument& document = documentInterface->getDocument();

    RLayer::Id layerId = document.getLayerId("0");

    if (layerId != RLayer::INVALID_ID)
    {
        RLayer::Id currentLayerId = document.getCurrentLayerId();

        if (layerId != currentLayerId)
        {
            document.setCurrentLayer(layerId);
        }
    }

    SystemConfigManager& smgr = SystemConfigManager::GetInstance();
    QString strDir = smgr.getRecentlyUsedExportNCFileDir();

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    QString strSaveFilePathName = QFileDialog::getSaveFileName(NULL, tc->toUnicode("保存NC文件"),
                                                               strDir,
                                                               "NC Files (*.nc)");
    if (strSaveFilePathName.isEmpty() || strSaveFilePathName.isNull())
    {
        this->terminate();
        return;
    }

    QFileInfo inf(strSaveFilePathName);
    smgr.setRecentlyUsedDExportNCFileDir(inf.path());

    double dSourceBoardW = smgr.getSourceMaterialBoardWidth();
    double dSourceBoardH = smgr.getSourceMaterialBoardHeight();

    int nCuttingSpeed = smgr.getCuttingSpeed();
    int nProcessSpeed = smgr.getProcessingSpeed();
    int nTerminalSpped = smgr.getTerminalSpeed();

    double dSafetyHeight = smgr.getSafetyHeight();
    double dCuttingDepth = smgr.getCuttingDepth();
    double dKnifeDiameter = smgr.getKinfeDiameter();
    int nUseDrills = smgr.getUseDrills();
    QString strSlotKnifeType = smgr.getSlotKnifeType();
    QMap<QString, QVariant> drillsMap;
    smgr.getDrills(drillsMap);
    QMap<QString, QVariant> holesMap;
    smgr.getHoles(holesMap);
    QString strKnifeChangeMode = smgr.getKnifeChangeMode();
    QString strNCFileHeader = smgr.getNCFileHeader();
    QString strNCFileTail = smgr.getNCFileTail();


    CNCGenerator generator(nCuttingSpeed,
                           nProcessSpeed,
                           nTerminalSpped,
                           dSafetyHeight,
                           dCuttingDepth,
                           dKnifeDiameter,
                           strSlotKnifeType,
                           nUseDrills,
                           drillsMap,
                           holesMap,
                           dSourceBoardW,
                           dSourceBoardH,
                           strKnifeChangeMode,
                           strNCFileHeader,
                           strNCFileTail);

    int nRet = generator.generateCNC(document, m_strSourceBoardID, strSaveFilePathName);
    if (1 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 无法在折线中插入下刀点, 因为原折线不是闭合的."), QMessageBox::Ok);
    }
    else if (2 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 无法在折线中插入下刀点, 因为原折线线段数量小于3."), QMessageBox::Ok);
    }
    else if (3 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 无法在折线中插入下刀点, 因为下刀点不在折线上."), QMessageBox::Ok);
    }
    else if (4 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 无法在折线中插入下刀点, 因为不允许下刀点在圆弧上."), QMessageBox::Ok);
    }
    else if (5 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 未知错误."), QMessageBox::Ok);
    }
    else if (6 == nRet)
    {
        QMessageBox::warning(NULL, tc->toUnicode("警告"), tc->toUnicode("生成CNC文件失败: 动态转换实体失败."), QMessageBox::Ok);
    }
    else if (7 == nRet)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("生成CNC文件失败: 因为拉槽刀具T2无法加工拉槽."), QMessageBox::Ok);
    }
    else if (8 == nRet)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("生成CNC文件失败: 因为拉槽刀具T3无法加工拉槽."), QMessageBox::Ok);
    }
    else if (9 == nRet)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("生成CNC文件失败: 因为无法打开一个nc文件用以写入G代码."), QMessageBox::Ok);
    }
}
