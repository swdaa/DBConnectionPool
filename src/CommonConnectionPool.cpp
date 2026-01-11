
#include "CommonConnectionPool.h"
#include "pch.h"
#include <condition_variable>
#include <thread>

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

    for (int i = 0; i < _initSize; ++i)
    {
        Connection *p = new Connection();
        p->connect(_ip, _port, _user, _password, _dbname);
        p->refreshAliveTime();

        _connectionQue.push(p);
        ++_connectionCnt;
    }

    // generate a thread as connection producer

    thread produce(std::bind(&ConnectionPool::produceConnectionTask, this));
    produce.detach();
    thread scanner(std::bind(&ConnectionPool::scannerConnectionTask, this));
    scanner.detach();
}

void ConnectionPool::produceConnectionTask()
{
    for (;;)
    {
        unique_lock<mutex> lock(_queueMutex);
        while (!_connectionQue.empty())
        {
            _cv.wait(lock); // realease lock and wait to be notified
        }

        if (_connectionCnt < _maxSize)
        {
            Connection *p = new Connection();
            p->connect(_ip, _port, _user, _password, _dbname);
            _connectionQue.push(p);
            ++_connectionCnt;
        }
        // notify all waiting threads that a new connection is available
        _cv.notify_all();
    } // realease lock when out of scope
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

std::shared_ptr<Connection> ConnectionPool::getConnection()
{
    unique_lock<mutex> lock(_queueMutex);

    while (_connectionQue.empty())
    {
        if (_connectionQue.empty())
        {
            // wait for a connection to be available
            _cv.wait_for(lock, chrono::milliseconds(_connectionTimeout));

            if (cv_status::timeout == _cv.wait_for(lock, chrono::milliseconds(_connectionTimeout)))
            {

                if (_connectionQue.empty())
                {
                    LOG("Get connection timeout");
                    return nullptr;
                }
            }
        }
    }

    shared_ptr<Connection> sp(_connectionQue.front(),
                              [&](Connection *pcon)
                              {
                                  unique_lock<mutex> lock(_queueMutex);
                                  pcon->refreshAliveTime();
                                  _connectionQue.push(pcon);
                              });
    _connectionQue.pop();
    if (_connectionQue.empty())
    {
        _cv.notify_all(); // if consume the last connection, notify producer
    }
    return sp;
}

void ConnectionPool::scannerConnectionTask()
{
    for (;;)
    {
        // sleep for 5000 milliseconds
        this_thread::sleep_for(chrono::milliseconds(5000));

        unique_lock<mutex> lock(_queueMutex);
        while (_connectionCnt > _initSize)
        {
            Connection *p = _connectionQue.front();
            if (p->getAliveTime() >= (_maxIdelTime * 1000))
            {
                _connectionQue.pop();
                --_connectionCnt;
                delete p;
            }
            else
            {
                break; // since connections are ordered by time, we can break here
            }
        }
    }
}