#ifndef DB_H
#define DB_H

#include "db_global.h"

class DBSHARED_EXPORT DB
{

public:
    DB();
    static bool createDatabase();

private:
    static std::string errorMsg(const QSqlError& error);
};

#endif // DB_H
