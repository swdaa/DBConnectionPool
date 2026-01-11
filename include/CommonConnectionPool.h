#pragma once

#include "Connection.h"
#include <mutex>
#include <queue>
#include <string>
#include <atomic>
#include <memory>
#include <functional>
#include<condition_variable>

class ConnectionPool
{
  public:
    static ConnectionPool *getInstance();
    std::shared_ptr<Connection> getConnection(); // get connection from pool 

  private:
    ConnectionPool();

    bool loadConfigFile(); // load configuration from file
    void produceConnectionTask(); // produce new connections
    void scannerConnectionTask(); 
    


    // configure parameters
    string _ip{"127.0.0.1"};
    unsigned short _port{3306};
    string _user{"testuser"};
    string _password{"testpass"};
    string _dbname{"testdb"};
    int _initSize{10};          // initial size of connection pool
    int _maxSize{10};           // max size of connection pool
    int _maxIdelTime{10};       // max idle time of connections
    int _connectionTimeout{10}; // connection timeout

    queue<Connection *> _connectionQue; // connection queue
    mutex _queueMutex;                  // mutex for connection queue
    atomic_int _connectionCnt; // current connection count
    condition_variable _cv;   // condition variable for connection pool

};