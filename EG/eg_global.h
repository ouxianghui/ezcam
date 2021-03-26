#ifndef EG_GLOBAL_H
#define EG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(EG_LIBRARY)
#  define EGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define EGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // EG_GLOBAL_H
