#pragma once

#include <iostream>

using namespace std;

#define LOG(str)                                                                                                       \
    do                                                                                                                 \
    {                                                                                                                  \
        std::cout << __FILE__ << ":" << __LINE__ << " " << __TIME__ << " " << str << std::endl;                        \
    } while (0)