/*
  ARM 加密锁 API 接口库
*/

#ifndef  __DONGLE_HEADER_H
#define  __DONGLE_HEADER_H

#include "winscard.h"
#pragma comment (lib, "winscard.lib")

#ifdef __cplusplus
extern "C" {
#endif

#ifndef         IN
    #define     IN
#endif

#ifndef         OUT
    #define     OUT
#endif

/************************************************************************/
/*                                  定义                                */
/************************************************************************/
//加密锁句柄定义
typedef void * DONGLE_HANDLE;

//默认的PIN码重试次数为无限制
//根据种子码初始化锁时会同时初始化PID和ADMINPIN (PID不可更改, ADMINPIN可更改)


#define CONST_PID                  0xFFFFFFFF   //出厂时默认的PID
#define CONST_USERPIN              "12345678"   //出厂时默认的USERPIN
#define CONST_ADMINPIN             "FFFFFFFFFFFFFFFF" //出厂时默认的ADMINPIN

//通讯协议类型定义
#define PROTOCOL_HID               0 //hid协议
#define PROTOCOL_CCID              1 //ccid协议

//文件类型定义
#define FILE_DATA                  1 //普通数据文件
#define FILE_PRIKEY_RSA            2 //RSA私钥文件
#define FILE_PRIKEY_ECCSM2         3 //ECC或者SM2私钥文件(SM2私钥文件和ECC私钥文件结构相同，属相同文件类型)
#define FILE_KEY                   4 //SM4和3DES密钥文件
#define FILE_EXE                   5 //可执行文件

//LED灯状态定义
#define LED_OFF			           0 //灯灭
#define LED_ON			           1 //灯亮
#define LED_BLINK		           2 //灯闪

//PIN码类型
#define FLAG_USERPIN   			   0 //用户PIN
#define FLAG_ADMINPIN			   1 //开发商PIN

//加解密标志
#define FLAG_ENCODE                0 //加密
#define FLAG_DECODE                1 //解密

//HASH算法类型
#define FLAG_HASH_MD5			   0 //MD5     运算结果16字节
#define FLAG_HASH_SHA1             1 //SHA1    运算结果20字节
#define FLAG_HASH_SM3              2 //SM3     运算结果32字节

//远程升级的功能号
#define UPDATE_FUNC_CreateFile     1 //创建文件
#define UPDATE_FUNC_WriteFile      2 //写文件
#define UPDATE_FUNC_DeleteFile     3 //删除文件
#define UPDATE_FUNC_FileLic        4 //设置文件授权
#define UPDATE_FUNC_SeedCount      5 //设置种子码可运算次数
#define UPDATE_FUNC_DownloadExe    6 //升级可执行文件
#define UPDATE_FUNC_UnlockUserPin  7 //解锁用户PIN
#define UPDATE_FUNC_Deadline       8 //时钟锁升级使用期限

/************************************************************************/
/*                              结构                                    */
/************************************************************************/
//RSA公钥格式(兼容1024,2048)
typedef struct {
	unsigned int  bits;                   // length in bits of modulus
	unsigned int  modulus;				  // modulus
	unsigned char exponent[256];          // public exponent

} RSA_PUBLIC_KEY;

//RSA私钥格式(兼容1024,2048)
typedef struct {
	unsigned int  bits;                   // length in bits of modulus
	unsigned int  modulus;                // modulus
	unsigned char publicExponent[256];    // public exponent
	unsigned char exponent[256];          // private exponent

} RSA_PRIVATE_KEY;

//外部ECCSM2公钥格式 ECC(支持bits为192或256)和SM2的(bits为固定值0x8100)公钥格式
typedef struct{
	unsigned int bits;                    // length in bits of modulus
	unsigned int XCoordinate[8];          // 曲线上点的X坐标
	unsigned int YCoordinate[8];          // 曲线上点的Y坐标

} ECCSM2_PUBLIC_KEY;

//外部ECCSM2私钥格式 ECC(支持bits为192或256)和SM2的(bits为固定值0x8100)私钥格式
typedef struct{
	unsigned int bits;                    // length in bits of modulus
	unsigned int PrivateKey[8];           // 私钥

} ECCSM2_PRIVATE_KEY;

//加密锁信息
typedef struct
{
	unsigned short  m_Ver;               //COS版本,比如:0x0201,表示2.01版 
	unsigned short  m_Type;              //产品类型: 0xFF表示标准版, 0x00为标准时钟锁,0x02为标准U盘锁  
	unsigned char   m_BirthDay[8];       //出厂日期 
	unsigned long   m_Agent;             //代理商编号,比如:默认的0xFFFFFFFF
	unsigned long   m_PID;               //产品ID
	unsigned long   m_UserID;            //用户ID
	unsigned char   m_HID[8];            //8字节的硬件ID
	unsigned long   m_IsMother;          //母锁标志: 0x01表示是母锁, 0x00表示不是母锁
    unsigned long   m_DevType;           //设备类型(PROTOCOL_HID或者PROTOCOL_CCID)

} DONGLE_INFO;

/**   
 *   锁内文件说明
 *   1.RSA私钥文件允许创建的最大数量为8个
 *   2.ECCSM2私钥文件允许创建的最大数量为16个
 *   3.3DES/SM4密钥文件允许创建的最大数量为32个
 *   4.可执行文件允许创建的最大数量为64个,总大小不能超过64K
 *   5.数据文件创建个数受锁内空间大小和文件系统其他因素的影响，最大个数不超过54个。
 *   6.文件ID取值范围为0x0000~0xFFFF之间，其中ID：0x0000、0xFFFF、0x3F00被锁内系统占用，用户不能使用。
 */

/*************************文件授权结构***********************************/
//数据文件授权结构
typedef struct
{
	unsigned short  m_Read_Priv;     //读权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限
	unsigned short  m_Write_Priv;    //写权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限

} DATA_LIC;

//私钥文件授权结构
typedef struct
{
	long           m_Count;        //可调次数: 0xFFFFFFFF表示不限制, 递减到0表示已不可调用
	unsigned char  m_Priv;         //调用权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限
	unsigned char  m_IsDecOnRAM;   //是否是在内存中递减: 1为在内存中递减，0为在FLASH中递减
	unsigned char  m_IsReset;      //用户态调用后是否自动回到匿名态: TRUE为调后回到匿名态 (开发商态不受此限制)
	unsigned char  m_Reserve;      //保留,用于4字节对齐

} PRIKEY_LIC;

//对称加密算法(SM4/TDES)密钥文件授权结构
typedef struct
{
	unsigned long  m_Priv_Enc;   //加密时的调用权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限

} KEY_LIC;

//可执行文件授权结构
typedef struct
{
	unsigned short  m_Priv_Exe;   //运行的权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限
	
} EXE_LIC;

/****************************文件属性结构********************************/
//数据文件属性数据结构
typedef struct
{
	unsigned long    m_Size;      //数据文件长度，该值最大为4096
	DATA_LIC         m_Lic;       //授权
	
} DATA_FILE_ATTR;

//ECCSM2/RSA私钥文件属性数据结构
typedef struct
{
	unsigned short  m_Type;       //数据类型:ECCSM2私钥 或 RSA私钥
	unsigned short  m_Size;       //数据长度:RSA该值为1024或2048, ECC该值为192或256, SM2该值为0x8100
	PRIKEY_LIC      m_Lic;        //授权

} PRIKEY_FILE_ATTR;

//对称加密算法(SM4/TDES)密钥文件属性数据结构
typedef struct
{
	unsigned long  m_Size;       //密钥数据长度=16
	KEY_LIC        m_Lic;        //授权

} KEY_FILE_ATTR;

//可执行文件属性数据结构
typedef struct
{
	EXE_LIC         m_Lic;        //授权	
	unsigned short  m_Len;        //文件长度

} EXE_FILE_ATTR;

/*************************文件列表结构***********************************/
//获取私钥文件列表时返回的数据结构
typedef struct
{
	unsigned short    m_FILEID;  //文件ID
	unsigned short    m_Reserve; //保留,用于4字节对齐
    PRIKEY_FILE_ATTR  m_attr;    //文件属性

}PRIKEY_FILE_LIST;

//获取SM4及TDES密钥文件列表时返回的数据结构
typedef struct
{
	unsigned short    m_FILEID;  //文件ID
	unsigned short    m_Reserve; //保留,用于4字节对齐
    KEY_FILE_ATTR     m_attr;    //文件属性

}KEY_FILE_LIST;

//获取数据文件列表时返回的数据结构
typedef struct
{
    unsigned short    m_FILEID;  //文件ID
	unsigned short    m_Reserve; //保留,用于4字节对齐
	DATA_FILE_ATTR    m_attr;    //文件属性

}DATA_FILE_LIST;

//获取可执行文件列表时返回的数据结构
typedef struct
{
	unsigned short  m_FILEID;    //文件ID
	EXE_FILE_ATTR   m_attr;
	unsigned short  m_Reserve;  //保留,用于4字节对齐
	
}EXE_FILE_LIST;

//下载和列可执行文件时填充的数据结构
typedef struct
{    
	unsigned short  m_dwSize;           //可执行文件大小
	unsigned short  m_wFileID;          //可执行文件ID
	unsigned char   m_Priv;             //调用权限: 0为最小匿名权限，1为最小用户权限，2为最小开发商权限
	unsigned char  *m_pData;            //可执行文件数据

}EXE_FILE_INFO;

//需要发给空锁的初始化数据
typedef struct
{
	int    m_SeedLen;                 //种子码长度
	BYTE   m_SeedForPID[256];         //产生产品ID和开发商密码的种子码 (最长250个字节)
	char   m_UserPIN[18];             //用户密码(16个字符的0终止字符串)
	BYTE   m_UserTryCount;            //用户密码允许的最大错误重试次数
	BYTE   m_AdminTryCount;           //开发商密码允许的最大错误重试次数
	RSA_PRIVATE_KEY m_UpdatePriKey;   //远程升级私钥
	DWORD  m_UserID_Start;            //起始用户ID

} SON_DATA;

//母锁数据
typedef struct
{
	SON_DATA  m_Son;                  //子锁初始化数据
	long      m_Count;                //可产生子锁初始化数据的次数 (-1表示不限制次数, 递减到0时会受限)

} MOTHER_DATA;

/**************************************************************************/
/*                                      API                               */
/**************************************************************************/

/** 
 * @brief  枚举加密锁。本函数最多会枚举出32个hid设备和32个ccid设备。
 *
 * @param  pDongleInfo     [out]     设备信息的数组。当此参数为NULL时, pCount返回找到的设备的数目。
 * @param  pCount 	       [out]     设备数目。该函数最多可以同时枚举出32个HID设备和32个CCID设备。
 *
 * @return DONGLE_SUCCESS            执行成功。
 */
DWORD WINAPI Dongle_Enum(DONGLE_INFO * pDongleInfo, int * pCount);

/**
 * @brief  打开指定的加密锁。
 *
 * @param  phDongle     [out]     句柄指针。如果打开成功,会被填充。
 * @param  nIndex       [in]      基于0的索引值。指示打开找到的第几把加密锁。
 *
 * @return DONGLE_SUCCESS         打开成功。
 */
DWORD WINAPI Dongle_Open(DONGLE_HANDLE * phDongle, int nIndex);

/**
 * @brief  清除PIN码验证状态。将加密锁状态变为匿名。
 *
 * @param  hDongle     [in]     打开的加密锁句柄。
 *
 * @return DONGLE_SUCCESS       执行成功。
 */
DWORD WINAPI Dongle_ResetState(DONGLE_HANDLE hDongle);

/**
 * @brief  关闭打开的加密锁。
 *
 * @param  hDongle     [in]     打开的加密锁句柄。
 *
 * @return DONGLE_SUCCESS       关闭成功。
 */
DWORD WINAPI Dongle_Close(DONGLE_HANDLE hDongle);

/**
 * @brief  产生随机数。匿名权限即可操作。
 *
 * @param  hDongle          [in]      打开的加密锁句柄。
 * @param  nLen             [in]      要产生的随机数的长度。nLen的取值范围为 1~128。
 * @param  pRandom          [out]     随机数缓冲区。
 *
 * @return DONGLE_SUCCESS             获取随机数成功。
 */
DWORD WINAPI Dongle_GenRandom(DONGLE_HANDLE hDongle, int nLen, BYTE * pRandom);

/**
 * @brief  LED灯的控制操作。匿名权限即可操作。
 *
 * @param  hDongle     [in]     打开的加密锁句柄。
 * @param  nFlag       [in]     控制类型。例如：nFlag = LED_ON，表示控制LED为亮的状态；
 *                              nFlag = LED_OFF，表示控制LED为灭的状态；nFlag = LED_BLINK，
 *                              表示控制LED为闪烁的状态。
 *
 * @return DONGLE_SUCCESS       命令执行成功。
 */
DWORD WINAPI Dongle_LEDControl(DONGLE_HANDLE hDongle, int nFlag);

/**
 * @brief  切换通讯协议。调用执行成功后加密锁会自动重启,打开的句柄hDongle会无效,接下来执行关闭操作会返回
 *         0xF0000002的错误码，这属正常。如需继续操作，请重新枚举并打开锁。该操作必须要验证开发商PIN码之
 *	       后方可使用。
 *
 * @param  hDongle       [in]   打开的加密锁句柄。
 * @param  nFlag         [in]   协议类型。例如：nFlag值为PROTOCOL_HID，表示将加密锁切换为HID设备；
 *                              nFlag值为PROTOCOL_CCID，表示将加密锁切换为CCID设备
 *
 * @return DONGLE_SUCCESS       执行成功。
 */
DWORD WINAPI Dongle_SwitchProtocol(DONGLE_HANDLE hDongle, int nFlag);

/**
 * @brief  一键恢复。即返回出厂状态，加密锁的PID、用户PIN码、开发商PIN码等，全部恢复到出厂状态所有写入的
 *       数据都将被清空。另外，调用执行成功后加密锁会自动重启,打开的句柄hDongle会无效,接下来执行关闭操
 *       做会返回0xF0000002的错误码，这属正常。如需继续操作，请重新枚举并打开锁。该操作需要开发商权限。
 *
 * @param  hDongle       [in]     打开的加密锁句柄。
 *
 * @return DONGLE_SUCCESS         执行成功。
 */
DWORD WINAPI Dongle_RFS(DONGLE_HANDLE hDongle);

/**
 * @brief  创建文件。该函数不支持可执行文件的创建。该操作需要开发商权限。
 *
 * @param  hDongle       [in]     打开的加密锁句柄。
 * @param  nFileType     [in]     文件类型。
 *                                nFileType = FILE_DATA，表示创建数据文件；对数据文件有以下说明：
 *                                   1.文件大小设为252字节时,最多可创建54个文件,即占用空间13608字节
 *                                   2.文件大小设为1024字节时,最多可创建31个文件，即占用空间31744字节
 *                                   3.文件大小设为4096字节时，最多可创建9个文件,即占用空间36864字节
 *                                nFileType = FILE_PRIKEY_RSA，表示创建RSA私钥文件；
 *                                nFileType = FILE_PRIKEY_ECCSM2，表示创建ECCSM2私钥文件；

 *                                nFileType = FILE_KEY，表示创建SM4和3DES密钥文件；
 *                                不支持nFileType = FILE_EXE的文件类型。
 * @param  wFileID       [in]     文件ID。
 * @param  pFileAttr     [in]     文件的属性。参数的结构为：DATA_FILE_ATTR、PRIKEY_FILE_ATTR或KEY_FILE_ATTR。
 *
 * @return DONGLE_SUCCESS         创建文件成功。  
 */
DWORD WINAPI Dongle_CreateFile(DONGLE_HANDLE hDongle, int nFileType, WORD wFileID, void * pFileAttr);

/**
 * @brief  写文件。该函数不支持可执行文件的写入操作，且该操作需要开发商权限。
 *
 * @param  hDongle       [in]     打开的加密锁句柄。
 * @param  nFileType     [in]     文件类型。例如，
 *                                nFileType = FILE_DATA，表示创建数据文件；
 *                                nFileType = FILE_PRIKEY_RSA，表示创建RSA私钥文件；
 *                                nFileType = FILE_PRIKEY_ECCSM2，表示创建ECCSM2私钥文件；
 *                                nFileType = FILE_KEY，表示创建SM4和3DES密钥文件；
 *                                不支持nFileType = FILE_EXE的文件类型。
 * @param  wFileID       [in]     文件ID。
 * @param  wOffset       [in]     文件偏移。文件写入的起始偏移量。
 * @param  pInData       [in]     准备写入的数据。
 * @param  nDataLen      [in]     参数pInData的大小。
 *
 * @return DONGLE_SUCCESS         写入文件成功。
 */
DWORD WINAPI Dongle_WriteFile(DONGLE_HANDLE hDongle, int nFileType, WORD wFileID, WORD wOffset, BYTE * pInData, int nDataLen);

/**
 * @brief  读取加密锁内的数据文件。数据文件的读取权限取决于创建时的设定。
 *
 * @param  hDongle      [in]         打开的加密锁句柄。
 * @param  wFileID      [in]         文件ID。
 * @param  wOffset      [in]         文件偏移量。
 * @param  pOutData     [in]         数据缓冲区。
 * @param  nDataLen     [out]        参数pOutData的长度。
 *
 * @return DONGLE_SUCCESS            读取数据文件成功  
 */
DWORD WINAPI Dongle_ReadFile(DONGLE_HANDLE hDongle, WORD wFileID, WORD wOffset, BYTE * pOutData, int nDataLen);

/**
 * @brief  批量下载可执行文件。锁内可执行文件的数量不能超过64个，可执行文件的总大小不能超过64K，
 *         该操作需要验证管理员权限
 *
 * @param  hDongle          [in]     打开的加密锁句柄。
 * @param  pExeFileInfo     [in]     可执行文件信息的数组。
 * @param  nCount           [in]     即可执行文件的数量。
 * 
 * @return DONGLE_SUCCESS            批量下载可执行文件成功。
 */
DWORD WINAPI Dongle_DownloadExeFile(DONGLE_HANDLE hDongle, EXE_FILE_INFO * pExeFileInfo, int nCount);

/**
 * @brief  运行指定的锁内可执行程序。运行可执行文件的权限，取决于批量下载时，每个可执行文件的设置，
 *         即，EXE_FILE_INFO中的m_Priv参数。输入输出数据的最大长度不能超过1024字节，输入输出数据缓
 *         冲区pInOutBuf对应锁内的InOutBuf。
 *
 * @param  hDongle            [in]             打开的加密锁句柄。
 * @param  wFileID            [in]             可执行文件的文件ID。
 * @param  pInOutBuf          [in,out]         输入输出数据缓冲区。
 * @param  wInOutBufLen       [in]             输入输出数据缓冲区pInOutBuf的大小。                                           
 * @param  pMainRet           [out]            锁内可执行程序main函数的返回值，可以为NULL。
 *
 * @return DONGLE_SUCCESS                  运行指定的可执行文件成功。
 */
DWORD WINAPI Dongle_RunExeFile(DONGLE_HANDLE hDongle, WORD wFileID, BYTE * pInOutBuf, WORD wInOutBufLen, int * pMainRet);

/**
 * @brief  删除文件。需要开发商权限。
 * 
 * @param  hDongle       [in]      打开的加密锁句柄。
 * @param  nFileType     [in]      文件类型。
 * @param  wFileID       [in]      文件ID。
 *
 * @return DONGLE_SUCCESS          删除文件成功 
 */
DWORD WINAPI Dongle_DeleteFile(DONGLE_HANDLE hDongle, int nFileType, WORD wFileID);

/**
 * @brief 列文件。需要开发商权限。
 *
 * @param  hDongle       [in]         打开的加密锁句柄。
 * @param  nFileType     [in]         指示文件类型。例如，FILE_DATA等。
 * @param  pFileList     [in]         pList：输出文件的列表 (此参数为NULL时, pLen中返回所需的缓冲区长度)
 *                                    当nFileType = FILE_DATA时,       pFileList指向DATA_FILE_LIST结构；
 *                                    当nFileType = FILE_PRIKEY_RSA时, pFileList指向PRIKEY_FILE_LIST结构；
 *                                    当nFileType = FILE_PRIKEY_ECCSM2时, pFileList指向PRIKEY_FILE_LIST结构
 *                                    当nFileType = FILE_KEY时,        pFileList指向KEY_FILE_LIST结构；
 *	                                  当nFileType = FILE_EXE时,        pFileList指向EXE_FILE_LIST结构。
 * @param  pDataLen      [in,out]     参数pFileList的输入长度，执行成功返回pFileList的字节长度。
 *
 * @return DONGLE_SUCCESS             列文件成功。
 */
DWORD WINAPI Dongle_ListFile(DONGLE_HANDLE hDongle, int nFileType, void* pFileList, int * pDataLen);

/**
 * @brief 唯一化锁。输入种子码产生PID和开发商PIN码，需要开发商权限，执行成功后加密锁自动回到匿名态。
 *		  产生开发商PIN码的目的是为了使密码肯定不是弱密码，产生的开发商PIN可以借助Dongle_ChangePIN
 *        进行更改。另外，种子码一定要牢记，否则任何人永远无法得知开发商PIN码。
 * 
 * @param hDongle		   [in]       打开的加密锁句柄。
 * @param nSeedLen		   [in]       参数pSeed的缓冲区长度。
 * @param pSeed            [in]       种子码的缓冲区。
 * @param pPIDStr          [out]      函数执行成功返回PID。该缓冲区大小至少应该为8字节，返回一个
 *                                    8字节的以0终止的ansi字符串。
 * @param pAdminPINstr     [out]      函数执行成功返回开发商PIN码。该缓冲区大小至少应该为16字节，
 *                                    返回字符串长度为16字节的以0终止的ansi字符串。
 *
 * @return DONGLE_SUCCESS             唯一化锁成功。
 */
DWORD WINAPI Dongle_GenUniqueKey(DONGLE_HANDLE hDongle,int nSeedLen, BYTE * pSeed, char * pPIDstr, char * pAdminPINstr);

/**
 * @brief  校验密码
 *
 * @param  hDongle		    [in]       打开的加密锁句柄。
 * @param  nFlags           [in]       PIN码类型。参数取值为FLAG_USERPIN或者FLAG_ADMINPIN。
 * @param  pPIN             [in]       PIN码，0终止的ansi字符串。
 * @param  pRemainCount     [out]      剩余重试次数。返回0表示已锁死；1~253表示剩余次数；255表示不限制重试次数。
 *
 * @return DONGLE_SUCCESS              校验成功。如果校验失败，函数的返回值中也含有剩余的重试次数，
 *                                     (错误码 & 0xFFFFFF00) == DONGLE_INCORRECT_PIN，即后两位表示剩余次数。
 */
DWORD WINAPI Dongle_VerifyPIN(DONGLE_HANDLE hDongle, int nFlags, char * pPIN, int * pRemainCount);

/**
 * @brief  更改密码
 * 
 * @param  hDongle       [in]     打开的加密锁句柄。
 * @param  nFlags        [in]     PIN码类型。参数取值为FLAG_USERPIN或者FLAG_ADMINPIN。
 * @param  pOldPIN       [in]     旧的PIN码缓冲区。必须是一个字符串长度为16字节的0终止的ansi字符串,且可以是中文。
 * @param  pNewPIN       [in]     新的PIN码缓冲区。必须是一个字符串长度为16字节的0终止的ansi字符串。
 * @param  nTryCount     [in]     重试次数。该参数的取值范围为1~255,其中255表示不限制重试次数。
 *
 * @return DONGLE_SUCCESS         修改密码成功。
 */
DWORD WINAPI Dongle_ChangePIN(DONGLE_HANDLE hDongle, int nFlags, char * pOldPIN, char * pNewPIN, int nTryCount);

/**
 * @brief  重置用户PIN码。空锁(即PID=FFFFFFFF)不能重置密码。执行成功后用户密码恢复为出厂默认 CONST_USERPIN
 *
 * @param  hDongle       [in]     打开的加密锁句柄。
 * @param  pAdminPIN     [in]     开发商PIN码缓冲区。长度为16字节的0终止的ansi字符串
 *
 * @return DONGLE_SUCCESS         重置用户PIN码成功。
 */
DWORD WINAPI Dongle_ResetUserPIN(DONGLE_HANDLE hDongle, char * pAdminPIN);

/**
 * @brief  设置用户ID。需要开发商权限。
 *
 * @param  hDongle      [in]     打开的加密锁句柄。
 * @param  dwUserID     [in]     用户ID。
 *
 * @return DONGLE_SUCCESS        重置用户PIN码成功。
 */
DWORD WINAPI Dongle_SetUserID(DONGLE_HANDLE hDongle, DWORD dwUserID);

/** 
 * @brief  获取加密锁到期时间。匿名权限获取。
 *
 * @param  hDongle     [in]      打开的加密锁句柄。
 * @param  pdwTime     [out]     获取的到期UTC时间值。
 *                               若*pdwTime = 0XFFFFFFFF，表示不限制到期时间
 *                               若(*pdwTime & 0XFFFF0000) == 0，值表示还剩余几小时
 *                               若(*pdwTime & 0XFFFF0000) != 0，值表示到期的UTC的时间，可以通过gmtime等将该值进行转换。
 *
 * @return DONGLE_SUCCESS        获取加密锁到期时间成功。
 */
DWORD WINAPI Dongle_GetDeadline(DONGLE_HANDLE hDongle, DWORD * pdwTime);

/**
 * @brief  设置加密锁的到期时间。该操作需要管理员权限。
 * 
 * @param  hDongle     [in]     打开的加密锁句柄。                              
 * @param  dwTime      [in]     时间值。说明：
 *                              1.设置可用小时数，范围在1~65535，例如dwTime = 24。这种情况在校验了用户PIN码后开始计时。
 *                              2.设置到期的年与日时分秒。可通过函数time或者mktime 取得即时的utc时间值(utc值都大于65535)；
 *                              3.取消到期时间限制，此时dwTime的值只能为0xFFFFFFFF。
 *
 * @return DONGLE_SUCCESS       设置加密锁到期时间成功。
 */
DWORD WINAPI Dongle_SetDeadline(DONGLE_HANDLE hDongle, DWORD dwTime);

/**
 * @brief  获取加密锁的UTC时间。
 * 
 * @param  hDongle         [in]      打开的加密锁句柄。
 * @param  pdwUTCTime      [out]     UTC时间值指针。
 *
 * @return DONGLE_SUCCESS       设置加密锁到期时间成功。
 */
DWORD WINAPI Dongle_GetUTCTime(DONGLE_HANDLE hDongle, DWORD * pdwUTCTime);

/**
 * @brief  读取锁内数据区数据。数据区大小共8k，前4k(0~4095)的读写没有权限限制，后4k(4096~8191)任意权限可读，
 *         但是只有开发商权限可写。
 * 
 * @param  hDongle      [in]      打开的加密锁句柄。
 * @param  nOffset      [in]      起始偏移。范围在0~8191
 * @param  pData        [out]     读取的数据缓冲区。
 * @param  nDataLen     [in]      参数pData的缓冲区大小。
 *
 * @return  DONGLE_SUCCESS        读取数据区数据成功。 
 */
DWORD WINAPI Dongle_ReadData(DONGLE_HANDLE hDongle, int nOffset, BYTE* pData, int nDataLen);

/**
 * @brief  写入锁内数据区数据。数据区大小共8k，前4k(0~4095)的读写没有权限限制，后4k(4096~8191)任意权限可读，
 *         但是只有开发商权限可写。
 * 
 * @param  hDongle      [in]      打开的加密锁句柄。
 * @param  nOffset      [in]      起始偏移。范围在0~8191
 * @param  pData        [in]      写入的数据缓冲区。
 * @param  nDataLen     [in]      参数pData的缓冲区大小。
 *
 * @return  DONGLE_SUCCESS        写入数据区数据成功。 
 */
DWORD WINAPI Dongle_WriteData(DONGLE_HANDLE hDongle, int nOffset, BYTE * pData, int nDataLen);

/**
 * @brief  获取共享内存数据。共享内存总大小为32字节。没有权限限制，掉电后数据自动擦除。
 *
 * @param  hDongle     [in]      打开的加密锁句柄。
 * @param  pData       [out]     输出的数据。输出共享内存的数据，固定为32个字节。
 *
 * @return  DONGLE_SUCCESS       获取共享内存数据成功。
 */
DWORD WINAPI Dongle_ReadShareMemory(DONGLE_HANDLE hDongle, BYTE * pData);

/**
 * @brief  设置共享内存数据。没有权限限制，掉电后数据自动擦除。
 * 
 * @param  hDongle      [in]     打开的加密锁句柄。
 * @param  pData        [in]     输入数据。
 * @param  nDataLen     [in]     参数pData的缓冲区大小。长度不能超过32。
 *
 * @return  DONGLE_SUCCESS       设置共享内存数据成功。
 */
DWORD WINAPI Dongle_WriteShareMemory(DONGLE_HANDLE hDongle, BYTE * pData, int nDataLen);

/**
 * @brief  产生RSA公钥和私钥。使用该函数之前需要先创建一个RSA私钥文件。需要开发商权限。成功后注意保存RSA公私钥数据。
 *
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  wPriFileID     [in]      RSA私钥文件ID。
 * @param  pPubBakup      [out]     RSA公钥数据。
 * @param  pPriBakup      [out]     RSA私钥数据。
 *
 * @return DONGLE_SUCCESS           产生RSA公私钥成功。 
 */
DWORD WINAPI Dongle_RsaGenPubPriKey(DONGLE_HANDLE hDongle, WORD wPriFileID, RSA_PUBLIC_KEY * pPubBakup, RSA_PRIVATE_KEY * pPriBakup);

/**
 * @brief  RSA私钥运算。函数的使用权限取决于锁内RSA私钥文件的权限，在RSA私钥文件创建时设定。说明：
 *         1.对于加密运算,输入数据长度必须小于私钥ID为wPriFileID的密钥长度除以8再减去11,以便在函数内部进行padding
 *         2.对于解密运算,输入数据长度必须与wPriFileID中指示的密钥长度相一致(比如1024位密钥时为128，2048时为256)
 *         3.加密时内部padding方式为:PKCS1_TYPE_1 (即第二个字节为0x01,空数据填充0XFF)   
 *
 * @param  hDongle         [in]         打开的加密锁句柄。
 * @param  wPriFileID      [in]         RSA私钥文件ID。
 * @param  nFlag           [in]         运算类型。例如，FLAG_ENCODE表示加密运算；FLAG_DECODE表示解密运算。
 * @param  pInData         [in]         输入数据。
 * @param  nInDataLen      [in]         参数pInData的大小
 * @param  pOutData        [out]        输出数据缓冲区。
 * @param  pOutDataLen     [in,out]     参数pOutData的大小和返回的数据大小。
 *
 * @return DONGLE_SUCCESS               RSA私钥运算成功。 
 */
DWORD WINAPI Dongle_RsaPri(DONGLE_HANDLE hDongle, WORD wPriFileID, int nFlag, BYTE * pInData, int nInDataLen, BYTE * pOutData, int * pOutDataLen);

/**
 * @brief  RSA公钥运算。匿名权限可调用。说明：
 *         1.对于加密运算,输入数据长度必须小于pPubKey中指示的密钥长度除以8再减去11,以便在函数内部进行padding
 *         2.对于解密运算,输入数据长度必须与pPubKey中指示的密钥长度相一致(比如1024位密钥时为128，2048时为256)
 *         3.加密时内部padding方式为:PKCS1_TYPE_2 (即第二个字节为0x02,空数据填充随机数)  
 *
 * @param  hDongle         [in]         打开的加密锁句柄。
 * @param  nFlag           [in]         运算类型。例如，FLAG_ENCODE表示加密运算；FLAG_DECODE表示解密运算。
 * @param  pPubKey         [in]         RSA公钥数据。该数据来源于生成RSA公私钥时的公钥数据。
 * @param  pInData         [in]         输入数据。
 * @param  nInDataLen      [in]         参数pInData的大小。
 * @param  pOutData        [out]        输出数据缓冲区。
 * @param  pOutDataLen     [in,out]     参数pOutData的大小和返回的数据大小。
 *
 * @return DONGLE_SUCCESS               RSA公钥运算成功。
 */
DWORD WINAPI Dongle_RsaPub(DONGLE_HANDLE hDongle, int nFlag, RSA_PUBLIC_KEY * pPubKey, BYTE * pInData, int nInDataLen, BYTE * pOutData, int * pOutDataLen);

/**
 * @brief  产生ECC公钥和私钥。使用该函数之前需要先创建一个ECC私钥文件。需要开发商权限。成功后注意保存ECC公私钥数据。
 *
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  wPriFileID     [in]      ECC私钥文件ID。
 * @param  pPubBakup      [out]     ECC公钥数据。
 * @param  pPriBakup      [out]     ECC私钥数据。
 *
 * @return DONGLE_SUCCESS           产生ECC公私钥成功。 
 */
DWORD WINAPI Dongle_EccGenPubPriKey(DONGLE_HANDLE hDongle, WORD wPriFileID, ECCSM2_PUBLIC_KEY * pPubBakup, ECCSM2_PRIVATE_KEY * pPriBakup);

/**
 * @brief  ECC私钥签名。函数的使用权限取决于锁内ECC私钥文件的权限，在ECC私钥文件创建时设定。说明：
 *         1.锁内签名算法为: ECDSA_Sign
 *         2.输入的Hash值的长度与ECC私钥的密钥长度有关(如果密钥是192位的,则hash值长度不能超过24(192/8 = 24)字节)
 *                                                    (如果密钥是256位的,则hash值长度不能超过32(256/8 = 32)字节)
 *         3.曲线参数为:EC_NIST_PRIME_192及EC_NIST_PRIME_256
 *
 * @param  hDongle          [in]      打开的加密锁句柄。
 * @param  wPriFileID       [in]      ECC私钥文件ID。
 * @param  pHashData        [in]      Hash数据。
 * @param  nHashDataLen     [in]      参数pHashData的大小。
 * @param  pOutData         [out]     签名数据。大小固定为64字节(256位ECC时是正好,192位ECC时的位会补0)
 *
 * @return DONGLE_SUCCESS             表示签名成功。
 */
DWORD WINAPI Dongle_EccSign(DONGLE_HANDLE hDongle, WORD wPriFileID, BYTE * pHashData, int nHashDataLen, BYTE * pOutData);

/**
 * @brief  ECC公钥验签。函数的使用权限取决于锁内ECC私钥文件的权限，在ECC私钥文件创建时设定。说明：
 *         1.锁内签名算法为: ECDSA_Verify
 *         2.输入的Hash值的长度与ECC私钥的密钥长度有关(如果密钥是192位的,则hash值长度不能超过24(192/8 = 24)字节)
 *                                                    (如果密钥是256位的,则hash值长度不能超过32(256/8 = 32)字节)
 *         3.曲线参数为:EC_NIST_PRIME_192及EC_NIST_PRIME_256
 *
 * @param  hDongle          [in]      打开的加密锁句柄。
 * @param  pPubKey          [in]      ECC公钥数据。
 * @param  pHashData        [in]      Hash数据。
 * @param  nHashDataLen     [in]      参数pHashData的大小。
 * @param  pSign            [in]      签名数据。大小固定为64字节，为Dongle_EccSign函数返回的pOutData数据。
 *
 * @return DONGLE_SUCCESS             表示验签成功,否则表示验签失败。
 */
DWORD WINAPI Dongle_EccVerify(DONGLE_HANDLE hDongle, ECCSM2_PUBLIC_KEY * pPubKey, BYTE * pHashData, int nHashDataLen, BYTE * pSign);

/**
 * @brief  产生SM2公钥和私钥。使用该函数之前需要先创建一个SM2私钥文件。需要开发商权限。成功后注意保存ECC公私钥数据。
 *
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  wPriFileID     [in]      SM2私钥文件ID。
 * @param  pPubBakup      [out]     SM2公钥数据。
 * @param  pPriBakup      [out]     SM2私钥数据。
 *
 * @return DONGLE_SUCCESS           产生ECC公私钥成功。 
 */
DWORD WINAPI Dongle_SM2GenPubPriKey(DONGLE_HANDLE hDongle, WORD wPriFileID, ECCSM2_PUBLIC_KEY * pPubBakup, ECCSM2_PRIVATE_KEY * pPriBakup);

/**
 * @brief  SM2私钥签名。函数的使用权限取决于锁内SM2私钥文件的权限，在SM2私钥文件创建时设定。
 *
 * @param  hDongle          [in]      打开的加密锁句柄。
 * @param  wPriFileID       [in]      SM2私钥文件ID。
 * @param  pHashData        [in]      Hash数据。
 * @param  nHashDataLen     [in]      参数pHashData的大小。数据长度必须小于32个字节。
 * @param  pOutData         [out]     签名数据。大小固定为64字节。
 *
 * @return DONGLE_SUCCESS             表示签名成功。
 */
DWORD WINAPI Dongle_SM2Sign(DONGLE_HANDLE hDongle, WORD wPriFileID, BYTE * pHashData, int nHashDataLen, BYTE * pOutData);

/**
 * @brief  SM2公钥验签。函数的使用权限取决于锁内SM2私钥文件的权限，在SM2私钥文件创建时设定。
 *
 * @param  hDongle          [in]      打开的加密锁句柄。
 * @param  wPriFileID       [in]      SM2公钥数据。
 * @param  pHashData        [in]      Hash数据。
 * @param  nHashDataLen     [in]      参数pHashData的大小。
 * @param  pSign            [in]      签名数据。大小固定为64字节，为Dongle_EccSign函数返回的pOutData数据。
 *
 * @return DONGLE_SUCCESS             表示验签成功,否则表示验签失败。
 */
DWORD WINAPI Dongle_SM2Verify(DONGLE_HANDLE hDongle, ECCSM2_PUBLIC_KEY * pPubKey, BYTE * pHashData, int nHashDataLen, BYTE * pSign);

/**
 * @brief  3DES加解密。解密运算匿名权限即可, 加密运算的权限取决于密钥文件的权限。
 * 
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  wKeyFileID     [in]      密钥文件ID。
 * @param  nFlag          [in]      运算类型。例如，FLAG_ENCODE表示加密运算；FLAG_DECODE表示解密运算。
 * @param  pInData        [in]      输入数据缓冲区。
 * @param  pOutData       [out]     输出数据缓冲区。大小至少要和输入数据缓冲区相同，输入和输出数据缓冲区可以为同一个。
 * @param  nDataLen       [in]      参数pInData的数据大小。数据长度必须是16的整数倍,允许的最大值是1024。
 *
 * @return DONGLE_SUCCESS           3DES加密或解密运算成功。
 */
DWORD WINAPI Dongle_TDES(DONGLE_HANDLE hDongle, WORD wKeyFileID, int nFlag, BYTE * pInData, BYTE * pOutData, int nDataLen);

/**
 * @brief  SM4加解密。解密运算匿名权限即可, 加密运算的权限取决于密钥文件的权限。
 * 
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  wKeyFileID     [in]      密钥文件ID。
 * @param  nFlag          [in]      运算类型。例如，FLAG_ENCODE表示加密运算；FLAG_DECODE表示解密运算。
 * @param  pInData        [in]      输入数据缓冲区。
 * @param  pOutData       [out]     输出数据缓冲区。大小至少要和输入数据缓冲区相同，输入和输出数据缓冲区可以为同一个。
 * @param  nDataLen       [in]      参数pInData的数据大小。数据长度必须是16的整数倍,允许的最大值是1024。
 *
 * @return DONGLE_SUCCESS           SM4加密或解密运算成功。
 */
DWORD WINAPI Dongle_SM4(DONGLE_HANDLE hDongle, WORD wKeyFileID, int nFlag, BYTE * pInData, BYTE * pOutData, int nDataLen);

/**
 * @brief  HASH运算。
 * 
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  nFlag          [in]      Hash运算算法类型。
 *                                  nFlag = FLAG_HASH_MD5，表示MD5运算，此时pHash的缓冲区大小为16字节。
 *                                  nFlag = FLAG_HASH_SHA1，表示SHA1运算，此时pHash的缓冲区大小为20字节。
 *                                  nFlag = FLAG_HASH_SM3，表示国密SM3运算，此时pHash的缓冲区大小为32字节。
 * @param  pInData        [in]      输入数据缓冲区。
 * @param  nDataLen       [in]      参数pInData的数据大小。SHA1、MD5为锁外运算，长度不限制；SM3为锁内运算，
 *                                  最大不超过1024字节。
 * @param  pHash          [out]     输出的Hash值。
 *
 * @return DONGLE_SUCCESS           HASH运算成功。
 */
DWORD WINAPI Dongle_HASH(DONGLE_HANDLE hDongle, int nFlag, BYTE * pInData, int nDataLen, BYTE * pHash);

/**
 * @brief  种子码算法。匿名权限可使用, 开发商可设置可运算次数。
 *         1.种子码算法与PID有关，空锁(即PID=FFFFFFFF)不能进行种子码运算。
 *         2.如果内部种子码可运算次数不为-1，当其递减到0后此函数将不能使用。
 *
 * @param  hDongle        [in]      打开的加密锁句柄。
 * @param  pSeed          [in]      输入的种子码数据。 
 * @param  nSeedLen       [in]      种子码长度。取值范围为1~250字节。
 * @param  pOutData       [out]     输出数据缓冲区。输出的大小固定为16字节。
 *
 * @return DONGLE_SUCCESS           种子码运算成功。
 */
DWORD WINAPI Dongle_Seed(DONGLE_HANDLE hDongle, BYTE * pSeed, int nSeedLen, BYTE * pOutData);

/**
 * @brief 设置种子码算法可运算次数。需要开发商权限。
 *
 * @param  hDongle     [in]      打开的加密锁句柄。
 * @param  nCount      [in]      可运算次数。如果此值设置为-1，表示不限制运算次数。
 * 
 * @return DONGLE_SUCCESS        设置成功。
 */
DWORD WINAPI Dongle_LimitSeedCount(DONGLE_HANDLE hDongle, int nCount);

/**
 * @brief  制作一把母锁。子母锁的方式是是一种可选的初始化锁方式，安全又快速，推荐使用。需要开发商权限。
 *         1.空锁(即PID=FFFFFFFF)不能写入母锁数据。
 *	       2.出于安全考虑,MOTHER_DATA中的远程升级私钥不允许和母锁自身的远程升级私钥相同,否则会操作失败。
 *
 * @param  hDongle     [in]      打开的加密锁句柄。
 * @param  pInData     [in]      输入数据。用于初始化母锁的结构为MOTHER_DATA的数据。
 *
 * @return DONGLE_SUCCESS        制作母锁成功。
 */
DWORD WINAPI Dongle_GenMotherKey(DONGLE_HANDLE hDongle, MOTHER_DATA * pInData);

/**
 * @brief  从空锁获取生产请求。此函数只对PID为FFFFFFFF的空锁有效。需要开发商权限。
 *
 * @param  hDongle      [in]       打开的加密锁句柄。
 * @param  pRequest     [out]      输出数据。返回该数据的有效长度为16字节，因此需要至少16字节的空间。
 *
 * @return DONGLE_SUCCESS          获取生产请求数据成功。
 */
DWORD WINAPI Dongle_RequestInit(DONGLE_HANDLE hDongle, BYTE * pRequest);

/**
 * @brief  从母锁获取用于初始化子锁的数据，该函数只对母锁有效。匿名权限可使用。
 *
 * @param  hDongle       [in]          打开的加密锁句柄。
 * @param  pRequest      [in]          请求数据。通过Dongle_RequestInit获取的请求数据。
 * @param  pInitData     [out]         输出数据。函数执行成功返回用于初始化子锁的数据。
 * @param  pDataLen      [int,out]     参数pInitData的有效长度。表示pInitData的缓冲区长度，函数执行成功
 *                                     返回pInitData的有效长度。
 *
 * @return DONGLE_SUCCESS              从母锁获取生产数据成功。
 */
DWORD WINAPI Dongle_GetInitDataFromMother(DONGLE_HANDLE hDongle, BYTE * pRequest, BYTE * pInitData, int * pDataLen);

/*
 * @brief  生产子锁。用子母锁的方式制作子锁，匿名权限即可调用。
 *
 * @param  hDongle       [in]         打开的加密锁句柄。
 * @param  pInitData     [in]         输入数据。函数Dongle_GetInitDataFromMother返回的用于初始化子锁的数据。
 * @param  nDataLen      [in]         参数pInitData数据缓冲区的有效长度。
 *
 * @return DONGLE_SUCCESS             生产子锁成功。
 */
DWORD WINAPI Dongle_InitSon(DONGLE_HANDLE hDongle, BYTE * pInitData, int nDataLen);

/**
 * @brief  向锁内设置远程升级私钥。私钥长度为1024的RSA私钥。需要开发商权限。
 *         出于安全考虑，如果锁是母锁的话，远程升级私钥不允许和母锁数据区中的子锁远程升级私钥相同，否则会操作失败。
 *
 * @param  hDongle       [in]          打开的加密锁句柄。
 * @param  pPriKey       [in]          RSA私钥。
 *
 * @return DONGLE_SUCCESS              设置远程升级私钥成功。
 */
DWORD WINAPI Dongle_SetUpdatePriKey(DONGLE_HANDLE hDongle, RSA_PRIVATE_KEY * pPriKey);

/**
 * @brief  制作远程升级数据包。匿名权限即可调用。
 *
 * @param  hDongle         [in]         打开的加密锁句柄。
 * @param  pHID            [in]         硬件序列号。如果不需要绑定该参数可以为NULL。
 * @param  nFunc           [in]         升级包类型。
 *                                      nFunc = UPDATE_FUNC_CreateFile，表示创建文件。
 *                                      nFunc = UPDATE_FUNC_WriteFile，写文件。只有锁内已有的文件才可升级写文件操作。
 *                                      nFunc = UPDATE_FUNC_DeleteFile，删除文件。
 *                                      nFunc = UPDATE_FUNC_FileLic，设置文件授权，不支持可执行文件授权升级。
 *                                      nFunc = UPDATE_FUNC_SeedCount，设置种子码可运算次数。
 *                                      nFunc = UPDATE_FUNC_DownloadExe，升级可执行文件。
 *                                      nFunc = UPDATE_FUNC_UnlockUserPin，解锁用户PIN。出于安全考虑解锁用户PIN码必须绑定HID，
 *                                      即pHID不能为空，只有这样才能升级成功。升级成功后用户PIN码恢复为"12345678"。
 *                                      nFunc = UPDATE_FUNC_Deadline，时钟锁升级使用期限。
 *
 * @param  nFileType       [in]         文件类型。升级有关文件操作时的文件类型。其他升级类型该参数无效。
 * @param  wFileID         [in]         文件ID。升级有关文件操作时的文件ID。其他升级类型该参数无效。
 *                                      当wFileID=0xFFFF时，表示升级锁内数据区的数据。
 * @param  nOffset         [in]         偏移量。升级有关文件操作时的文件偏移量。其他升级类型该参数无效。
 * @param  pBuffer         [in]         输入数据。
 *                                      当nFunc = UPDATE_FUNC_CreateFile时, pBuffer指向要文件的属性结构，例如KEY_FILE_ATTR。
 *                                      当nFunc = UPDATE_FUNC_WriteFile时, pBuffer指向要写入的数据。
 *                                      当nFunc = UPDATE_FUNC_FileLic时, pBuffer指向文件权限的数据结构，例如：DATA_LIC。
 *                                      当nFunc = UPDATE_FUNC_SeedCount时, pBuffer指向long值，表示种子码可运算次数。
 *                                      当nFunc = UPDATE_FUNC_DownloadExe时, pBuffer指向EXE_FILE_INFO结构，与Dongle_DownloadExeFile函数用法类似。
 *                                      当nFunc = UPDATE_FUNC_Deadline时, pBuffer指向DWORD值，表示到期的时间。
 * @param  nBufferLen      [in]         参数pBuffer的缓冲区大小。
 * @param  pUPubKey        [in]         制作升级包的RSA公钥。与设置到锁内的远程升级私钥相对应。该值无论何种升级类型都必须填写。
 * @param  pOutData        [out]        输出数据。制作的升级包数据。
 * @param  pOutDataLen     [in,out]     参数pOutData输入大小，返回升级包的有效长度。
 *
 * @return DONGLE_SUCCESS               制作升级包成功。
 */
DWORD WINAPI Dongle_MakeUpdatePacket(DONGLE_HANDLE hDongle, char * pHID, int nFunc, int nFileType, WORD wFileID, int nOffset, BYTE * pBuffer, int nBufferLen, RSA_PUBLIC_KEY * pUPubKey, BYTE * pOutData, int * pOutDataLen);

/**
 * @brief  制作远程升级数据包。该函数采用母锁方式制作，与Dongle_MakeUpdatePacket相比少了远程升级公钥，其他相同。匿名权限即可调用。
 *
 * @param  hDongle         [in]         打开的加密锁句柄。
 * @param  pHID            [in]         硬件序列号。如果不需要绑定该参数可以为NULL。
 * @param  nFunc           [in]         升级包类型。
 *                                      nFunc = UPDATE_FUNC_CreateFile，表示创建文件。
 *                                      nFunc = UPDATE_FUNC_WriteFile，写文件。只有锁内已有的文件才可升级写文件操作。
 *                                      nFunc = UPDATE_FUNC_DeleteFile，删除文件。
 *                                      nFunc = UPDATE_FUNC_FileLic，设置文件授权，不支持可执行文件授权升级。
 *                                      nFunc = UPDATE_FUNC_SeedCount，设置种子码可运算次数。
 *                                      nFunc = UPDATE_FUNC_DownloadExe，升级可执行文件。
 *                                      nFunc = UPDATE_FUNC_UnlockUserPin，解锁用户PIN。出于安全考虑解锁用户PIN码必须绑定HID，
 *                                      即pHID不能为空，只有这样才能升级成功。升级成功后用户PIN码恢复为"12345678"。
 *                                      nFunc = UPDATE_FUNC_Deadline，时钟锁升级使用期限。
 *
 * @param  nFileType       [in]         文件类型。升级有关文件操作时的文件类型。其他升级类型该参数无效。
 * @param  wFileID         [in]         文件ID。升级有关文件操作时的文件ID。其他升级类型该参数无效。
 *                                      当wFileID=0xFFFF时，表示升级锁内数据区的数据。
 * @param  nOffset         [in]         偏移量。升级有关文件操作时的文件偏移量。其他升级类型该参数无效。
 * @param  pBuffer         [in]         输入数据。
 *                                      当nFunc = UPDATE_FUNC_CreateFile时, pBuffer指向要文件的属性结构，例如KEY_FILE_ATTR。
 *                                      当nFunc = UPDATE_FUNC_WriteFile时, pBuffer指向要写入的数据。
 *                                      当nFunc = UPDATE_FUNC_FileLic时, pBuffer指向文件权限的数据结构，例如：DATA_LIC。
 *                                      当nFunc = UPDATE_FUNC_SeedCount时, pBuffer指向long值，表示种子码可运算次数。
 *                                      当nFunc = UPDATE_FUNC_DownloadExe时, pBuffer指向EXE_FILE_INFO结构，与Dongle_DownloadExeFile函数用法类似。
 *                                      当nFunc = UPDATE_FUNC_Deadline时, pBuffer指向DWORD值，表示到期的时间。
 * @param  nBufferLen      [in]         参数pBuffer的缓冲区大小。
 * @param  pOutData        [out]        输出数据。制作的升级包数据。
 * @param  pOutDataLen     [in,out]     参数pOutData输入大小，返回升级包的有效长度。
 *
 * @return DONGLE_SUCCESS               制作升级包成功。
 */
DWORD WINAPI Dongle_MakeUpdatePacketFromMother(DONGLE_HANDLE hDongle, char * pHID, int nFunc, int nFileType, WORD wFileID, int nOffset, BYTE * pBuffer, int nBufferLen, BYTE * pOutData, int * pOutDataLen);

/**
 * @brief  远程升级子锁中的数据。匿名权限即可。升级数据pUpdateData对一把锁只能使用一次。
 *         1.本函数内部是按1024字节的分块机制发送,如遇返回值不是DONGLE_SUCCESS会立即中断发送并返回。
 *         2.如果需要进行流程控制,可由调用方来分块(每块1024字节)发送，并保证数据块的顺序不被打乱，根据返回的错误码来控制流程。
 *
 * @param  hDongle         [in]     打开的加密锁句柄。
 * @param  pUpdateData     [in]     输入数据。升级数据，由Dongle_MakeUpdatePacket或者Dongle_MakeUpdatePacketFromMother产生
 * @param  nDataLen        [in]     参数pUpdateData的大小。必须为1024的整数倍。
 *
 * @return DONGLE_SUCCESS           升级成功。
 */
DWORD WINAPI Dongle_Update(DONGLE_HANDLE hDongle, BYTE * pUpdateData, int nDataLen);


//错误码
#define   DONGLE_SUCCESS                       0x00000000          // 操作成功
#define   DONGLE_NOT_FOUND                     0xF0000001          // 未找到指定的设备
#define   DONGLE_INVALID_HANDLE				   0xF0000002		   // 无效的句柄
#define   DONGLE_INVALID_PARAMETER			   0xF0000003		   // 参数错误
#define   DONGLE_COMM_ERROR					   0xF0000004		   // 通讯错误
#define   DONGLE_INSUFFICIENT_BUFFER		   0xF0000005		   // 缓冲区空间不足
#define   DONGLE_NOT_INITIALIZED               0xF0000006		   // 产品尚未初始化 (即没设置PID)
#define   DONGLE_ALREADY_INITIALIZED		   0xF0000007		   // 产品已经初始化 (即已设置PID)
#define   DONGLE_ADMINPIN_NOT_CHECK			   0xF0000008		   // 开发商密码没有验证
#define   DONGLE_USERPIN_NOT_CHECK			   0xF0000009		   // 用户密码没有验证
#define   DONGLE_INCORRECT_PIN				   0xF000FF00		   // 密码不正确 (后2位指示剩余次数)
#define   DONGLE_PIN_BLOCKED				   0xF000000A		   // PIN码已锁死
#define   DONGLE_ACCESS_DENIED				   0xF000000B		   // 访问被拒绝 
#define   DONGLE_FILE_EXIST					   0xF000000E		   // 文件已存在
#define   DONGLE_FILE_NOT_FOUND				   0xF000000F		   // 未找到指定的文件
#define	  DONGLE_READ_ERROR                    0xF0000010          // 读取数据错误
#define	  DONGLE_WRITE_ERROR                   0xF0000011          // 写入数据错误
#define	  DONGLE_FILE_CREATE_ERROR             0xF0000012          // 创建文件错误
#define	  DONGLE_FILE_READ_ERROR               0xF0000013          // 读取文件错误
#define	  DONGLE_FILE_WRITE_ERROR              0xF0000014          // 写入文件错误
#define	  DONGLE_FILE_DEL_ERROR                0xF0000015          // 删除文件错误
#define   DONGLE_FAILED                        0xF0000016          // 操作失败
#define   DONGLE_CLOCK_EXPIRE                  0xF0000017          // 加密锁时钟到期
#define   DONGLE_ERROR_UNKNOWN		           0xFFFFFFFF		   // 未知的错误

#ifdef __cplusplus 
}
#endif

#endif