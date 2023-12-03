#pragma once
#include "Math/ToolBoxMathHeaders.h"
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <vector>
#include <array>
#include <utility>
#include <functional>
#include <limits>
#include <typeinfo>


#include <queue>
#include <map>
#include <set>
#include <optional>

#define WHITE "\x1b[0m"
#define BLACK "\x1b[30m"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define PURPLE "\x1b[35m"	


#define CORE_LOG(message)(std::cout << WHITE << message)
#define CORE_LOG_SUCCESS(message)(std::cout << GREEN << message << WHITE )
#define CORE_LOG_ERROR(message)(std::cout << RED << message << WHITE)