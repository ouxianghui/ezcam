#include "RockeyArmOperator.h"
#include "Dongle_API.h"
#include <stdio.h>
#include <QTextCodec>
#include <QMessageBox>
#include <QDebug>

RockeyArmOperator& RockeyArmOperator::GetInstance()
{
    static RockeyArmOperator op;
    return op;
}

RockeyArmOperator::RockeyArmOperator()
{

}

RockeyArmOperator::~RockeyArmOperator()
{
}

int RockeyArmOperator::verrifyDongle()
{
    DWORD dwRet = 0;
    int nCount = 0;

    //默认用户PIN码
    char userPin [] = "0x150621";
    int nRemainCount = 0;

    DONGLE_HANDLE hDongle = NULL;

    BYTE buffer[128];
    memset(buffer, 0, 128);

    BYTE tmpbuf[128];
    memset(tmpbuf, 0, 128);

    QTextCodec *tc = QTextCodec::codecForName("GBK");

    //枚举锁
    dwRet = Dongle_Enum(NULL, &nCount);
    if (DONGLE_SUCCESS != dwRet)
    {
        qDebug("Enum %d Dongle ARM. \n", nCount);
        return -1;
    }

    if (nCount <= 0)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("找不到可用的加密锁，请插入加密锁"), tc->toUnicode("知道了"));
        return -1;
    }

    //打开锁
    dwRet = Dongle_Open(&hDongle, 0);
    if (DONGLE_SUCCESS != dwRet)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("打开加密锁失败"), tc->toUnicode("知道了"));
        qDebug("Open Dongle ARM. Return : 0x%08X . \n", dwRet);
        return -1;
    }

    //获取一个128字节的随机数作为密钥
    dwRet = Dongle_GenRandom(hDongle, 128, tmpbuf);
    if (DONGLE_SUCCESS != dwRet)
    {
        return -1;
    }

    //验证开发商PIN码
    dwRet = Dongle_VerifyPIN(hDongle, FLAG_USERPIN, userPin, &nRemainCount);
    if (DONGLE_SUCCESS != dwRet)
    {
        QMessageBox::critical(NULL, tc->toUnicode("错误"), tc->toUnicode("加密锁认证失败"), tc->toUnicode("知道了"));
        qDebug("Verify User PIN. Return: 0x%08X\n", dwRet);
        return -1;
    }

    //3DES加密
    memcpy(buffer, tmpbuf, 128);
    dwRet = Dongle_TDES(hDongle, 0x0622, FLAG_ENCODE, buffer, buffer, 128);
    if (DONGLE_SUCCESS != dwRet)
    {
        qDebug("3DES encode. Return: 0x%08X\n", dwRet);
        return -1;
    }

    //运行可执行文件
    int nMainRet = 0;
    WORD wInOufBufLen = 128;
    //memset(buffer, 0, sizeof(buffer));
    dwRet = Dongle_RunExeFile(hDongle, 0x0621, buffer, wInOufBufLen, &nMainRet);
    if (DONGLE_SUCCESS != dwRet)
    {
        qDebug("Run Exe file. Return: 0x%08X\n", dwRet);
        return -1;
    }

    //SM4解密
    dwRet = Dongle_SM4(hDongle, 0x0623, FLAG_DECODE, buffer, buffer, 128);
    if (DONGLE_SUCCESS != dwRet)
    {
        qDebug("SM4 decode. Return: 0x%08X\n", dwRet);
        return -1;
    }

    if (memcmp(tmpbuf, buffer, 128) != 0)
    {
        qDebug("SM4 decode result is wrong. \n");
        return -1;
    }

    //关闭加密锁
    dwRet = Dongle_Close(hDongle);
    if (DONGLE_SUCCESS != dwRet)
    {
        qDebug("Close Dongle ARM. Return: 0x%08X\n", dwRet);
        return -1;
    }

    return 0;
}

