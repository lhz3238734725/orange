#include<iostream>
#include<thread>
#include "src/log.h"
#include "src/util.h"

using namespace orange;

int main(int argc, char** argv){

    Logger::ptr logger(new Logger());
    logger->addAppender(LogAppender::ptr(new StdoutLogAppneder()));

    Logger::ptr fileLogger(new Logger());
    fileLogger->addAppender(LogAppender::ptr(new FileLogAppneder("./log.txt")));

    // LogEvent::ptr event(new LogEvent(logger, LogLevel::INFO, __FILE__, __LINE__, 0, orange::GetThreadId(), orange::GetFiberId(), (uint64_t)time(0)));
    // event->getSS() << "Hello orange";
    // logger->log(LogLevel::DEBUG, event);
    logger->setLevel(LogLevel::ERROR);
    std::cout << "[Test Log]" << std::endl;
    std::cout << "1.Test stream Log" << std::endl;
    ORANGE_LOG_DEBUG(logger) << "DEBUG Hello orange Success";
    ORANGE_LOG_INFO(logger) << "INFO Hello orange Success";
    ORANGE_LOG_WARN(logger) << "WARN Hello orange Success";
    ORANGE_LOG_ERROR(logger) << "ERROR Hello orange Success";
    ORANGE_LOG_FATAL(fileLogger) << "FATAL Hello orange Success";
    std::cout << "2.Test format Log" << std::endl;
    ORANGE_LOG_FMT_DEBUG(logger, "DEBUG %d Hello orange %s", 1, "Success");
    ORANGE_LOG_FMT_INFO(logger, "INFO %d Hello orange %s", 2, "Success");
    ORANGE_LOG_FMT_WARN(logger, "WARN %d Hello orange %s", 3, "Success");
    ORANGE_LOG_FMT_ERROR(logger, "ERROR %d Hello orange %s", 4, "Success");
    ORANGE_LOG_FMT_FATAL(fileLogger, "FATAL %d Hello orange %s", 5, "Success");

    auto l = LogMgrPtr::GetInstance()->getLogger("xx");
    ORANGE_LOG_FATAL(l) << "Test LoggerManager Success";

    return 0;
}