#pragma once

#include <mutex>
#include <deque>
#include <string>
#include <pthread.h>
#include <thread>
#include <memory>
#include <vector>
#include <atomic>
#include <iostream>
#include <algorithm>
#include <functional>
#include <condition_variable>
#include <wiringPi.h>
#include <mysql/mysql.h>
#include <mysql++/mysql++.h>

using std::cout; 
using std::endl;
using std::cin;
using std::thread;
using std::vector;
using std::unique_ptr;
using std::shared_ptr;
using std::atomic;
using std::string;