#include "CommonConnectionPool.h"
#include "pch.h"

ConnectionPool *ConnectionPool::getInstance()
{
    static ConnectionPool pool; // coming auto lock and unlock in C++11
    return &pool;
}

ConnectionPool::ConnectionPool()
{
    // if (!loadConfigFile())
    // {
    //     LOG("Failed to load configuration file");
    //     return;
    // }

    for(int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _port, _user, _password, _dbname);
        _connectionQue.push(p);
        ++_connectionCnt;
    }

    // generate a thread as connection producer

    


}

bool ConnectionPool::loadConfigFile()
{
    FILE *pf = fopen("dq.conf", "r");
    if (pf == nullptr)
    {
        LOG("dq.conf file is not exist!");
        return false;
    }

    while (!feof(pf))
    {
        char line[1024] = {0};
        fgets(line, 1024, pf);
        string str = line;
        int pos = str.find('=');
        if (pos == -1)
            continue;
        string key = str.substr(0, pos);
        string value = str.substr(pos + 1, str.size() - pos - 2);

        if (key == "ip")
            _ip = value;
        else if (key == "port")
            _port = atoi(value.c_str());
        else if (key == "username")
            _user = value;
        else if (key == "password")
            _password = value;
        else if (key == "dbname")
            _dbname = value;
        else if (key == "initSize")
            _initSize = atoi(value.c_str());
        else if (key == "maxSize")
            _maxSize = atoi(value.c_str());
        else if (key == "maxIdleTime")
            _maxIdelTime = atoi(value.c_str());
        else if (key == "connectionTimeout")
            _connectionTimeout = atoi(value.c_str());
    }
    return true;
}
