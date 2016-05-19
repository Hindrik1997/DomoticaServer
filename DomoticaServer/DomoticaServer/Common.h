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

#include "Task.h"
#include "ITask.h"
#include "Threadpool.h"

#include <mysql/mysql.h>
#include <mysql++/mysql++.h>