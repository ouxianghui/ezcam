#ifndef QR_GLOBAL_H
#define QR_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(GQR_LIBRARY)
#  define QRSHARED_EXPORT Q_DECL_EXPORT
#else
#  define QRSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // QR_GLOBAL_H
