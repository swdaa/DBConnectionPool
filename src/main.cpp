

#include "Connection.h"
#include "pch.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    Connection conn;
    char sql[1024] = {0};
    sprintf(sql, "insert into user(name, age) values('%s', %d);", "zhangsan", 20);
    conn.connect("127.0.0.1", 3306, "testuser", "testpass", "testdb");
    conn.update(sql);
    return 0;
}