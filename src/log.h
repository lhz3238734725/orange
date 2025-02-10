#ifndef __ORANGE_LOG_H__
#define __ORANGE_LOG_H__

#include "singleton.h"
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
#include <map>

/**
 * @brief 使用流模式将日志级别为level的日志写入logger
 */
#define ORANGE_LOG_LEVEL(logger, level) \
    if(logger->getLevel() <= level) \
        orange::LogEventWarp(orange::LogEvent::ptr( \
            new orange::LogEvent(logger, level, __FILE__, __LINE__, \
            0, orange::GetThreadId(), orange::GetFiberId(), (uint64_t)time(0)))).getSS()

/**
 * @brief 使用流模式将日志级别为DEBUG的日志写入logger
 */
#define ORANGE_LOG_DEBUG(logger) ORANGE_LOG_LEVEL(logger, LogLevel::DEBUG)

/**
 * @brief 使用流模式将日志级别为INFO的日志写入logger
 */
#define ORANGE_LOG_INFO(logger) ORANGE_LOG_LEVEL(logger, LogLevel::INFO)

/**
 * @brief 使用流模式将日志级别为WARN的日志写入logger
 */
#define ORANGE_LOG_WARN(logger) ORANGE_LOG_LEVEL(logger, LogLevel::WARN)

/**
 * @brief 使用流模式将日志级别为ERROR的日志写入logger
 */
#define ORANGE_LOG_ERROR(logger) ORANGE_LOG_LEVEL(logger, LogLevel::ERROR)

/**
 * @brief 使用流模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FATAL(logger) ORANGE_LOG_LEVEL(logger, LogLevel::FATAL)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_LEVEL(logger, level, fmt, ...) \
    if(logger->getLevel() <= level) \
        orange::LogEventWarp(orange::LogEvent::ptr( \
            new orange::LogEvent(logger, level, __FILE__, __LINE__, \
            0, orange::GetThreadId(), orange::GetFiberId(), (uint64_t)time(0)))).getEvent()->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_DEBUG(logger, fmt, ...) ORANGE_LOG_FMT_LEVEL(logger, LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_INFO(logger, fmt, ...) ORANGE_LOG_FMT_LEVEL(logger, LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_WARN(logger, fmt, ...) ORANGE_LOG_FMT_LEVEL(logger, LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_ERROR(logger, fmt, ...) ORANGE_LOG_FMT_LEVEL(logger, LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化模式将日志级别为FATAL的日志写入logger
 */
#define ORANGE_LOG_FMT_FATAL(logger, fmt, ...) ORANGE_LOG_FMT_LEVEL(logger, LogLevel::FATAL, fmt, __VA_ARGS__)

namespace orange{

class Logger;

/*
* @brief 日志等级
*/
class LogLevel{
public:
    enum Level{
        UNKNOWN = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };

    static const char* toString(LogLevel::Level level);
};

/*
* @brief 日志事件
*/
class LogEvent
{
public:
    typedef std::shared_ptr<LogEvent> ptr;
    LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level,  const char* file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time);

    std::shared_ptr<Logger> getLogger() const { return m_logger; }
    LogLevel::Level getLevel() const { return m_level; }
    const char* getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint64_t getTime() const { return m_time; }
    std::string getContext() const { return m_ss.str(); }
    std::stringstream& getSS() { return m_ss; }

    /**
    * @brief 使用格式化模式将日志内容输入日志内容流
    */
    void format(const char* fmt, ...);
    void format(const char* fmt, va_list al);
private:
    const char* m_file = nullptr;       // 文件名
    int32_t m_line = 0;                 // 行号
    uint32_t m_elapse = 0;              // 程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;            // 线程id
    uint32_t m_fiberId = 0;             // 协程id
    uint64_t m_time;                    // 时间戳
    std::stringstream m_ss;              // 日志内容流
    std::shared_ptr<Logger> m_logger;   // 日志器
    LogLevel::Level m_level;            // 日志等级
};

/**
 * @brief 日志事件包装器
 */
class LogEventWarp
{
public:
    LogEventWarp(LogEvent::ptr event);
    ~LogEventWarp();

    LogEvent::ptr getEvent() const {return m_event;}
    std::stringstream& getSS();
private:
    // 日志事件
    LogEvent::ptr m_event;
};

/*
* @brief 日志格式器
*/
class LogFormater{
public:
    typedef std::shared_ptr<LogFormater> ptr;

    LogFormater(const std::string& pattern);

    /**
    * @brief 将event中内容按照 pattern 格式输入到 ostream 流中
    * @param[out] os 字符串输出流
    * @param[in] event 日志事件
    */
    std::string format(LogEvent::ptr event);

    /**
     * @brief 是否有错误
     */
    bool isError() const { return m_error;}

    const std::string getPattern() const{ return m_pattern;}
private:
        /**
        * @brief 初始化，按照 pattern 解析日志格式，将相应的 item 按照顺序放入 item vector中
        */
    void init();
public:
    class FormatItem{
    public:
        typedef std::shared_ptr<FormatItem> ptr;

        FormatItem(const std::string& str = "") {};
        virtual ~FormatItem() = default;

        /**
        * @brief 将event中各项内容按照 item规则解析到 ostream 流中
        * @param[out] os 字符串输出流
        * @param[in] event 日志事件
        */
        virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
    };

private:
    // 日志格式
    std::string m_pattern;
    // 日志解析item vector
    std::vector<FormatItem::ptr> m_items;
    // 是否有错误
    bool m_error = false;
};

/*
* @brief 日志输出地
*/
class LogAppender{
public:
    typedef std::shared_ptr<LogAppender> ptr;

    virtual ~LogAppender() = default;
    
    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

    LogFormater::ptr getFormater() const;
    void setFormater(LogFormater::ptr formater);

    LogLevel::Level getLevel() const { return m_level;}
    void setLevel(LogLevel::Level level) { m_level = level;}
protected:
    // 日志等级
    LogLevel::Level m_level = LogLevel::DEBUG;
    // 日志格式器
    LogFormater::ptr m_formater;
};

/*
* @brief 输出到控制台的Appender
*/ 
class StdoutLogAppneder : public LogAppender{
public:
    typedef std::shared_ptr<StdoutLogAppneder> ptr;

    void log(LogLevel::Level level, LogEvent::ptr event) override;
};

/*
* @brief 输出到文件的Appender
*/
class FileLogAppneder : public LogAppender{
public:
    typedef std::shared_ptr<FileLogAppneder> ptr;

    FileLogAppneder(const std::string& filename);

    void log(LogLevel::Level level, LogEvent::ptr event) override;

    /*
    * @brief 重新打开文件，文件打开成功返回true
    */
    bool reopen();
private:
    // 文件名
    std::string m_filename;
    // 文件流
    std::ofstream m_filestream;
};

/*
* @brief 日志器
*/
class Logger{
public:
    typedef std::shared_ptr<Logger> ptr;
    
    Logger(const std::string& name = "root");

    void log(LogLevel::Level level, LogEvent::ptr event);
    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level level) { m_level = level; }

    std::string getName() const { return m_name; };
private:
    std::string m_name;                     // 日志名称
    LogLevel::Level m_level;                // 日志级别
    std::list<LogAppender::ptr> m_appenders;// Appender集合 
    LogFormater::ptr m_formatter;           // 默认的格式器
};

/**
 * @brief 日志器管理器
 */
class LoggerManager
{
public:
    LoggerManager();
    Logger::ptr getLogger(const std::string& name);
    void init();
private:
    // 日志器的存储结构
    std::map<std::string, Logger::ptr> m_loggers;
    // 默认的日志器
    Logger::ptr m_root;
};

typedef Singleton<LoggerManager> LogMgrPtr;
 
}

#endif