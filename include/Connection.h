#pragma once

#include <mysql/mysql.h>
#include <string>
using namespace std;
#include "public.h"

class Connection
{
  public:
    Connection() { _conn = mysql_init(nullptr); }
    ~Connection()
    {
        if (_conn != nullptr)
            mysql_close(_conn);
    }
    bool connect(string ip, unsigned short port, string user, string password, string dbname)
    {
        MYSQL *p =
            mysql_real_connect(_conn, ip.c_str(), user.c_str(), password.c_str(), dbname.c_str(), port, nullptr, 0);
        return p != nullptr;
    }
    bool update(string sql)
    {
        if (mysql_query(_conn, sql.c_str()))
        {
            LOG("Update failed:" + sql);
            return false;
        }
        return true;
    }
    
    MYSQL_RES *query(string sql)
    {
        if (mysql_query(_conn, sql.c_str()))
        {
            LOG("Query failed:" + sql);
            return nullptr;
        }
        return mysql_use_result(_conn);
    }

  private:
    MYSQL *_conn; // one connection to MySQL server
};