#ifndef __DIC_PLUS_H__
#define __DIC_PLUS_H__
// ====================================================================

// 1. 查找计算机上连接的加密锁设备
EXTERN_C int WINAPI DicPlus_Find();
// 返回:
// 如果返回值小于 0，表示返回的是一个错误编码，具体含义请看下面的错误编码。
// 如果返回值为 0，表示没有任何读卡器设备存在。
// 如果返回值大于 0，返回的是读卡器的个数(注意: 通常即使用户不插入设备，
// 因为驱动程序一直存在，驱动程序会报告读卡器存在，这时候需要通过 DicPlus_Open 
// 命令进一步确认是否真的有设备存在)，我们假设在同一台计算机上面最多有 32 
// 个读卡器设备。

// ====================================================================

// 2. 打开指定的读卡器
EXTERN_C int WINAPI DicPlus_Open(int hic, char* reader_name);
// 输入:
// hic	设备的句柄
// 这是一个枚举值，是一个 0, 1, 2, ... DicPlus_Find() 返回值之内的值。
// 例如，DicPlus_Find() 返回 3，表示找到了 3 个读卡器设备，那么 
// DicPlus_Open(0, reader_name) 表示的就是打开第 1 个找到的读卡器设备，
// 1 就表示第 2 个，依此类推。
// 如果 hic 的值为负值，DicPlus_Open 会尝试直接使用 reader_name 中的读卡
// 器名称去直接打开对应的读卡器。
// reader_name 读卡器的名称
// 这是一个输入输出值，当 hic 为负值的时候，用来输入读卡器的名称。
// 如果这个值不为空(NULL)，那么在成功的打开读卡器后，会从这里返回读
// 卡器的名称。
// 另外，对于虚拟设备来说，这是要打开的路径文件名。
// 返回值:
// 大于等于0		表示成功，返回的就是 hic 的打开句柄
// 小于0			返回的是一个错误编码，具体含义请看下面的错误编码部分。
// ====================================================================

// 3. 关闭指定的读卡器
EXTERN_C int WINAPI DicPlus_Close(int hic);
// 输入:
// hic 设备的句柄，跟 DicPlus_Open 命令中的 hic 一致。
// 返回:
// 返回的是一个错误编码，具体含义请看下面的错误编码部分。

// ====================================================================

// 4. 创建虚拟 IC 卡文件(*虚拟卡专用指令)
EXTERN_C int WINAPI DicPlus_Create(char* filename, char* mfname, char* atr, WORD atrlen);
// 输入:
// filename	要创建的路径文件名
// mfname	主卷标的名字
// atrlen	开发商信息的长度
// atr		开发商信息
// 返回:
// 返回的是一个错误编码，具体含义请看下面的错误编码部分。

// ======================================================================

// 5. 获得此动态库的版本
EXTERN_C int WINAPI DicPlus_GetVersion(char* ver);
// 输入:
// ver 用来返回此动态库的版本号
// 返回:
// 在 ver 中返回的动态库版本内容如下:
// [V/R]xx.yy
// 例如: R01.10	表示这是 "真实卡动态库 1.10 版本"
// 例如: R03.02	表示这是 "虚拟卡动态库 3.02 版本"
// 返回的是一个错误编码，具体含义请看下面的错误编码部分。

// ======================================================================

// 6. 加密锁操作命令函数
EXTERN_C int WINAPI DicPlus_Command(int hic, int cmd, void* cmddata);
// 输入:
// hic		设备的句柄
// cmd		具体的命令
// cmddata	跟命令相关的数据
EXTERN_C int WINAPI DicPlus_FindByMgrCode(void * pMgrCode);

EXTERN_C int WINAPI EnterMultThreadMode();
// Command 定义
#define GET_CARD_INFO				0x02000000
#define GET_HARDWARE_INFO			0x00FF1C00
#define GET_MANAGER_CODE			0x00FF1A00
#define GET_CARD_PRIVILEGE			0x00FF2200
#define GET_REMOTE_INFO				0x00FF1e00
#define SET_REMOTE_INFO				0x00FF1003
#define CHECK_REMOTE_INFO			0x01FF1003
#define GET_CURRENT_DIR				0x00001800
#define SET_CURRENT_DIR				0x0000a400
#define GET_CURRENT_FILE			0x00001801
#define SET_CURRENT_FILE			0x0100a400
#define GET_PARENT_DIR				0x00001802
#define SET_PARENT_DIR				0x0000a402
#define LIST_DIR					0x0000b200
#define	READ_FILE					0x0000b000
#define WRITE_FILE					0x0000d000
#define FORMAT_CARD					0x0000e000
#define CREATE_DIR					0x0000e001
#define CREATE_FILE					0x0100e001
#define REMOVE_DIR					0x0000ee00
#define REMOVE_FILE					0x0000ee01
#define RANDOM						0x00008400
#define RUN							0x03000000
#define CRYPTOTEXT_FILE				0x04000000
#define PLAINTEXT_FILE				0x05000000

#define DESENC						0x00005000
#define DESDEC						0x00005100
#define RSAGENKEY					0x00003300
#define RSAENC						0x00003400
#define RSADEC						0x00003500

#define GETFREESPACE				0X00003000
#define MODULE_GETFILEFLAG			0X00003600
#define MODULE_CRYPTOTEXT_MODFLAG	0x00003700
#define MODULE_PLAINTEXT_MODFLAG	0x00003800
#define MODULE_ENCRYPT_CONTENT		0x00003900

#define SETCOUNTER				0x00005700
#define STEPCOUNTER					0x00005800

#define CHECK_SUPER_PASS			0x00FF1002
#define SET_SUPER_PASS				0x00FFF402
#define GET_UPGRADE_REMOTE_PASS		0x00FF2000

// data 跟命令有关的参数

// GET_CARD_INFO 命令的 data 中的数据结构
typedef struct
{
	char volume[16];
	char atr[15];
} DicPlus_CardInfo;

// GET_HARDWARE_INFO 命令的 data 中的数据结构
typedef struct
{
	DWORD FactoryTime;
	DWORD HardSerial;
	DWORD ShipTime;
	DWORD COSVersion;
} DicPlus_HardInfo;

// GET_MANAGER_CODE 命令的 data 中的数据结构
typedef struct
{
	WORD Zone;
	WORD Agent;
	WORD User1;
	WORD User2;
} DicPlus_ManagerCode;

// GET_CARD_PRIVILEGE 命令的 data 中只有一个字节的数据
// 用位来表示当前密码验证的状态
#define DICPR_NOPASS		0
#define	DICPR_SUPERPASS		4
#define	DICPR_REMOTEPASS	8

// GET_REMOTE_INFO, SET_REMOTE_INFO, CHECK_REMOTE_INFO 命令的 data 中的数据结构
typedef struct
{
	DWORD RemoteTag;
	BYTE RemotePass[8];
} DicPlus_RemoteInfo;

// GET_UPGRADE_REMOTE_PASS 命令的 data 中的数据结构
typedef struct
{
	DWORD RemoteTag;
	DWORD HardSerial;
	BYTE RemotePass[8];
} DicPlus_Upgrade_RemotePass;

// GET_CURRENT_DIR, SET_CURRENT_DIR, GET_PARENT_DIR, CREATE_DIR 命令的 data 中的数据结构
typedef struct
{
	WORD		dirid;			// 目录 ID
	BYTE		dircla;			// 目录类别
	BYTE		diratrpri;		// 目录属性 & 目录安全级
	WORD		dirsize;		// 没用
	char		dirname[16];	// 目录名
} DicPlus_Dir;

// GET_CURRENT_FILE, SET_CURRENT_FILE, CREATE_FILE 命令的 data 中的数据结构
typedef struct
{
	WORD		fileid;			// 文件 ID
	BYTE		filecla;		// 文件类别
	BYTE		fileatrpri;		// 文件属性 & 文件安全级
	WORD		filesize;		// 文件大小
	char		filename[17];	// 文件名
	char        filepath[128];
} DicPlus_File;

// 文件的属性定义
#define FILEATTR_NORMAL			0x00
#define FILEATTR_EXEC			0x10	// 表示可执行
#define FILEATTR_DIR			0x20	// 表示为目录文件
#define FILEATTR_UPIGNORE		0x40	// 表示如果当前安全级别已经高于可执行程序自身的级别，则不执行本程序
#define FILEATTR_INTERNAL		0x80	// 表示为内部应用程序自己使用的文件，不能从外部进行操作
                                        // 如果一个可执行文件也设置内部属性,在列目录时就只有通过超级密码验证之后才能显示出来
// 注意: LIST_DIR 命令返回的结构既可能是 DicPlus_Dir 也可能是 DicPlus_File
// 具体是哪个，要通过判断 FILEATTR_DIR 来决定

// READ_FILE 命令的 data 需要数据结构
typedef struct
{
	WORD		offset;
	WORD		size;
} DicPlus_Before_Read_Data;

// READ_FILE 命令的 data 返回的数据结构
typedef struct
{
	WORD		readedsize;
	char		buffer[1];		// 大小为 readsize
} DicPlus_After_Read_Data;

// WRITE_FILE 命令的 data 中的数据结构
typedef struct
{
	WORD		offset;
	WORD		size;
	char		buffer[1];		// 大小为 size
} DicPlus_Write_Data;

// FORMAT_CARD 命令的 data 中的数据结构
typedef struct
{
	char		volume[16];
	char		atr[15];
	BOOL        isdes;
	BOOL        isfloat;
	BOOL        isExtend;
	BYTE		bModify;
} DicPlus_System_Info;

// RUN_DATA 所需要的 data 结构是:
typedef struct
{
	WORD RunID;
	WORD ParaSize;
	BYTE Para[1];	// 大小为 ParaSize
} DicPlus_Before_Run_Data;

// RUN_DATA 返回参数的时候，data 结构是:
typedef struct
{
	WORD ResultSize;
	BYTE Result[1]; // 大小为 ResultSize
} DicPlus_After_Run_Data;

// SET_SUPER_PASS 命令的 data 中的数据结构
typedef struct
{
	BYTE MaxTryTimes;
	BYTE SuperPass[8];
} DicPlus_SuperPass_Data;

// CRYPTOTEXT_FILE 命令的 data 中的数据结构
typedef struct
{
	WORD		fileid;			// 文件 ID
	BYTE		filecla;		// 文件类别
	BYTE		fileatrpri;		// 文件属性 & 文件安全级
	WORD		filesize;		// 文件大小
	char		filename[16];	// 文件名
	BYTE		filedata[1];	// 文件的内容，大小为 filesize
} DicPlus_Before_CryptoFile_Data;

// CRYPTOTEXT_FILE 返回参数的时候，data 结构是:
typedef struct
{
	WORD ResultSize;
	BYTE Result[1]; // 大小为 ResultSize
} DicPlus_After_CryptoFile_Data;

// 三种加密和解密算法命令需要填充的结构体
typedef struct 
{								//对加密来说		//对解密来说
	WORD dataLength;			//加密数据的长度	//解密数据的长度
	WORD KeyFileid;				//DES密钥文件的文件ID
	BYTE KeyIndex;				//本次加密使用的密钥在DES密钥文件中的标识，
								//注意一个DES密钥文件包含多个DES加解密使用的密钥
								//各次解密使用的密钥用KeyIndex来标识

	char data[1];				//加密数据			//解密数据
} DicPlus_Des_Data;
typedef struct 
{								//对加密来说		//对解密来说
	WORD	KeyFileid;			//公钥文件的文件ID	//私钥文件的文件ID
	WORD	dataLength;			//加密数据的长度	//解密数据的长度
	char	data[1];			//加密数据			//解密数据
} DicPlus_Rsa_Data;
typedef struct 
{
	WORD pubKeyID;				//公钥文件ID
	WORD pubKeySize;			//公钥大小 512/1024
	WORD pivKeyID;				//私钥文件ID
} DicPlus_Rsa_GenKey;
// 三种加密算法调用返回的数据结构
typedef struct 
{								//对加密来说			//对解密来说
	WORD dataLength;			//加密后返回数据的长度	//解密后返回数据的长度
	char data[1];			//加密后返回数据		//解密后返回数据
} DicPlus_After_EncDec_Data;

// 取得模块开通标识结构
typedef struct 
{
	BYTE NumFileID;		//可执行文件ID序列的实际BYTE长度
	WORD fileID[1];		//需要读取的可执行文件的ID的序列
}
DicPlus_Module_GetFLag;
typedef BYTE* FileFlag;	//取得模块开通标识后的返回值


typedef struct  
{
	WORD fileID;	//文件ＩＤ
	BYTE flag;		//文件开通标识
}
DicPlus_PlainText_OneMod;
typedef struct
{
	BYTE dataLen;
	DicPlus_PlainText_OneMod data[1];
}
DicPlus_PlainText_ModuleData;

typedef struct 
{
	BYTE numExe;		//可执行文件的个数，最大为３
	BYTE flag;			//后面指定的几个可执行文件的开通标识
	WORD exeFileID[3];	//可执行文件ID
}
DicPlus_CryptoText_OneMod;
typedef struct  
{
	BYTE nDataLen;
	DicPlus_CryptoText_OneMod plaintextModuleData[1];
}
DicPlus_CryptoText_Encrypt;

typedef struct
{
	BYTE dataLen;
	BYTE data[1];
}
DicPlus_CryptoText_ModuleData;

// ====================================================================

// 7. 帮助用户从 DicPlus_Command 命令中的 data 中提取数据的函数
EXTERN_C int WINAPI DicPlus_Get(void* xdata, int p1, int p2, char* buffer);
// 输入:
// xdata	提取的目标缓冲区
// p1		模式/偏移，最高位为 1 表示这是模式操作，否则是用户自定义的偏移值
// p2		返回方式/尺寸，最高2位表示返回的方式，剩余的位表示操作的尺寸
// buffer	提取出数据的字符型缓冲区

// ====================================================================

// 8. 帮助用户向 DicPlus_Command 命令中的 data 中设置数据的函数
EXTERN_C int WINAPI DicPlus_Set(void* xdata, int p1, int p2, int p3, char* buffer);
// 输入:
// xdata	存储的目标缓冲区
// p1		模式/偏移，最高位为 1 表示这是模式操作，否则是用户自定义的偏移值
// p2		传入方式/尺寸，最高2位表示传入的方式，剩余的位表示操作的大小
// p3		通常是用户要传入的值。
// buffer	准备存储的字符型缓冲区

// ====================================================================
// 对于 DicPlus_Get, DicPlus_Set 的 p1 参数，我们预定义了一些模式操作
// 这些模式操作的最高位我们都设定为 1
#define FILL					0x80000000
#define ATR						0x80010000
#define VOLUME					0x80020000
#define FACTORY_TIME			0x80030000
#define HARD_SERIAL				0x80040000
#define SHIP_TIME				0x80050000
#define COS_VERSION				0x80060000
#define ZONE					0x80070000
#define AGENT					0x80080000
#define USER1					0x80090000
#define USER2					0x800A0000
#define DIR_ID					0x800B0000
#define DIR_CLASS				0x800C0000
#define DIR_ATTRIBUTE			0x800D0000
//#define DIR_PRIVILEGE			0x800E0000	目录没有安全级别
#define FILE_NO					0x800F0000
#define DIR_NAME				0x80100000
#define FILE_ID					0x80110000
#define FILE_CLASS				0x80120000
#define FILE_ATTRIBUTE			0x80130000
#define FILE_PRIVILEGE			0x80140000
#define FILE_SIZE				0x80150000
#define FILE_NAME				0x80160000
#define WRITE_DATA				0x80170000
#define READ_DATA				0x80180000
#define RUN_DATA				0x80190000
#define RANDOM_SIZE				0x801A0000
#define REMOTE_TAG				0x801B0000
#define REMOTE_PASS				0x801C0000
#define UPGRADE_HARD_SERIAL		0x801D0000
#define UPGRADE_REMOTE_TAG		0x801E0000
#define UPGRADE_REMOTE_PASS		0x801F0000
#define REMOTEPASS_STATUS		0x80200000
#define SUPERPASS_STATUS		0x80210000
#define SUPERPASS_DATA			0x80220000
#define FILE_DATA				0x80230000
#define RELOAD_FLOATLIBS            0x0000e002
#define RELOAD_RSADESLIBS           0x0000e003
#define ISLOADEDFLOATLIBS           0x0000e004
#define ISLOADEDRSADESLIBS          0x0000e005 
#define RELOAD_EXTENDFLOATLIBS      0x0000e006
#define ISLOADEXTENDFLOATLIBS       0x0000e007
#define WRITE_HARDSERIAL            0x0000e008

//DicPlus_Des_Data			//Used by DicPlus_Set
#define DES_KEYFILEID			0x80240000
#define DES_KEYINDEX			0x80250000
#define DES_DATA				0x80260000

//DicPlus_After_EncDec_Data	//Used by DicPlus_Get
#define AFTER_ENCDEC_DATA		0x80270000

//DicPlus_Rsa_Data			//Used by DicPlus_Set
#define RSA_KEYFILEID			0x80280000
#define RSA_DATA				0x80290000

//DicPlus_Rsa_GenKey			//Used by DicPlus_Set
#define RSA_PUBKEYID			0x802A0000
#define RSA_PUBKEYSIZE			0x802B0000
#define RSA_PIVKEYID			0x802C0000

// ====================================================================
// 对于 DicPlus_Get, DicPlus_Set 的 p2 参数，最高 2 位表示数据的传入或返回方式
#define BY_VALUE				0x00000000
#define BY_DOUBLE				0x40000000
#define BY_ARRAY				0x80000000
#define BY_STRING				0xC0000000

// 对于 DicPlus_GetData 而言
// BY_VALUE		表示返回数据是通过返回值来返回的
// BY_DOUBLE	表示返回数据如果是浮点数的化，会被转换成字符串的方式放在 buffer 中返回
// BY_ARRAY		表示返回数据被放在 buffer 中来返回
// BY_STRING	表示返回数据如果是数字的化，会被转换成字符串的方式放在 buffer 中返回

// 对于 DicPlus_SetData 而言
// BY_VALUE		表示数据是通过 p3 来输入的
// BY_DOUBLE	表示数据是通过 buffer 输入，并且 buffer 中的数据是浮点数的字符串形式
// BY_ARRAY		表示数据是通过 buffer 来输入的
// BY_STRING	表示数据是通过 buffer 输入，并且 buffer 中的数据是字符串形式
// ====================================================================


// 错误编码 ===========================================================

#ifndef SCARD_S_SUCCESS

// 成功，没有错误
#define SCARD_S_SUCCESS 				((DWORD)0x00000000L)

// 内部连接检查失败
#define SCARD_F_INTERNAL_ERROR			((DWORD)0x80100001L)

// 操作被用户中止
#define SCARD_E_CANCELLED				((DWORD)0x80100002L)

// 不正确的操作句柄
#define SCARD_E_INVALID_HANDLE			((DWORD)0x80100003L)

// 不正确的参数(p1, p2)
#define SCARD_E_INVALID_PARAMETER		((DWORD)0x80100004L)

// 注册的启动信息丢失或无效
#define SCARD_E_INVALID_TARGET			((DWORD)0x80100005L)

// 没有足够的内存用于完成命令
#define SCARD_E_NO_MEMORY				((DWORD)0x80100006L)

// 内部超时
#define SCARD_F_WAITED_TOO_LONG			((DWORD)0x80100007L)

// 用户给出的缓冲区太小，不足以放下全部的返回数据
#define SCARD_E_INSUFFICIENT_BUFFER		((DWORD)0x80100008L)

// 未知的读卡器
#define SCARD_E_UNKNOWN_READER			((DWORD)0x80100009L)

// 用户指定的时间超时
#define SCARD_E_TIMEOUT					((DWORD)0x8010000AL)

// 卡正在被其它连接占用
#define SCARD_E_SHARING_VIOLATION		((DWORD)0x8010000BL)

// 在读卡器里面没有卡
#define SCARD_E_NO_SMARTCARD			((DWORD)0x8010000CL)

// 未知的卡类型
#define SCARD_E_UNKNOWN_CARD			((DWORD)0x8010000DL)

// 读卡器无法完成退出卡操作
#define SCARD_E_CANT_DISPOSE			((DWORD)0x8010000EL)

// 当前的卡不支持用户指定的通讯协议
#define SCARD_E_PROTO_MISMATCH			((DWORD)0x8010000FL)

// 卡还没有准备好接收命令
#define SCARD_E_NOT_READY				((DWORD)0x80100010L)

// 某些变量的值不合适
#define SCARD_E_INVALID_VALUE			((DWORD)0x80100011L)

// 操作被系统中止，可能是重新登陆或关机
#define SCARD_E_SYSTEM_CANCELLED		((DWORD)0x80100012L)

// 内部通讯错误
#define SCARD_F_COMM_ERROR				((DWORD)0x80100013L)

// 内部未知错误
#define SCARD_F_UNKNOWN_ERROR			((DWORD)0x80100014L)

// 无效的 ATR 串
#define SCARD_E_INVALID_ATR				((DWORD)0x80100015L)

// 用户尝试结束某个不存在的处理
#define SCARD_E_NOT_TRANSACTED			((DWORD)0x80100016L)

// 指定的读卡器当前无法使用
#define SCARD_E_READER_UNAVAILABLE		((DWORD)0x80100017L)

// 操作被中止，允许服务程序退出
#define SCARD_P_SHUTDOWN				((DWORD)0x80100018L)

// PCI 的接收缓冲区太小
#define SCARD_E_PCI_TOO_SMALL			((DWORD)0x80100019L)

// 读卡器的驱动无法支持当前的读卡器
#define SCARD_E_READER_UNSUPPORTED		((DWORD)0x8010001AL)

// 读卡器的驱动程序无法建立唯一的名字，已经有相同名字的读卡器存在
#define SCARD_E_DUPLICATE_READER		((DWORD)0x8010001BL)

// 卡无法被当前的读卡器支持
#define SCARD_E_CARD_UNSUPPORTED		((DWORD)0x8010001CL)

// 智能卡服务没有开启
#define SCARD_E_NO_SERVICE				((DWORD)0x8010001DL)

// 智能卡服务已经被中止
#define SCARD_E_SERVICE_STOPPED			((DWORD)0x8010001EL)

// 某个意外的智能卡错误产生
#define SCARD_E_UNEXPECTED				((DWORD)0x8010001FL)

// 无法获知智能卡的提供者信息
#define SCARD_E_ICC_INSTALLATION		((DWORD)0x80100020L)

// 无法获知智能卡的生产者信息
#define SCARD_E_ICC_CREATEORDER			((DWORD)0x80100021L)

// 当前的智能卡无法支持用户要求的功能
#define SCARD_E_UNSUPPORTED_FEATURE		((DWORD)0x80100022L)

// 指定的目录不存在
#define SCARD_E_DIR_NOT_FOUND			((DWORD)0x80100023L)

// 指定的文件不存在
#define SCARD_E_FILE_NOT_FOUND			((DWORD)0x80100024L)

// 指定的目录不再是有效的目录
#define SCARD_E_NO_DIR					((DWORD)0x80100025L)

// 指定的文件不再是有效的文件，没有选择文件
#define SCARD_E_NO_FILE					((DWORD)0x80100026L)

// 此文件拒绝访问
#define SCARD_E_NO_ACCESS				((DWORD)0x80100027L)

// 卡的空间已满，无法再写入信息
#define SCARD_E_WRITE_TOO_MANY			((DWORD)0x80100028L)

// 设置文件指针错误
#define SCARD_E_BAD_SEEK				((DWORD)0x80100029L)

// PIN 码错误
#define SCARD_E_INVALID_CHV				((DWORD)0x8010002AL)

// 一个无法识别的错误码从智能卡服务返回
#define SCARD_E_UNKNOWN_RES_MNG			((DWORD)0x8010002BL)

// 请求的证书不存在
#define SCARD_E_NO_SUCH_CERTIFICATE		((DWORD)0x8010002CL)

// 请求的证书不允许获得
#define SCARD_E_CERTIFICATE_UNAVAILABLE	((DWORD)0x8010002DL)

// 找不到任何一个读卡器
#define SCARD_E_NO_READERS_AVAILABLE	((DWORD)0x8010002EL)

// 智能卡通讯过程中发生数据丢失，请再次尝试
#define SCARD_E_COMM_DATA_LOST			((DWORD)0x8010002FL)

// 请求的密钥文件不存在
#define SCARD_E_NO_KEY_CONTAINER		((DWORD)0x80100030L)

// 由于 ATR 配置冲突，读卡器无法跟卡通讯
#define SCARD_W_UNSUPPORTED_CARD		((DWORD)0x80100065L)

// 卡对复位没有响应
#define SCARD_W_UNRESPONSIVE_CARD		((DWORD)0x80100066L)

// 卡没有电
#define SCARD_W_UNPOWERED_CARD			((DWORD)0x80100067L)

// 卡被复位了，因此共享的信息无效了
#define SCARD_W_RESET_CARD				((DWORD)0x80100068L)

// 卡已经被移出了
#define SCARD_W_REMOVED_CARD			((DWORD)0x80100069L)

// 因为安全规则，访问被拒绝了
#define SCARD_W_SECURITY_VIOLATION		((DWORD)0x8010006AL)

// PIN 码没有被验证，访问被拒绝
#define SCARD_W_WRONG_CHV				((DWORD)0x8010006BL)

// 已经到达最大 PIN 码验证次数，访问被拒绝
#define SCARD_W_CHV_BLOCKED				((DWORD)0x8010006CL)

// 已经到达最后的智能卡文件，没有更多的文件可以访问了
#define SCARD_W_EOF						((DWORD)0x8010006DL)

// 操作被用户中止
#define SCARD_W_CANCELLED_BY_USER		((DWORD)0x8010006EL)

// 智能卡 PIN 没有设置
#define SCARD_W_CARD_NOT_AUTHENTICATED	((DWORD)0x8010006FL)

#endif // SCARD_S_SUCCESS


// 扩展错误码 =========================================================

// 文件已经存在
#define SCARD_E_FILE_EXISTS				((DWORD)0xA0100001L)

// 卡内存储器操作出错
#define SCARD_E_EPROM_ERROR				((DWORD)0xA0100002L)

// 用户给出了无效的 CLA
#define SCARD_E_INVALID_CLA				((DWORD)0xA0100003L)

// 用户给出了无效的 INS
#define SCARD_E_INVALID_INS				((DWORD)0xA0100004L)

// VM 地址超界/异常
#define SCARD_E_VM_ADDRESS_ERROR		((DWORD)0xA0100005L)

// 除 0 错
#define SCARD_E_ZERO_DIVIDE				((DWORD)0xA0100006L)

// 卡没有被插入到正确的位置
#define SCARD_E_WRONG_POSITION			((DWORD)0xA0100007L)

// 卡当前处于某种未知的状态
#define SCARD_E_UNKNOWN_STATE			((DWORD)0xA0100008L)

// 卡还没有被打开
#define SCARD_E_CARD_NOT_OPENED			((DWORD)0xA0100009L)

// 未知的命令
#define SCARD_E_UNKNOWN_COMMAND			((DWORD)0xA010000AL)

// 即将设定超级密码的重新设置次数是 0
#define SCARD_E_ZERO_TRYTIME			((DWORD)0xA010000BL)

// 打开了太多的设备
#define SCARD_E_TOO_MANY_DEVICE			((DWORD)0xA010000CL)

// 非法指令错
#define	SCARD_E_INVALID_INSTRUCTION		((DWORD)0xA010000DL)

// 卡还有数据要返回
#define SCARD_W_RESPONSE				((DWORD)0xA01000FFL)

#define RETURN_LENGTH					((DWORD)0xA0100014)

// 虚拟设备专用错误码 =================================================

// 创建虚拟卡文件失败
#define SCARD_E_FILE_CREATE_FAILED		((DWORD)0xA0101001L)

// 打开虚拟卡文件失败
#define SCARD_E_FILE_OPEN_FAILED		((DWORD)0xA0101002L)

#define SCARD_E_FLOAT_NOT_FOUND         ((DWORD)0x0000e00aL)
//查询RSA_DES库未发现
#define SCARD_E_RSADES_NOT_FOUND        ((DWORD)0x0000e00bL)

#define SCARD_E_LIBS_ERROR              ((DWORD)0x0000e00cL)

#define SCARD_E_FLOAT_NOT_FOUND         ((DWORD)0x0000e00aL)
//查询RSA_DES库未发现
#define SCARD_E_RSADES_NOT_FOUND        ((DWORD)0x0000e00bL)

#define SCARD_E_LIBS_ERROR              ((DWORD)0x0000e00cL)

#define SCARD_E_EXTENDFLOAT_NOT_FOUND   ((DWORD)0x0000e00dL)

//加密锁扩展错误代码

//找不到加密锁
#define ROCKEY_K_NOT_FOUND               ((DWORD)0xF0100001L)

//查询加密锁失败
#define ROCKEY_K_QUERY_FAILED            ((DWORD)0xF0100002L)

//写入数据失败
#define ROCKEY_K_WRITE_FAILED            ((DWORD)0xF0100003L)

//读出数据失败
#define ROCKEY_K_READ_FAILED             ((DWORD)0xF0100004L)















/***********************************************************************************************************************
										下面为Rockey6Smart升级新增函数
***********************************************************************************************************************/

//	关于一些较难理解问题的说明：

// (1) 目录/文件 ID (2 字节)
//     在加密锁内部，每个文件与目录都有一个 ID，这是一个 16 位的数字。在同一目
//     录下的目录与文件的 ID 必须不同，作为文件和目录的唯一标识。
//     注意:
//     3F00 是根目录 ID
//     2F01 是 ATR 文件的 ID
//     3FFF 是当前目录的 ID(我们没有使用)
//     0000 系统保留
//     在创建文件和目录的时候，不能够选择以上的 ID
//
// (2) 目录/文件 类别编码 (1 字节)
//     a) 每个文件和目录都拥有一个自身的类别描述字节，我们称其为类别码(FCLA)，
//        表示一个文件所属的类别，类别编码相同的文件一般会被假定为同一产品的文
//        件。
//     b) 加密锁内部的可执行文件受到类别码(FCLA)的限制，只能够读写和创建相同类
//        别码(FCLA)的数据文件。
//     c) 每个文件的类别码(FCLA)在 IC 卡中占用 1 字节，表示范围是 0 - FF。
//     d) 系统有一个字节用来记录当前的文件类别，我们称其为 "系统当前文件类别"
//        (SYSFCLA)。
//     e) 作为特例 FF(16 进制) 表示"无类别"，FF 类别的数据文件可以被任何类别的
//        可执行文件访问。
//     f) "系统当前文件类别" 在执行 "文件选择" 命令的时候或调用加密锁中的可执
//        行文件的时候，会产生切换 SYSFCLA = FCLA。
//     g) 如果切换的目标文件是 "无类别" 文件，SYSFCLA 会保持原有类别不变。
//     h) 当产生类别切换的时候，"系统当前文件权限" 会被重新置 0 (SYSPRI = 0)
//     i) 加密锁复位后 SYSFCLA 缺省是 FF (无类别)。
//     j) 系统根目录的类别是 "无类别"。
//     k) 在加密锁中，相同类别的文件构成一个相对独立的子系统，"无类别" 数据文
//        件是公共数据，"无类别" 可执行文件是公共处理程序。
//     l) 无类别的可执行文件只能访问无类别的数据文件。
//     m) 类别编码更为重要的用途是在可执行的过滤器文件上，以后我们会详细说明。
//
//     对使用者的建议:
//     a) 通常类别编码被应用在开发者的管理上，如果某个开发商开发了多个产品，或
//        多个开发商合作开发同一个产品，类别管理是很方便的做法。
//     b) 如果某个加密锁只需要保护一套软件，建议类别码都设定为 FF。
//     c) 在同一目录下的文件和子目录建议设定相同的类别，用 "文件权限" 来控制访
//        问。
//
// (3) 目录/文件 权限编码 (半字节)
//     a) 除了类别以外，每个文件还有一个 "文件的访问权限"(FPRI) 来限制相同类别
//        文件的访问控制。
//     b) 文件的访问权限(FPRI) 是针对相同类别文件设置的，也就是说 FPRI 是比 
//        FCLA 低一层的访问控制。
//     c) 文件的访问权限在 IC 卡中占用半个字节，编码范围是 0-15，分为 16 个不
//        同的权限。0 是最低级，15 是最高级。
//     d) 系统有一个字节用来记录当前的文件的访问权限 "系统当前文件访问权限"
//        (SYSFPRI)
//     e) "系统当前文件访问权限"(SYSFPRI) 必须由加密锁内部的可执行文件来提升，
//        SYSFPRI = 可执行文件通过系统调用设定的权限，但可执行文件不能设定超过
//        自身设定的权限 SYSFPRI <= 可执行文件的 FPRI。
//     f) 可执行文件只能操作低于或等于自身级别的数据文件(读、写、创建)。
//     g) 可执行文件在退出的时候可以清除 "系统当前文件访问权限"(SYSFPRI = 0)
//     i) 对加密锁内部的可执行文件而言，可以设定一个文件属性 FILEATTR_UPIGNORE，
//        这个属性告诉系统，如果 SYSFPRI >= 可执行文件的 FPRI，那么就不需要调
//        用这个可执行文件了。
//
//     对使用者的建议:
//     a) 权限编码是更高一层次的文件管理功能，如果用户不需要这么复杂的管理功能，
//        把所有的文件权限编码都设定为 0 就好了。
//     b) 注意权限编码跟类别编码不同，先分类再分级。
//
// (4) 目录/文件 属性 (半字节)
//     每个文件/目录都有一个属性描述(FATTR)，每1位代表一种属性。
//        NORMAL	0x00	// 普通文件					a)
//        EXEC		0x10	// 可执行文件				b)
//        DIR		0x20	// 这是一个目录，不是文件	c)
//        UPIGNORE	0x40	// 高于忽略					d)
//        INTERNAL	0x80	// 内部文件					e)
//	   a) 普通文件
//     b) 可执行属性，表明这个文件是加密锁内部可执行文件，这种文件必须被放在加
//        密锁的根目录下，而且只有在超级密码验证通过后才能创建和删除。
//     c) 目录属性，表示这是一个目录，如果这个属性被设置，其它属性位会被忽略。
//     d) 高于忽略属性，只对加密锁内部的可执行文件有效的属性位，如果 
//        SYSFPRI >= FPRI，这个执行被忽略。
//     e) 内部文件属性位，表示这个文件只能由可执行文件来访问，外部操作只有在超
//        级密码验证通过后可以删除，但不可以读写。没有超级密码验证通过的时候根
//        本不允许外部访问。
//
// (5) 文件的长文件名
//     在创建文件/目录的时候，可以给出一个不超过 16 个字符的长文件名，这个长文
//     件名是可选的。如果设定了长文件名，以后对这个文件的操作既可以通过文件 ID 
//     也可以通过长文件名来访问。
// ==========================================================================

// (1) 格式化操作必须在通过超级密码验证的情况下才允许进行，在进行格式化操作的
//     时候，用户可以重新设定加密锁的卷标和厂商信息，这两个信息也只有在这个时
//     候才能够设定。
// (2) 创建目录的时候要给出 目录ID、类别、权限、长文件名等信息。
// (3) 列目录指令采用的是一种枚举的方式，一直到最后返回错误，表示当前目录下没
//     有更多的文件或目录了。
// (4) SET_CURRENT_DIR 类似于 DOS 命令 cd xxxx，这个命令只需要给出目录 ID 或
//     长文件名就可以，如果通过长文件名来设定，必须首先把 ID 添为 0。
// (5) 创建文件的时候也要给出全部的 文件ID、类别、权限、属性、文件大小、长文件
//     名等信息，如果没有长文件名，全部添 0 就可以。
// (6) 创建文件会自动把被创建的文件设定为当前文件，不需要调用 SET_CURRENT_FILE

#define NOT_SUPPORT_SIZE_DURING_GENERATE_RSA_KEY 0x90000001						//生成Rsa Key时给出了不支持的大小（只支持512位和1024位两种）
#define GET_3DES_KEY_FAILED	0x90000002											//获取3DES密钥失败
#define NO_PASSED 0																//密码没有验证通过
#define SUPER_PASSWORD_PASSED 4													//超级密码验证通过
#define REMOTE_PASSWORD_PASSED 8												//远程升级密码验证通过
#define SUPER_REMOTE_PASSWORD_PASSED 12											//超级密码和远程升级密码均验证通过

//	BYTE gDesKey[128];
//	BYTE gRsaPubKey[128];


	//功能：				顺序表翻转
	//内部使用
	void plus_ReverseByte(BYTE* pBuf,DWORD len);
	void plus_ReverseDword(DWORD* pBuf,DWORD len);

	//下列函数中的hic参数均为加密锁的句柄

	//功能：				获取卷标和开发商信息
	//strVolume:			返回加密锁的卷标
	//strManufactureInfo:	返回加密锁内的开发商信息
	//这两个参数是开发商可以更改的，最终用户不可以更改
	EXTERN_C int WINAPI DicPlus_GetCardInfo(int hic, char strVolume[], char strManufactureInfo[]);

	//功能：				获取生产时间、序列号、销售时间、COS版本等信息
	//pdwProduceTime:		返回加密锁的生产时间
	//pdwSerial:			返回加密锁的序列号
	//pdwSalesTime:			返回加密锁的销售时间
	//pdwCOSVersion:		返回加密锁内的COS版本
	//这四个参数出厂时由生产商设定，不可更改
	EXTERN_C int WINAPI DicPlus_GetHardwareInfo(int hic, 
		DWORD *pdwProducedTime, 
		DWORD *pdwSerial, 
		DWORD *pdwSalesTime, 
		DWORD *pdwCOSVersion);

	//功能：					获取区域编码、代理商编码和两个用户编码
	//pwZoneCode:				返回区域编码
	//pwAgentCode:				返回代理商编码
	//pwUser1Code:				返回用户1的编码
	//pwUser2Code:				返回用户2的编码
	EXTERN_C int WINAPI DicPlus_GetManagerCode(int hic, 
		WORD *pwZoneCode, 
		WORD *pwAgentCode, 
		WORD *pwUser1Code, 
		WORD *pwUser2Code);

	//功能：					取得密码验证状态，由枚举变量piState返回，返回值意义如下：
	//	NO_PASSED = 0,									//密码没有验证通过
	//	SUPER_PASSWORD_PASSED = 4,						//超级密码验证通过
	//	REMOTE_PASSWORD_PASSED = 8,						//远程升级密码验证通过
	//	SUPER_REMOTE_PASSWORD_PASSED = 12				//超级密码和远程升级密码均验证通过
	EXTERN_C int WINAPI DicPlus_GetPasswordVerifyState(int hic, int *piState);

	//功能：					获取远程升级标志和升级密码
	//pdwFlag:					返回远程升级标志
	//abyPassword:				返回远程升级密码
	EXTERN_C int WINAPI DicPlus_GetRemoteInfo(int hic, DWORD *pdwFlag, BYTE abyPassword[]);

	//功能：					设置远程升级标志和升级密码
	//dwFlag:					设置远程升级标志
	//abyPassword:				设置远程升级密码
	EXTERN_C int WINAPI DicPlus_SetRemoteInfo(int hic, DWORD dwFlag, const BYTE abyPassword[]);

	//功能：					验证远程升级标志和升级密码是否正确
	//dwFlag:					要验证的远程升级标志
	//dwPassword:				要验证的远程升级密码
	EXTERN_C int WINAPI DicPlus_CheckRemoteInfo(int hic, DWORD dwFlag, const BYTE byPassword[]);

	//下面是关于文件和目录的一些操作，说明如下：

	//文件和目录的类别是一个0x00 - 0xff之间的数

	//文件和目录的属性：
	//FILEATTR_NORMAL			一般的数据文件
	//FILEATTR_EXEC				表示可执行
	//FILEATTR_DIR				表示为目录
	//FILEATTR_UPIGNORE			表示为高于忽略，如果当前系统安全级别高于或等于可执行程序的安全级别，则不执行该程序
	//FILEATTR_INTERNAL			表示为内部应用程序自己使用的文件，不能从外部进行操作。如果一个文件被标志为内部文件，它就具有了隐含的属性，在列目录时，只有通过了超级密码验证才能看到这些隐含的文件

	//文件的安全级别是一个0x00 - 0x0f之间的数

	//功能：					获取当前目录ID、类别、属性、安全级别和目录名
	//pwDirID:					返回当前目录ID
	//pbyDirType:				返回当前目录类别
	//pbyDirAttribute:			返回当前目录属性
	//strDirName:				返回当前目录名
	EXTERN_C int WINAPI DicPlus_GetCurrentDir(int hic, 
		WORD *pwDirID, 
		BYTE *pbyDirType, 
		BYTE *pbyDirAttribute, 
		char strDirName[]);

	//功能：					设置当前目录为wDirID标识的目录
	//wDirID:					目录ID
	//strDirName:				目录名
	//备注：					可以通过目录ID或目录名设置当前目录，传入其中一个值即可，
	//							另一个值可以设为0或NULL，两个值都传入时这两个值应该代表同一个目录
	EXTERN_C int WINAPI DicPlus_SetCurrentDir(int hic, 
		WORD wDirID,
		char strDirName[]);

	//功能：					获取当前文件ID、类别、属性、安全级别和文件名
	//pwFileID:					返回当前文件ID
	//pbyFileType:				返回当前文件类别
	//pwFileSize:				返回当前文件大小
	//pbyFileAttribute:			返回当前文件属性
	//pbyFileSecurity:			返回当前文件安全级别
	//strFileName:				返回当前文件名
	EXTERN_C int WINAPI DicPlus_GetCurrentFile(int hic, 
		WORD *pwFileID, 
		BYTE *pbyFileType, 
		WORD *pwFileSize, 
		BYTE *pbyFileAttribute, 
		BYTE *pbyFileSecurity, 
		char strFileName[]);

	//功能：					设置当前文件为wFileID标识的文件
	//wFileID:					文件ID
	//strFileName:				文件名
	//备注：					可以通过文件ID或文件名设置当前目录，传入其中一个值即可，
	//							另一个值可以设为0或NULL，两个值都传入时这两个值应该代表同一个文件
	EXTERN_C int WINAPI DicPlus_SetCurrentFile(int hic, 
		WORD wFileID,
		char strFileName[]);

	//功能：					获取上级目录ID、类别、属性、安全级别和目录名
	//pwDirID:					返回上级目录ID
	//pbyDirType:				返回上级目录类别
	//pbyDirAttribute:			返回上级目录属性
	//strDirName:				返回上级目录名
	EXTERN_C int WINAPI DicPlus_GetParentDir(int hic, 
		WORD *pwDirID, 
		BYTE *pbyDirType, 
		BYTE *pbyDirAttribute, 
		char strDirName[]);

	//功能：					回到上级目录
	EXTERN_C int WINAPI DicPlus_SetParentDir(int hic);

	//功能：					列出当前目录下的所有目录和文件，与DicPlus_FindNextFileOrDir配合使用
	//piPosition:				返回-1表示当前目录下没有目录或文件，其它表示还有目录或文件没有列出，
	//							可以通过调用DicPlus_FindNextFileOrDir函数获取剩余目录或文件
	//pwID:						返回当前目录下第1个目录或文件的ID
	//pbyType:					返回当前目录下第1个目录或文件的类别
	//pbyDirAttribute:			返回当前目录下第1个目录或文件的属性
	//pbyDirSecurity:			返回当前目录下第1个文件的安全级别(查询目录时返回0)
	//strName:					返回当前目录下第1个目录或文件的名称
	EXTERN_C int WINAPI DicPlus_FindFirstFileOrDir(int hic, 
		int *piPosition, 
		WORD *pwID, 
		BYTE *pbyType, 
		BYTE *pbyAttribute, 
		BYTE *pbySecurity, 
		char strName[]);

	//功能：					列出当前目录下的所有目录和文件，与DicPlus_FindFirstFileOrDir配合使用
	//piPosition:				返回-1表示已经列出当前目录下的所有目录和文件，其它表示还有目录或文件没有列出，
	//							可以通过继续调用DicPlus_FindNextFileOrDir函数获取剩余目录或文件
	//pwID:						返回当前目录下piPosition后的下1个目录或文件的ID
	//pbyType:					返回当前目录下piPosition后的下1个目录或文件的类别
	//pbyDirAttribute:			返回当前目录下piPosition后的下1个目录或文件的属性
	//pbyDirSecurity:			返回当前目录下piPosition后的下1个文件的安全级别(查询目录时返回0)
	//strName:					返回当前目录下piPosition后的下1个目录或文件的名称
	EXTERN_C int WINAPI DicPlus_FindNextFileOrDir(	int hic, 
		int *piPosition, 
		WORD *pwID, 
		BYTE *pbyType, 
		BYTE *pbyAttribute, 
		BYTE *pbySecurity, 
		char strName[]);

	//注意：读写文件时要首先将文件设置为当前文件，可通过DicPlus_SetCurrentFile设置，
	//DicPlus_CreateFile会自动将创建的文件设置为当前文件

	//功能：					读取当前文件
	//wOffset:					当前文件偏移量
	//pwRead:					传入要读取的数据大小，返回实际读取的数据大小
	//pBuf:						接收数据的缓冲区
	EXTERN_C int WINAPI DicPlus_ReadFile(int hic, WORD wOffset, WORD *pwRead, char *pBuf);

	//功能：					写当前文件
	//wOffset:					当前文件偏移量
	//wWrite:					要写入的数据大小
	//pBuf:						写入数据存放的缓冲区
	EXTERN_C int WINAPI DicPlus_WriteFile(int hic, WORD wOffset, WORD wWrite, const char *pBuf);

	//功能：					格式化
	//strVolume:				卷标
	//strManufactureInfo:		开发商信息
	//注意：					格式化完毕后要关闭再打开才可生效
	EXTERN_C int WINAPI DicPlus_FormatCard(int hic, char strVolume[], char strManufactureInfo[]);

	//功能：					创建一个目录
	//wDirID:					要创建的目录ID
	//byDirType:				创建的目录类别
	//byDirAttribute:			创建的目录属性
	//strDirName:				创建的目录名
	EXTERN_C int WINAPI DicPlus_CreateDir(int hic, 
		WORD wDirID, 
		BYTE byDirType, 
		BYTE byDirAttribute, 
		const char strDirName[]);

	//功能：					创建一个文件
	//wFileID:					要创建的文件ID
	//byFileType:				创建的文件类别
	//byFileAttribute:			创建的文件属性
	//byFileSecurity:			创建的文件安全级别
	//strFileName:				创建的文件名
	EXTERN_C int WINAPI DicPlus_CreateFile(int hic, 
		WORD wFileID, 
		BYTE byFileType, 
		BYTE byFileAttribute, 
		WORD wFileSize, 
		BYTE byFileSecurity, 
		const char strFileName[]);

	//功能：					删除当前目录
	EXTERN_C int WINAPI DicPlus_DeleteDir(int hic);

	//功能：					删除当前文件
	EXTERN_C int WINAPI DicPlus_DeleteFile(int hic);

	//功能：					取得一个随机数
	//byRandomLength:			随机数长度，最大为16
	//abyRandom:				存放随机数的数组
	EXTERN_C int WINAPI DicPlus_Random(int hic, BYTE byRandomLength, BYTE abyRandom[]);

	//功能：					运行一个可执行文件
	//wRunFileNameID:			可执行文件的文件名表示的文件ID，注意不是可执行文件的文件ID，
	//							可执行文件的文件名必须是如下形式“6AB3”、“032B”，即一个用字串表示的WORD值
	//							例如可执行文件的文件名为“6AB3”，则该参数应传入0x6AB3
	//wParamSize:				传入启动时的参数大小
	//cpcParam:					传入启动参数
	//							启动参数是将各个输入参数连接成的参数，例如输入参数为一个8字节的double和一个2字节的WORD
	//							则cpcParam的前8个字节是这个输入的double值，后2个字节是输入的WORD值，wParamSize的大小为10
	//pwRetDataLength:			执行后的返回数据的大小
	//pcRetData:				执行后的返回数据(由于返回数据类型未知，所以需用户自己翻转成正常字节序)
	//							这个参数由程序执行后的返回数据连接而成，例如如果返回数据包括一个4字节的float和一个4字节的DWORD
	//							则该参数的前4个字节是这个返回的float值，后4个字节是DWORD值，*pwRetDataLength的大小为8
	int WINAPI DicPlus_Run(int hic, WORD wRunFileNameID, WORD wParamSize, const char *cpcParam, WORD *pwRetDataLength, char *pcRetData);

	//功能：					使用TEA算法生成一个密文文件存于pcEncData中，该算法要使用一个临时文件，该临时文件在这个函数执行完后可以调用DicPlus_DeleteFile删除
	//wFileID:					临时文件ID
	//byFileType:				临时文件类别
	//byFileAttribute:			临时文件属性
	//pwFileSize:				传入cpcFileData的大小，返回pcEncData的大小
	//byFileSecurity:			临时文件的安全级别
	//strFileName:				临时文件名
	//cpcFileData:				要加密的文件数据指针
	//pcEncData:				加密后的文件数据指针，该指针指向的*pwFileSize大小的加密后的数据可以通过网络等介质传到目的地，
	//							在目的地做为DicPlus_TeaEncryptedFile2PlainFile函数的第2个参数传入，这两个函数配合实现远程升级
	int WINAPI DicPlus_EncryptFileUseTea(int hic, 
		WORD wFileID, 
		BYTE byFileType, 
		BYTE byFileAttribute, 
		WORD *pwFileSize, 
		BYTE byFileSecurity, 
		const char strFileName[], 
		const char *cpcFileData,
		char *pcEncData);

	//功能：					与DicPlus_EncryptFileUseTea函数配合实现远程升级，将DicPlus_EncryptFileUseTea函数
	//							的pcEncData参数返回的加密数据在锁内写成一个明文文件，文件的ID就是DicPlus_EncryptFileUseTea函数中指定的临时文件ID
	//cmddata:					DicPlus_EncryptFileUseTea函数的pcEncData参数返回的加密数据
	int WINAPI DicPlus_TeaEncryptedFile2PlainFile(int hic, char *cmddata);

	//功能：					验证超级密码
	//abySuperPW:				超级密码，不能全为0，否则会被锁死
	EXTERN_C int WINAPI DicPlus_CheckSuperPassword(int hic, BYTE abySuperPW[]);

	//功能：					设置超级密码
	//byTryTimesCeiling:		验证超级密码时最多可以尝试的次数
	//abySuperPW:				超级密码，不能全为0，否则会被锁死
	EXTERN_C int WINAPI DicPlus_SetSuperPassword(int hic, BYTE byTryTimesCeiling, BYTE abySuperPW[]);

	//功能：					获取远程升级密码
	//dwRemoteFlag:				本次远程升级标志
	//dwHardSerial:				本次远程升级的硬件序列号
	//abyRemotePW:				输入本次远程升级的密码，返回下一次远程升级的密码
	EXTERN_C int WINAPI DicPlus_GetRemoteUpgradePassword(int hic, DWORD dwRemoteFlag, DWORD dwHardSerial, BYTE abyRemotePW[]);

	//							对于DES算法，加解密数据的长度必须是密钥长度8个字节的倍数

	//功能：					采用DES加密算法加密一段数据
	//wKeyFileID:				密钥文件ID
	//byKeyIndex:				密钥文件中的密钥索引，即密钥文件中的第多少个密钥
	//wDataLength:				要加密的数据长度
	//cpcDataSrc:				要加密的数据
	//pcDataEnc:				加密后的数据
	EXTERN_C int WINAPI DicPlus_DesEncrypt(int hic, WORD wKeyFileID, BYTE byKeyIndex, WORD wDataLength, const char *cpcDataSrc, char *pcDataEnc);

	//功能：					采用DES加密算法解密
	//wKeyFileID:				密钥文件ID
	//byKeyIndex:				密钥文件中的密钥索引，见DicPlus_GenerateDesKeyFile函数说明中有关密钥索引的说明
	//wDataLength:				要解密的数据长度
	//cpcDataSrc:				要解密的数据
	//pcDataDec:				解密后的数据
	EXTERN_C int WINAPI DicPlus_DesDecrypt(int hic, WORD wKeyFileID, BYTE byKeyIndex, WORD wDataLength, const char *cpcDataSrc, char *pcDataDec);

	//功能：					生成一个DES密钥文件，密钥长度只有8个字节和16个字节两种，如果是16个字节的密钥，加解密时会自动采用3DES算法
	//wKeyFileID:				生成的密钥文件ID
	//wKeyDataLength:			密钥数据的长度
	//pcKeyData:				密钥数据
	//							密钥数据是如下形式的数据："\x03\x10\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\
	//\x01\x08\x01\x02\x03\x04\x05\x06\x07\x08"	其中第一个\x03表示后面紧接的是索引为3的密钥，第二个\x10（16进制表示）表示这个密钥的长度是16个字节
	//							紧接的16个字节是这个索引为3的密钥。接下来的第19个字节\x01表示后面紧接的是索引为1的密钥，\x08表示这个密钥的长度是8个字节
	//							紧接的8个字节是这个索引为1的密钥。
	//							用户也可以通过调用DicPlus_WriteFile函数往该密钥文件中继续写入密钥。
	//							例如要先生成一个包含上述密钥数据的DES密钥文件，接着写入一条索引为2，长度为8个字节的密钥，代码如下：
	//							DicPlus_GenerateDesKeyFile(hic, 0x1003, 28, "\x03\x10\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0A\x0B\x0C\x0D\x0E\x0F\x10\
	//\x01\x08\x01\x02\x03\x04\x05\x06\x07\x08");
	//							DicPlus_SetCurrentFile(hic, 0x1003);
	//							DicPlus_WriteFile(hic, 28, 10, "\x02\x08\x01\x02\x03\x04\x05\x06\x07\x08");
	EXTERN_C int WINAPI DicPlus_GenerateDesKeyFile(int hic, WORD wKeyFileID, WORD wKeyDataLength, char *pcKeyData);

	//功能：					按指定的公私钥ID和公钥大小生成RSA公私钥文件，目前只支持512位和1024位两种密钥长度
	//wPublicKeyFileID:			公钥文件ID
	//wPublicKeySize:			公钥大小
	//wPrivateKeyFileID:		私钥文件ID
	EXTERN_C int WINAPI DicPlus_GenerateRsaKey(int hic, WORD wPublicKeyFileID, WORD wPublicKeySize, WORD wPrivateKeyFileID);

	//							对于RSA算法，下面的知识是有必要了解的：即对一段明文先用私钥解密再用公钥加密＝先用公钥加密再用私钥解密＝明文，
	//							加解密数据的长度必须是密钥长度512/8=64或1024/8=128字节的倍数

	//功能：					采用RSA加密算法加密一段数据
	//wPublicKeyFileID:			公钥文件ID
	//wDataLength:				加密的数据长度
	//cpcDataSrc:				加密的数据
	//pcDataEnc:				加密后的数据
	//bIsDataSrcFromOpenssl:	加密的数据是否来源于Openssl操作过的数据（因为Openssl的字节序需要在传入锁内前翻转）
	EXTERN_C int WINAPI DicPlus_RsaEncrypt(int hic, WORD wPublicKeyFileID, WORD wDataLength, const char *cpcDataSrc, char *pcDataEnc, BOOL bIsDataSrcFromOpenssl);


	//功能：					采用RSA解密算法解密一段数据
	//wPrivateKeyFileID:		私钥文件ID
	//wDataLength:				解密的数据长度
	//cpcDataSrc:				解密的数据
	//pcDataDec:				解密后的数据
	//bIsDataSrcFromOpenssl:	解密的数据是否来源于Openssl操作过的数据（因为Openssl的字节序需要在传入锁内前翻转）
	EXTERN_C int WINAPI DicPlus_RsaDecrypt(int hic, WORD wPrivateKeyFileID, WORD wDataLength, const char *cpcDataSrc, char *pcDataDec, BOOL bIsDataSrcFromOpenssl);

	//功能：					获取加密锁的剩余空间大小
	//pdwFreeSpace:				返回加密锁的剩余空间大小
	EXTERN_C int WINAPI DicPlus_GetFreeSpace(int hic, DWORD *pdwFreeSpace);

	//功能：					设置计数器初始值
	//dwInit:					计数器初始值
	EXTERN_C int WINAPI DicPlus_SetCounter(int hic, DWORD dwInit);

	//功能：					使计数器值减1
	EXTERN_C int WINAPI DicPlus_DecreaseCounter(int hic);

	//功能：					使用Openssl生成一个公钥文件
	//wPublicKeyFileID			公钥文件ID
	//wPublicKeySize			公钥大小
	//cpbyPublicKeyData			公钥数据，一般为用BN_bn2bin函数得到的RSA结构中的n
	EXTERN_C int WINAPI DicPlus_GenerateRsaPublicKeyFile(int hic, WORD wPublicKeyFileID, WORD wPublicKeySize/*512/1024*/, const BYTE *cpbyPublicKeyData);

	//功能：					使用Openssl生成一个私钥文件
	//wPrivateKeyFileID			私钥文件ID
	//wPrivateKeySize			私钥大小
	//cpbyPrivateKeyDataD		私钥数据D，一般为用BN_bn2bin函数得到的RSA结构中的d
	//cpbyPrivateKeyDataN		私钥数据N，一般为用BN_bn2bin函数得到的RSA结构中的n
	EXTERN_C int WINAPI DicPlus_GenerateRsaPrivateKeyFile(int hic, WORD wPrivateKeyFileID, WORD wPrivateKeySize/*512/1024*/, const BYTE *cpbyPrivateKeyDataD, const BYTE *cpbyPrivateKeyDataN);

#endif //__DIC32_H__
