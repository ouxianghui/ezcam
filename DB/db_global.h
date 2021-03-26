#ifndef DB_GLOBAL_H
#define DB_GLOBAL_H

#include <QtCore/qglobal.h>
#include <string>
#include <QSqlError>

#if defined(DB_LIBRARY)
#  define DBSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DBSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DB_GLOBAL_H
