#ifndef CA_GLOBAL_H
#define CA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CA_LIBRARY)
#  define CASHARED_EXPORT Q_DECL_EXPORT
#else
#  define CASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CA_GLOBAL_H
