#ifndef _SYLAR_LOG_H_
#define _SYLAR_LOG_H_
#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

namespace sylar{

// 日志事件
class LogEvent{
public:
    typedef shared_ptr<LogEvent> ptr;
    LogEvent();
private:
    const char *m_file = nullptr;   //文件名
    int32_t m_line = 0;             // 行号
    uint32_t m_elapse = 0;          // 程序启动到现在的时间 毫秒
    uint32_t m_threadid = 0;        // 线程id
    uint32_t m_fiberid = 0;         // 协程id
    uint64_t m_time;                // 时间戳
    string m_content;
};

// 日志等级
class LogLevel{
public:
    enum Level{
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
private:
};

// 日志格式
class LogFormatter{
public:
    typedef shared_ptr<LogFormatter> ptr;
    LogFormatter(const string & pattern);

    // 根据pattern格式解析出日志 
    string format(LogEvent::ptr event);
private:
// 格式类型
    class FormatItem{
    public:
        typedef shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual void format(ostream &os,LogEvent::ptr event) = 0;
    };

    void init();
private:
    string m_pattern;
    vector<FormatItem::ptr> m_items;
};

// 日志输出地
class LogAppender{
public:
    typedef shared_ptr<LogAppender> ptr;
    virtual ~LogAppender(){}
    virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

    // set和getformater
    void setFormater(LogFormatter::ptr event){
        m_formatter = event;
    }
    LogFormatter::ptr getFormater()const{
        return m_formatter;
    }
protected:
    LogLevel::Level m_level;
    LogFormatter::ptr m_formatter;
};

// 日志器
class Logger{
public:
    typedef shared_ptr<Logger> ptr;
    Logger(const string& name = "root");
    void log(LogLevel::Level level, LogEvent::ptr event);

    void debug(LogEvent::ptr event);
    void info(LogEvent::ptr event);
    void warn(LogEvent::ptr event);
    void error(LogEvent::ptr event);
    void fatal(LogEvent::ptr event);

    // 添加和删除日志输出地
    void addAppender(LogAppender::ptr appender);
    void delAppender(LogAppender::ptr appender);

    // 设置和获取日志级别
    void setLevel(LogLevel::Level level){
        m_level = level;
    }
    LogLevel::Level getLevel() const {
        return m_level;
    }
private:
    string m_name;                      // 日志名称
    LogLevel::Level m_level;            // 日志级别
    list<LogAppender::ptr> m_appenders;  // appender集合
};

// 输出到控制台的Appender
class StdoutLogAppender
    : public LogAppender
    {
public:
    typedef shared_ptr<StdoutLogAppender> ptr;
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
private:

    };
// 定义输出到文件的Appender
class FileLogAppender
    :public LogAppender
    {
public:
    typedef shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const string &filename);
    virtual void log(LogLevel::Level level, LogEvent::ptr event) override;

    // 重新打开文件
    bool reopen();
private:
    string m_filename;//文件名
    ofstream m_filestream;//流
    };


}
#endif
