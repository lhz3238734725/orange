#include "log.h"
#include <iostream>
#include <map>
#include <functional>
#include <time.h>
#include <stdarg.h>

namespace orange{

const char* LogLevel::toString(LogLevel::Level level){
    switch (level)
    {
#define XX(name) \
    case LogLevel::name : \
    return #name

    XX(DEBUG);
    XX(INFO);
    XX(WARN);
    XX(ERROR);
    XX(FATAL);
#undef XX
    default:
        return "UNKNOWN";
    }

    return "UNKNOWN";    
}

LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char* file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint64_t time)
    : m_file(file) 
    , m_line(line) 
    , m_elapse(elapse) 
    , m_threadId(threadId)
    , m_fiberId(fiberId) 
    , m_time(time)
    , m_logger(logger)
    , m_level(level)  {
}

void LogEvent::format(const char* fmt, ...){
    va_list al;
    va_start(al ,fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al){
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);     // 自动生成buf内存空间，需要手动释放
    if(len != -1){
        m_ss << buf;
        free(buf);
    }
}

LogEventWarp::LogEventWarp(LogEvent::ptr event) 
    : m_event(event) {

}

LogEventWarp::~LogEventWarp(){
    m_event->getLogger()->log(m_event->getLevel(), m_event);
}

std::stringstream& LogEventWarp::getSS() {
    return m_event->getSS();
}

class MessageFormatItem : public LogFormater::FormatItem{
public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getContext();
    }
};

class LevelFormatItem : public LogFormater::FormatItem{
public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << LogLevel::toString(event->getLevel());
    }
};

class ElapseFormatItem : public LogFormater::FormatItem{
public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

class NameFormatItem : public LogFormater::FormatItem{
public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getLogger()->getName();
    }
};

class ThreadIdFormatItem : public LogFormater::FormatItem{
public:
    ThreadIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

class FiberIdFormatItem : public LogFormater::FormatItem{
public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getFiberId();
    }
};

class DateTimeFormatItem : public LogFormater::FormatItem{
public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
        : m_format(format) {
            if(format.empty()) m_format = "%Y-%m-%d %H:%M:%S";
        }
    void format(std::ostream& os, LogEvent::ptr event) override {
        time_t rawtime = static_cast<time_t>(event->getTime());
        struct tm timeinfo;
        localtime_r(&rawtime, &timeinfo); // 该函数是线程安全的
        char buffer[60];
        strftime(buffer, sizeof(buffer), m_format.c_str(), &timeinfo);
        os << buffer;
    }
private:
    std::string m_format;
};

class FilenameFormatItem : public LogFormater::FormatItem{
public:
    FilenameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

class LineFormatItem : public LogFormater::FormatItem{
public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getLine();
    }
};

class NewLineFormatItem : public LogFormater::FormatItem{
public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << std::endl;
    }
};

class StringFormatItem : public LogFormater::FormatItem{
public:
    StringFormatItem(const std::string& str = "")
        :m_content(str) {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << m_content;
    }
private:
    std::string m_content;
};

class TabFormatItem : public LogFormater::FormatItem{
public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << "\t";
    }
};

Logger::Logger(const std::string& name)
    :m_name(name)
    ,m_level(LogLevel::DEBUG) {
    m_formatter.reset(new LogFormater("[%c][%p][%d{%Y-%m-%d %H:%M:%S}][%f][%l][%t][%F]%T%m%n"));
}

void Logger::log(LogLevel::Level level, LogEvent::ptr event) {
    if(m_level <= level){
        for(auto& item : m_appenders){
            item->log(level, event);
        }
    }
}

void Logger::debug(LogEvent::ptr event) {
    log(LogLevel::Level::DEBUG, event);
}

void Logger::info(LogEvent::ptr event) {
    log(LogLevel::Level::INFO, event);
}

void Logger::warn(LogEvent::ptr event) {
    log(LogLevel::Level::WARN, event);
}

void Logger::error(LogEvent::ptr event) {
    log(LogLevel::Level::ERROR, event);
}

void Logger::fatal(LogEvent::ptr event) {
    log(LogLevel::Level::FATAL, event);
}

void Logger::addAppender(LogAppender::ptr appender) {
    if(!appender->getFormater()){
        appender->setFormater(m_formatter);
    }
    m_appenders.push_back(appender);
}

void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it){
        if(*it == appender){
            m_appenders.erase(it);
            break;
        }
    }
}

LogFormater::ptr LogAppender::getFormater() const {
    return m_formater;
}

void LogAppender::setFormater(LogFormater::ptr formater) {
    m_formater = formater;
}

void StdoutLogAppneder::log(LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {
        std::cout << m_formater->format(event);
    }
}

FileLogAppneder::FileLogAppneder(const std::string& filename)
    : m_filename(filename) {
        reopen();
}

bool FileLogAppneder::reopen() {
    if(m_filestream) {
        m_filestream.close();
    }
    m_filestream.open(m_filename);

    return !!m_filestream;
}

void FileLogAppneder::log(LogLevel::Level level, LogEvent::ptr event) {
    if(level >= m_level) {
        m_filestream << m_formater->format(event);
    }
}

LogFormater::LogFormater(const std::string& pattern)
    :m_pattern(pattern) {
    init();
}

std::string LogFormater::format(LogEvent::ptr event) {
    std::stringstream ss;
    for (auto& item : m_items)
    {
        item->format(ss, event);
    }
    return ss.str();
}

// %XXX %XXX{XXX} %% --> datetime%%:%d{YY MM DD HH SS SS}
void LogFormater::init() {
    std::vector<std::tuple<std::string, std::string, int>> vec;
    std::string nstr;

    for(size_t i = 0; i < m_pattern.size(); ++i){

        // 正常的字符
        if(m_pattern[i] != '%'){
            nstr.append(1, m_pattern[i]);
            continue;
        }

        // %%
        if((i + 1) < m_pattern.size()){
            if(m_pattern[i+1] == '%') {
                nstr.append(1, '%');
                ++i;
                continue;
            }
        }

        // %XXX %XXX{XXX}
        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;
        std::string str;    // % 和 { 中间的字符串
        std::string fmt;    // { 和 } 中间的字符串

        while (n < m_pattern.size())
        {
            if(!fmt_status && !isalpha(m_pattern[n]) && m_pattern[n] != '{' && m_pattern[n] != '}'){
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }

            if(fmt_status == 0){
                if(m_pattern[n] == '{'){
                    str = m_pattern.substr(i + 1, n - i - 1);
                    ++n;
                    fmt_begin = n;
                    fmt_status = 1;
                    continue;
                }
            }else if(fmt_status == 1){
                if(m_pattern[n] == '}'){
                    fmt = m_pattern.substr(fmt_begin, n - fmt_begin);
                    fmt_status = 0;
                    ++n;
                    break;
                }
            }

            ++n;
            if(n == m_pattern.size()){
                if(str.empty()){
                    str = m_pattern.substr(i + 1);
                }
            }
        }
        
        if(fmt_status == 0){
            if(!nstr.empty()){
                vec.push_back(std::make_tuple(nstr, "", 0));
                nstr.clear();
            }
            vec.push_back(std::make_tuple(str, fmt, 1));
        }else if(fmt_status == 1){
            std::cout << "[pattern parse error]\n" 
                      << "current pattern: " << m_pattern << "\n" 
                      << "error pattern  : " << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt , 0));
            m_error = true;
        }

        i = n - 1;
    }

    if(!nstr.empty()){
        vec.push_back(std::make_tuple(nstr, "", 0));
    }

#define XX(str, C) { #str, [](const std::string& fmt) {return FormatItem::ptr(new C(fmt)); }}
    static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
        XX(m, MessageFormatItem),
        XX(p, LevelFormatItem),
        XX(r, ElapseFormatItem),
        XX(c, NameFormatItem),
        XX(t, ThreadIdFormatItem),
        XX(n, NewLineFormatItem),
        XX(d, DateTimeFormatItem),
        XX(f, FilenameFormatItem),
        XX(l, LineFormatItem),
        XX(F, FiberIdFormatItem),
        XX(T, TabFormatItem)
    };
#undef XX

    for(auto& item : vec){
        if(std::get<2>(item) == 0){
            m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(item))));
        }else{
            auto it = s_format_items.find(std::get<0>(item));
            if(it == s_format_items.end()){
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(item) + ">>")));
                m_error = true;
            }else{
                m_items.push_back(it->second(std::get<1>(item)));
            }
        }
    }
}

LoggerManager::LoggerManager() {
    m_root.reset(new Logger());
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppneder));
}

Logger::ptr LoggerManager::getLogger(const std::string& name){
    auto it = m_loggers.find(name);
    return it == m_loggers.end() ? m_root : it->second;
}

void LoggerManager::init(){

}

}