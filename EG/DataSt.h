#ifndef DATAST_H
#define DATAST_H

#include <QString>

// 在Excel料单中的（列号、名称）
// 1、订单号 2、条码编号 3、产品名称 4、成品名称 5、材质名称 6、开料长度 7、开料宽度
// 8、开料厚度 9、需切数量 10、封边量 11、成品长度 12、成品宽度 13、成品厚度 14、异型 15、旋转 16、客户信息 17、加盟店

typedef struct DataSt
{
    QString strProductBoardID;
    QString strMaterialListID;
                                            // （列号、名称）
    QString strOrdersNo;                    // 1、订单号
    QString strBarCodeNo;                   // 2、条码编号
    QString strProductName;                 // 3、产品名称
    QString strFinishedProductName;         // 4、成品名称
    QString strMaterialType;                // 5、材质名称
    int nBoardWidth;                        // 6、开料长度
    int nBoardHeight;                       // 7、开料宽度
    int nBoardThickness;                    // 8、开料厚度
    int nBoardNum;                          // 9、需切数量
    QString strEdgeWeight;                  // 10、封边量
    int nFinishedProductWidth;              // 11、成品长度
    int nFinishedProductHeight;             // 12、成品宽度
    int nFinishedProductThickness;          // 13、成品厚度
    QString strDissimilarShapes;            // 14、异型
    int nRotate;                            // 15、旋转
    QString strClientInformation;           // 16、客户信息
    QString strFranchiseStore;              // 17、加盟店
    int nIsRotated;                         // xx、在优化排版过程中是否被旋转了

    DataSt()
    {
        strProductBoardID = "";
        strMaterialListID = "";

        strOrdersNo = "";
        strBarCodeNo = "";
        strProductName = "";
        strFinishedProductName = "";
        strMaterialType = "";

        nBoardWidth = 0;
        nBoardHeight = 0;
        nBoardThickness = 0;
        nBoardNum = 0;
        strEdgeWeight = "";
        nFinishedProductWidth = 0;
        nFinishedProductHeight = 0;
        nFinishedProductThickness = 0;
        strDissimilarShapes = "";
        nRotate = 0;
        strClientInformation = "";
        strFranchiseStore = "";
        nIsRotated = 0;
    }
} DataSt;

#endif // DATAST_H
