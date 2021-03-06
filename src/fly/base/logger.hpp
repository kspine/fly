/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                    _______    _                                     *
 *                   (  ____ \  ( \     |\     /|                      * 
 *                   | (    \/  | (     ( \   / )                      *
 *                   | (__      | |      \ (_) /                       *
 *                   |  __)     | |       \   /                        *
 *                   | (        | |        ) (                         *
 *                   | )        | (____/\  | |                         *
 *                   |/         (_______/  \_/                         *
 *                                                                     *
 *                                                                     *
 *     fly is an awesome c++11 network library.                        *
 *                                                                     *
 *   @author: lichuan                                                  *
 *   @qq: 308831759                                                    *
 *   @email: lichuan@lichuan.me                                        *
 *   @github: https://github.com/lichuan/fly                           *
 *   @date: 2015-06-10 17:27:54                                        *
 *                                                                     *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef FLY__BASE__LOGGER
#define FLY__BASE__LOGGER

#include <cstdio>
#include <atomic>
#include <string>
#include <mutex>
#include <sys/time.h>
#include "fly/base/common.hpp"
#include "fly/base/singleton.hpp"

namespace fly {
namespace base {

enum LOG_LEVEL
{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL
};

class Logger : public Singleton<Logger>
{
public:
    void init(LOG_LEVEL level, const std::string &app, const std::string &path);
    void _log(uint32 year, uint32 month, uint32 day, const char *format, ...);
    
    bool _enter(LOG_LEVEL level)
    {
        if(level < m_level)
        {
            return false;
        }
        
        m_enter_num.fetch_add(1, std::memory_order_relaxed);

        return true;
    }
    
private:
    LOG_LEVEL m_level;
    std::string m_file_name;
    std::string m_file_full_name;
    FILE *m_file;
    std::atomic<uint32> m_year;
    std::atomic<uint32> m_month;
    std::atomic<uint32> m_day;
    std::atomic<uint32> m_enter_num {0};
    std::mutex m_mutex;
};

}
}

#define LOG_DEBUG(format, ...) \
    if(fly::base::Logger::instance()->_enter(fly::base::DEBUG)) \
    { \
        char _format[1024]; \
        struct timeval _tv; \
        gettimeofday(&_tv, NULL); \
        struct tm _tm; \
        localtime_r(&_tv.tv_sec, &_tm); \
        sprintf(_format, "%d-%02d-%02d %02d:%02d:%02d.%06ld |debug| %s:%d {%s}\n", 1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, \
                _tm.tm_min, _tm.tm_sec, _tv.tv_usec, __FILE__, __LINE__, format); \
        fly::base::Logger::instance()->_log(1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _format, ##__VA_ARGS__); \
    }

#define LOG_INFO(format, ...) \
    if(fly::base::Logger::instance()->_enter(fly::base::INFO)) \
    { \
        char _format[1024]; \
        struct timeval _tv; \
        gettimeofday(&_tv, NULL); \
        struct tm _tm; \
        localtime_r(&_tv.tv_sec, &_tm); \
        sprintf(_format, "%d-%02d-%02d %02d:%02d:%02d.%06ld |info| %s:%d {%s}\n", 1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, \
                _tm.tm_min, _tm.tm_sec, _tv.tv_usec, __FILE__, __LINE__, format); \
        fly::base::Logger::instance()->_log(1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _format, ##__VA_ARGS__); \
    }

#define LOG_WARN(format, ...) \
    if(fly::base::Logger::instance()->_enter(fly::base::WARN)) \
    { \
        char _format[1024]; \
        struct timeval _tv; \
        gettimeofday(&_tv, NULL); \
        struct tm _tm; \
        localtime_r(&_tv.tv_sec, &_tm); \
        sprintf(_format, "%d-%02d-%02d %02d:%02d:%02d.%06ld |warn| %s:%d {%s}\n", 1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, \
                _tm.tm_min, _tm.tm_sec, _tv.tv_usec, __FILE__, __LINE__, format); \
        fly::base::Logger::instance()->_log(1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _format, ##__VA_ARGS__); \
    }

#define LOG_ERROR(format, ...) \
    if(fly::base::Logger::instance()->_enter(fly::base::ERROR)) \
    { \
        char _format[1024]; \
        struct timeval _tv; \
        gettimeofday(&_tv, NULL); \
        struct tm _tm; \
        localtime_r(&_tv.tv_sec, &_tm); \
        sprintf(_format, "%d-%02d-%02d %02d:%02d:%02d.%06ld |error| %s:%d {%s}\n", 1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, \
                _tm.tm_min, _tm.tm_sec, _tv.tv_usec, __FILE__, __LINE__, format); \
        fly::base::Logger::instance()->_log(1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _format, ##__VA_ARGS__); \
    }

#define LOG_FATAL(format, ...) \
    if(fly::base::Logger::instance()->_enter(fly::base::FATAL)) \
    { \
        char _format[1024]; \
        struct timeval _tv; \
        gettimeofday(&_tv, NULL); \
        struct tm _tm; \
        localtime_r(&_tv.tv_sec, &_tm); \
        sprintf(_format, "%d-%02d-%02d %02d:%02d:%02d.%06ld |fatal| %s:%d {%s}\n", 1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _tm.tm_hour, \
                _tm.tm_min, _tm.tm_sec, _tv.tv_usec, __FILE__, __LINE__, format); \
        fly::base::Logger::instance()->_log(1900 + _tm.tm_year, 1 + _tm.tm_mon, _tm.tm_mday, _format, ##__VA_ARGS__); \
    }

#endif
