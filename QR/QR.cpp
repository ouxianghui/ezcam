#include "QR.h"
#if HAVE_CONFIG_H
# include "config.h"
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "qrencode.h"

#define INCHES_PER_METER (100.0/2.54)
#define PIXEL_PRESCALER 8
#define PIXEL_COLOR_R				0   // Color of bmp pixels
#define PIXEL_COLOR_G				0
#define PIXEL_COLOR_B				0

/*
 * QImage::QImage(uchar * data, int width, int height, Format format, QImageCleanupFunction cleanupFunction = 0, void * cleanupInfo = 0)
 * Constructs an image with the given width, height and format, that uses an existing memory buffer, data.
 * The width and height must be specified in pixels, data must be 32-bit aligned,
 * and each scanline of data in the image must also be 32-bit aligned.
 * The buffer must remain valid throughout the life of the QImage
 * and all copies that have not been modified or otherwise detached from the original buffer.
 * The image does not delete the buffer at destruction.
 * You can provide a function pointer cleanupFunction along with an extra pointer cleanupInfo
 * that will be called when the last copy is destroyed.
 */
// 因为上述原因，定义了该函数以实现内存管理
void ImageCleanupHandler(void* data)
{
    if (data == NULL)
    {
        return;
    }
    free(data);
}

QR::QR(int nVersion,
       QRecLevel level,
       QRencodeMode hint,
       int nCaseSensitive,
       int nEightBit,
       int nMicro)
    : m_nVersion(nVersion),
      m_level(level),
      m_hint(hint),
      m_nCaseSensitive(nCaseSensitive),
      m_nEightBit(nEightBit),
      m_nMicro(nMicro)
{
}

QR::~QR()
{
}

void QR::setCaseSensitive(int nCaseSensitive)
{
    m_nCaseSensitive = nCaseSensitive;
}

void QR::setEightBit(int nEightBit)
{
    m_nEightBit = nEightBit;
}

void QR::setVersion(int nVersion)
{
    m_nVersion = nVersion;
}

void QR::setMicroQR(int nMicro)
{
    m_nMicro = nMicro;
}

void QR::setRecLevel(QRecLevel level)
{
    m_level = level;
}

void QR::setRencodeMode(QRencodeMode hint)
{
    m_hint = hint;
}

QRcode* QR::encode(const QString& strInput)
{
    QRcode* pCode = NULL;

    if(m_nMicro)
    {
        if(m_nEightBit)
        {
            pCode = QRcode_encodeDataMQR(strInput.toStdString().length(), (uchar*)strInput.toStdString().c_str(), m_nVersion, m_level);
        }
        else
        {
            pCode = QRcode_encodeStringMQR(strInput.toStdString().c_str(), m_nVersion, m_level, m_hint, m_nCaseSensitive);
        }
    }
    else
    {
        if(m_nEightBit)
        {
            pCode = QRcode_encodeData(strInput.toStdString().length(), (uchar*)strInput.toStdString().c_str(), m_nVersion, m_level);
        }
        else
        {
            pCode = QRcode_encodeString(strInput.toStdString().c_str(), m_nVersion, m_level, m_hint, m_nCaseSensitive);
        }
    }

    return pCode;
}

bool QR::qrEncode(const QString& strInput, QImage& image)
{
    QRcode* pCode = NULL;

    pCode = encode(strInput);
    if(pCode == NULL)
    {
        return false;
    }

    unsigned int unWidth = 0;
    unsigned int unWidthAdjusted = 0;
    unsigned int unDataBytes = 0;
    unsigned char* pRGB = NULL;
    unsigned char* pSrc = NULL;
    unsigned char* pDst = NULL;

    unWidth = pCode->width;
    unWidthAdjusted = unWidth * PIXEL_PRESCALER * 3;
    if (unWidthAdjusted % 4)
    {
        unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
    }

    unDataBytes = unWidthAdjusted * unWidth * PIXEL_PRESCALER;

    // Allocate pixels buffer
    if (!(pRGB = (unsigned char*)malloc(unDataBytes)))
    {
        printf("Out of memory");
        return false;
    }

    // Preset to white
    memset(pRGB, 0xff, unDataBytes);

    pSrc = pCode->data;
    for(unsigned int y = 0; y < unWidth; y++)
    {
        pDst = pRGB + unWidthAdjusted * y * PIXEL_PRESCALER;
        for(unsigned int x = 0; x < unWidth; x++)
        {
            if (*pSrc & 1)
            {
                for(unsigned int l = 0; l < PIXEL_PRESCALER; l++)
                {
                    for(unsigned int n = 0; n < PIXEL_PRESCALER; n++)
                    {
                        *(pDst + 0 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
                        *(pDst + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
                        *(pDst + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
                    }
                }
            }
            pDst += 3 * PIXEL_PRESCALER;
            pSrc++;
        }
    }

    image = QImage(pRGB, pCode->width*PIXEL_PRESCALER, pCode->width*PIXEL_PRESCALER, QImage::Format_RGB888, ImageCleanupHandler, pRGB);

    //free(pRGB);

    //main();

    QRcode_free(pCode);

    return true;
}


//#define QRCODE_TEXT               "http://www.ultramundum.org/index.htm";		// Text to encode into QRCode
//#define OUT_FILE					"I:/test.bmp"								// Output file name
//#define OUT_FILE_PIXEL_PRESCALER	8											// Prescaler (number of pixels in bmp file for each QRCode pixel, on each dimension)

//#define PIXEL_COLOR_R				0											// Color of bmp pixels
//#define PIXEL_COLOR_G				0
//#define PIXEL_COLOR_B				0xff

//// BMP defines

//typedef unsigned short	WORD;
//typedef unsigned long	DWORD;
//typedef signed long		LONG;

//#define BI_RGB			0L

//#pragma pack(push, 2)

//typedef struct
//{
//    WORD    bfType;
//    DWORD   bfSize;
//    WORD    bfReserved1;
//    WORD    bfReserved2;
//    DWORD   bfOffBits;
//} BITMAPFILEHEADER;

//typedef struct
//{
//    DWORD      biSize;
//    LONG       biWidth;
//    LONG       biHeight;
//    WORD       biPlanes;
//    WORD       biBitCount;
//    DWORD      biCompression;
//    DWORD      biSizeImage;
//    LONG       biXPelsPerMeter;
//    LONG       biYPelsPerMeter;
//    DWORD      biClrUsed;
//    DWORD      biClrImportant;
//} BITMAPINFOHEADER;

//#pragma pack(pop)

//int main()
//{
//    char* szSourceSring = QRCODE_TEXT;
//    unsigned int unWidth, x, y, l, n, unWidthAdjusted, unDataBytes;
//    unsigned char* pRGBData, *pSourceData, *pDestData;
//    QRcode*	pQRC;
//    FILE* f;

//    if (pQRC = QRcode_encodeString(szSourceSring, 0, QR_ECLEVEL_H, QR_MODE_8, 1))
//    {
//        unWidth = pQRC->width;
//        unWidthAdjusted = unWidth * OUT_FILE_PIXEL_PRESCALER * 3;
//        if (unWidthAdjusted % 4)
//            unWidthAdjusted = (unWidthAdjusted / 4 + 1) * 4;
//        unDataBytes = unWidthAdjusted * unWidth * OUT_FILE_PIXEL_PRESCALER;

//        // Allocate pixels buffer

//        if (!(pRGBData = (unsigned char*)malloc(unDataBytes)))
//        {
//            printf("Out of memory");
//            exit(-1);
//        }

//        // Preset to white

//        memset(pRGBData, 0xff, unDataBytes);


//        // Prepare bmp headers

//        BITMAPFILEHEADER kFileHeader;
//        kFileHeader.bfType = 0x4d42;  // "BM"
//        kFileHeader.bfSize = sizeof(BITMAPFILEHEADER)
//                + sizeof(BITMAPINFOHEADER)
//                + unDataBytes;
//        kFileHeader.bfReserved1 = 0;
//        kFileHeader.bfReserved2 = 0;
//        kFileHeader.bfOffBits =	sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

//        BITMAPINFOHEADER kInfoHeader;
//        kInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
//        kInfoHeader.biWidth = unWidth * OUT_FILE_PIXEL_PRESCALER;
//        kInfoHeader.biHeight = -((int)unWidth * OUT_FILE_PIXEL_PRESCALER);
//        kInfoHeader.biPlanes = 1;
//        kInfoHeader.biBitCount = 24;
//        kInfoHeader.biCompression = BI_RGB;
//        kInfoHeader.biSizeImage = 0;
//        kInfoHeader.biXPelsPerMeter = 0;
//        kInfoHeader.biYPelsPerMeter = 0;
//        kInfoHeader.biClrUsed = 0;
//        kInfoHeader.biClrImportant = 0;


//        // Convert QrCode bits to bmp pixels

//        pSourceData = pQRC->data;
//        for(y = 0; y < unWidth; y++)
//        {
//            pDestData = pRGBData + unWidthAdjusted * y * OUT_FILE_PIXEL_PRESCALER;
//            for(x = 0; x < unWidth; x++)
//            {
//                if (*pSourceData & 1)
//                {
//                    for(l = 0; l < OUT_FILE_PIXEL_PRESCALER; l++)
//                    {
//                        for(n = 0; n < OUT_FILE_PIXEL_PRESCALER; n++)
//                        {
//                            *(pDestData + 0 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_B;
//                            *(pDestData + 1 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_G;
//                            *(pDestData + 2 + n * 3 + unWidthAdjusted * l) = PIXEL_COLOR_R;
//                        }
//                    }
//                }
//                pDestData += 3 * OUT_FILE_PIXEL_PRESCALER;
//                pSourceData++;
//            }
//        }


//        // Output the bmp file

//        if (!(fopen_s(&f, OUT_FILE, "wb")))
//        {
//            fwrite(&kFileHeader, sizeof(BITMAPFILEHEADER), 1, f);
//            fwrite(&kInfoHeader, sizeof(BITMAPINFOHEADER), 1, f);
//            fwrite(pRGBData, sizeof(unsigned char), unDataBytes, f);

//            fclose(f);
//        }
//        else
//        {
//            printf("Unable to open file");
//            exit(-1);
//        }

//        // Free data

//        free(pRGBData);
//        QRcode_free(pQRC);
//    }
//    else
//    {
//        printf("NULL returned");
//        exit(-1);
//    }

//    return 0;
//}
////	-------------------------------------------------------

