#ifndef QR_H
#define QR_H

#include "qr_global.h"
#include <QImage>
#include "qrencode.h"

class QRSHARED_EXPORT QR
{

public:
    QR(int nVersion = 0,
       QRecLevel level = QR_ECLEVEL_L,
       QRencodeMode hint = QR_MODE_8,
       int nCaseSensitive = 1,
       int nEightBit = 0,
       int nMicro = 0);

    ~QR();

    bool qrEncode(const QString& strInput, QImage& image);

    // @param casesensitive case-sensitive(1) or not(0).
    void setCaseSensitive(int nCaseSensitive);

    // @param eight-bit(1) or not(0).
    void setEightBit(int nEightBit);

    void setVersion(int nVersion);

    // @param Micro-QR(1) or not(0).
    void setMicroQR(int nMicro);

    void setRecLevel(QRecLevel level);

    void setRencodeMode(QRencodeMode hint);

protected:
    QRcode* encode(const QString& strInput);

private:

    int m_nVersion;
    QRecLevel m_level;
    QRencodeMode m_hint;
    int m_nCaseSensitive;
    int m_nEightBit;
    int m_nMicro;
};

#endif // QR_H
