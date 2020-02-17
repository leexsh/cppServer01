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
#include <map>
using namespace std;

namespace sylar{

class Logger;
class LogFormatter;
// 日志事件
class LogEvent{
public:
    typedef shared_ptr<LogEvent> ptr;
    LogEvent();

    // get方法
    const char* getFile()const{return m_file;}
    int32_t getLine()const{return m_line;}
    uint32_t getElapse()const{return m_elapse;}
    uint32_t getThreadId()const{return m_threadid;}
    uint32_t getFiberId()const{return m_fiberid;}
    uint64_t getTime()const{return m_time;}
    const string getContent()const{return m_content;}

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
        UNKNOW = 0,
        DEBUG = 1,
        INFO = 2,
        WARN = 3,
        ERROR = 4,
        FATAL = 5
    };
    static const char* ToString(LogLevel::Level level);
private:
};


// 日志格式
class LogFormatter{
public:
    typedef shared_ptr<LogFormatter> ptr;
    LogFormatter(const string & pattern);

    // 根据pattern格式解析出日志 
    string format(shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event);
public:
// 格式类型
    class FormatItem{
    public:
        typedef shared_ptr<FormatItem> ptr;
        virtual ~FormatItem(){}
        virtual void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event) = 0;
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
    virtual void log(shared_ptr<Logger> pLogger,LogLevel::Level level, LogEvent::ptr event) = 0;

    // set和getformater
    void setFormater(LogFormatter::ptr event){
        m_formatter = event;
    }
    LogFormatter::ptr getFormater()const{
        return m_formatter;
    }
    LogLevel::Level getLevel()const{return m_level;}
    void setLevel(LogLevel::Level level){
        m_level = level;
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
    // get 方法
    string getName()const{return m_name;}
private:
    string m_name;                      // 日志名称
    LogLevel::Level m_level;            // 日志级别
    list<LogAppender::ptr> m_appenders;  // appender集合
};

// 输出到控制台的Appender
class StdoutLogAppender: public LogAppender
{
public:
    typedef shared_ptr<StdoutLogAppender> ptr;
    void log(shared_ptr<Logger> pLogger,LogLevel::Level level, LogEvent::ptr event);
private:

};
// 定义输出到文件的Appender
class FileLogAppender:public LogAppender
{
public:
    typedef shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const string &filename);
    virtual void log(shared_ptr<Logger> pLogger,LogLevel::Level level, LogEvent::ptr event) override;

    // 重新打开文件
    bool reopen();
private:
    string m_filename;//文件名
    ofstream m_filestream;//流
};


// 日志格式
   // %m--输出代码中指定的信息，如log(message)中的message
class MessageFormatItem : public LogFormatter::FormatItem{
public:
    MessageFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getContent();
    }
};
    // %p--输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推
class LevelFormatItem : public LogFormatter::FormatItem{
public:
    LevelFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<LogLevel::ToString(level);
    }
};
    // %r--输出自应用启动到输出该日志信息所耗费的毫秒数
class ElapseFormatItem : public LogFormatter::FormatItem{
public:
    ElapseFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getElapse();
    }
};
    // %t--输出产生该日志事件的线程名
class ThreadIDFormatItem : public LogFormatter::FormatItem{
public:
    ThreadIDFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getThreadId();
    }
};
    // %n--换行
class NewLineFormatItem : public LogFormatter::FormatItem{
public:
    NewLineFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<endl;
    }
};
    // %c--日志名称
class LoggerNameFormatItem : public LogFormatter::FormatItem{
public:
    LoggerNameFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<pLogger->getName();
    }
};
    // 输出协程id
class FiberIdFormatItem : public LogFormatter::FormatItem{
public:
    FiberIdFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getFiberId();
    }
};
    // 输出时间
class DateTimeFormatItem : public LogFormatter::FormatItem{
public:
    DateTimeFormatItem(string format = "%Y-%m-%d %H:%M:%S") : m_format(format){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getTime();
    }
private:
    string m_format;
};
    // %n--回车
    // %d--日志格式
    // %f--文件名
class FileNameFormatItem : public LogFormatter::FormatItem{
public:
    FileNameFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getFile();
    }
};
    // %l--行号
class LineFormatItem : public LogFormatter::FormatItem{
public:
    LineFormatItem(const string & str=""){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<event->getLine();
    }
};
class StringFormatItem : public LogFormatter::FormatItem{
public:
    // TODO:
    // StringFormatItem(const string str) : FormatItem(str), m_string(str){}
    StringFormatItem(const string str) : m_string(str){}
    void format(ostream &os,shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)override{
        os<<m_string;
    }
private:
    string m_string;
};


}
#endif
