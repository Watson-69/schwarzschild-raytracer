// log.h — Simple logging utility.
#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <mutex>

namespace bh::log {

enum class Level { Debug, Info, Warn, Error };

inline std::mutex logMutex;
inline Level currentLevel = Level::Info;

inline void setLevel(Level l) noexcept { currentLevel = l; }

template<typename... Args>
void log(Level l, const char* fmt, Args&&... args) {
    if (l < currentLevel) return;
    std::lock_guard<std::mutex> lock(logMutex);
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);
    const char* prefix = l == Level::Debug ? "[DBG]" :
                         l == Level::Info  ? "[INF]" :
                         l == Level::Warn  ? "[WRN]" : "[ERR]";
    std::cout << prefix << " " << std::ctime(&t);
    std::cout << "  ";
    ((std::cout << args), ...);
    std::cout << "\n";
}

#define LOG_DEBUG(...) bh::log::log(bh::log::Level::Debug, __VA_ARGS__)
#define LOG_INFO(...)  bh::log::log(bh::log::Level::Info,  __VA_ARGS__)
#define LOG_WARN(...)  bh::log::log(bh::log::Level::Warn,  __VA_ARGS__)
#define LOG_ERROR(...) bh::log::log(bh::log::Level::Error, __VA_ARGS__)

} // namespace bh::log