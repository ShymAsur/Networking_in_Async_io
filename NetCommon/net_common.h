#pragma once
#include <thread>
#include <memory>
#include <mutex>
#include <deque>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>
#include <chrono>
#include <cstdint>

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif

#define ASIO_STANDALONE //to not include boost
#include <asio.hpp>
#include <asio/ts/buffer.hpp> //memory movement 
#include <asio/ts/internet.hpp> //internet