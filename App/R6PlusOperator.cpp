#include "R6PlusOperator.h"
#include <QTextCodec>
#include <QMessageBox>
#include <Windows.h>
#include "DicPlus.h"
#include <stdio.h>

R6PlusOperator& R6PlusOperator::GetInstance()
{
    static R6PlusOperator op;
    return op;
}

R6PlusOperator::R6PlusOperator(void)
    : m_hic(-1)
{
}

R6PlusOperator::~R6PlusOperator(void)
{
    DicPlus_Close(m_hic);
}

int R6PlusOperator::verrifyDongle()
{
    int errorCode;                // Error code
    char dataSrc[256] = {0};	  // Source data
    char dataEnc[256] = {0};	  // Encrypted/decrypted data
    char buffer[256] = {0};       // 运行结果数据

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    //find dongle
    printf("find dongle : ");
    int num = DicPlus_Find();
    printf("%d\n\n",num);
    if (num <= 0)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("找不到可用的加密锁，请插入加密锁"), tc->toUnicode("知道了"));
        printf("cannot find any dongle\n");
        return -1;
    }

    int i = 0;
    for (i = 0; i < num; i++)
    {
        m_hic = DicPlus_Open(i, NULL);
        if (m_hic >= 0) break;		// Just use the first dongle
    }
    if (i == num)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("打开加密锁失败"), tc->toUnicode("知道了"));
        printf("open dongle failed\n");
        return -1;
    }

    char hardData[16];

    errorCode = DicPlus_Command(m_hic, GET_HARDWARE_INFO, hardData);

    if (errorCode != 0)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        printf("find dongle hardware information failed\n");
        return -1;
    }

    for (int i = 0; i < 16; i++)
    {
        printf("%02X ", (BYTE)hardData[i]);
    }

    printf("\n");

    char randomData[16];
    errorCode = DicPlus_Set(randomData, RANDOM_SIZE, BY_VALUE, 16, NULL);	// 设置要取的随机数长度
    if (errorCode != 0)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        printf("set random data size failed\n");
        return -1;
    }

    errorCode = DicPlus_Command(m_hic, RANDOM, randomData);                     // 获取随机数
    if (errorCode != 0)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        printf("get random data from dongle failed\n");
        return -1;
    }

    for (int i = 0; i < 16; i++)
    {
        printf("%02X ", (BYTE)randomData[i]);
    }

    printf("\n\n");

    ZeroMemory(dataSrc, 128);
    memcpy(dataSrc, hardData, 16);
    memcpy(dataSrc+16, randomData, 16);

    ZeroMemory(dataEnc, 128);

    printf("source data\n");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ",(BYTE)dataSrc[i]);
        if((i+1)%16 == 0)
            printf("\n");
    }
    printf("\n");

    errorCode = DicPlus_DesEncrypt(m_hic, 0x1002, 1, 32, dataSrc, dataEnc);

    printf("encrypted source data\n");
    for(int i = 0; i < 32; i++)
    {
        printf("%02x ",(BYTE)dataEnc[i]);
        if((i+1)%16 == 0)
            printf("\n");
    }
    printf("\n");

    WORD len = 0;

    //run algorithm
    errorCode = DicPlus_Run(m_hic, 0x6002, 32, dataEnc, &len, buffer);

    if (errorCode != SCARD_S_SUCCESS)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        printf("run algorithm failed : %8x \n", errorCode);
        return -1;
    }

    printf("return data\n");
    for(int i = 0; i < len; i++)
    {
        printf("%02x ",(BYTE)buffer[i]);
        if((i+1)%16 == 0)
            printf("\n");
    }
    printf("\n");

    //call dongle to decrypt the data
    errorCode = DicPlus_DesDecrypt(m_hic, 0x1002, 2, 32, buffer, buffer);

    if (errorCode != SCARD_S_SUCCESS)
    {
        printf("des decrypt error : %8x \n",errorCode);
    }
    else
    {
        printf("des decrypted result:\n");
        for(int i = 0; i < 32; i++)
        {
            printf("%02x ",(BYTE)buffer[i]);
            if((i+1)%16 == 0)
                printf("\n");
        }
        printf("\n");
    }

    if (buffer[0] == 0x00)
    {
        printf("pass dongle verrify\n\n");
        return 0;
    }
    else
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        printf("dongle verrify failed\n\n");
        return -1;
    }
}

int R6PlusOperator::checkDongle()
{
    int errorCode = -1;

    char hardData[16];

    errorCode = DicPlus_Command(m_hic, GET_HARDWARE_INFO, hardData);

    if (errorCode != SCARD_S_SUCCESS)
    {
        printf("cannot find device : %8x \n", errorCode);
        return -1;
    }

    return 0;
}

//int R6PlusOperator::verryDongle()
//{
//    int errorCode;                // Error code
//    int num;                    // Smartcard reader amount
//    char dataSrc[256] = {0};	// Source data
//    char dataEnc[256] = {0};	// Encrypted/decrypted data
//    char buffer[256] = {0};     // 运行结果数据

//    //find dongle
//    printf("find dongle : ");
//    num = DicPlus_Find();
//    printf("%d\n",num);
//    if (num <= 0)
//    {
//        printf("cannot find any dongle\n");
//        return -1;
//    }

//    //open first finded dongle
//    printf("open dongle\n");
//    int i = 0;
//    for (i = 0; i < num; i++)
//    {
//        m_hic = DicPlus_Open(i, NULL);
//        if (m_hic >= 0) break;		// Just use the first dongle
//    }
//    if (i == num)
//    {
//        printf("open dongle failed\n");
//        return -1;
//    }

//    char hardData[16];

//    errorCode = DicPlus_Command(m_hic, GET_HARDWARE_INFO, hardData);

//    if (errorCode != 0)
//    {
//        return -1;
//    }

//    for (int i = 0; i < 16; i++)
//    {
//        printf("%02X ", (BYTE)hardData[i]);
//    }

//    printf("\n");

//    char randomData[16];
//     errorCode = DicPlus_Set(randomData, RANDOM_SIZE, BY_VALUE, 16, NULL);	// 设置要取的随机数长度
//    if (errorCode != 0)
//    {
//        return -1;
//    }

//    errorCode = DicPlus_Command(m_hic, RANDOM, randomData);                     // 获取随机数
//    if (errorCode != 0)
//    {
//        return -1;
//    }

//    for (int i = 0; i < 16; i++)
//    {
//        printf("%02X ", (BYTE)randomData[i]);
//    }

//    printf("\n");

//    ZeroMemory(dataSrc, 128);
//    memcpy(dataSrc, hardData, 16);
//    memcpy(dataSrc+16, randomData, 16);

//    ZeroMemory(dataEnc, 128);

//    printf("source data\n");
//    for(int i = 0; i < 32; i++)
//    {
//        printf("%02x ",(BYTE)dataSrc[i]);
//        if((i+1)%16 == 0)
//            printf("\n");
//    }
//    printf("\n");

//    errorCode = DicPlus_DesEncrypt(m_hic, 0x1002, 1, 32, dataSrc, dataEnc);

//    printf("encrypted source data\n");
//    for(int i = 0; i < 32; i++)
//    {
//        printf("%02x ",(BYTE)dataEnc[i]);
//        if((i+1)%16 == 0)
//            printf("\n");
//    }
//    printf("\n");


//    WORD len = 0;

//    //run algorithm
//    errorCode = DicPlus_Run(m_hic, 0x6002, 32, dataEnc, &len, buffer);

//    if (errorCode != SCARD_S_SUCCESS)
//    {
//        printf("run algorithm failed : %8x \n", errorCode);
//    }

//    printf("returned data\n");
//    for(int i = 0; i < len; i++)
//    {
//        printf("%02x ",(BYTE)buffer[i]);
//        if((i+1)%16 == 0)
//            printf("\n");
//    }
//    printf("\n");

//    //call dongle to decrypt the data
//    errorCode = DicPlus_DesDecrypt(m_hic, 0x1002, 2, 32, buffer, buffer);

//    if (errorCode != SCARD_S_SUCCESS)
//    {
//        printf("des decrypt error : %8x \n",errorCode);
//    }
//    else
//    {
//        printf("des decrypted result:\n");
//        for(int i = 0; i < 32; i++)
//        {
//            printf("%02x ",(BYTE)buffer[i]);
//            if((i+1)%16 == 0)
//                printf("\n");
//        }
//        printf("\n");
//    }

//    DicPlus_Close(m_hic);

//    if (buffer[0] == 0x00)
//    {
//        printf("pass dongle verrify\n\n");
//        return 0;
//    }
//    else
//    {
//        printf("verrify dongle failed\n\n");
//        return -1;
//    }
//}
