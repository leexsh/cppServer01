#ifndef _SYLAR_LOG_H_
#define _SYLAR_LOG_H_
#include <string>
#include <stdint.h>
#include<memory>
#include <list>
#include<stringstream>
#include<iostream>
#include <vector>
using namespace std;

namespace sylar{

// 日志事件
class LogEvent{
public:
	// 智能指针
	typedef std::shared_ptr<LogEvent> ptr;
	LogEvent();	
private:       
	const char *m_file = nullptr;//文件
	int32_t m_line = 0;//行号
	uint32_t m_elapse = 0;//程序启动到现在的毫秒数
	uint32_t m_threadId = 0;//线程id
	uint32_t m_fiberId = 0;//协程id
	uint64_t m_time;//事件戳
	std::string m_content;//内容
		
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
};

// 日志格式器
class LogFormatter{
public:
	typedef std::shared_ptr<LogFormatter> ptr;
	std::string format(LogEvent::ptr event);
	LogFormatter(const std::string &pattern);

private:
	
	class FormatItem{
	public:
		typedef std::shared_ptr<FormatItem> ptr;
		virtual ~FormatItem(){}
		virtual std::string format(std::ostream &os,LogEvent::ptr event) = 0
	};

	void init();
private:
	std::string m_pattern;
	std::vector<FormatItem::ptr> m_items;
};

// 日志输出地
class LogAppender{
public:
	typedef std::shared_ptr<LogAppender> ptr;
	virtual ~LogAppender(){}
	virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;

// 设置和获取formater
	void setFormatter(LogFormatter::ptr pf){
		m_formatter = pf;
	}
	LogFormatter::ptr getFormatter(){
		return m_formatter;
	}
protected:
	LogLevel::Level m_level;
	LogFormatter::ptr m_formatter;
};
 
// 日志器
class Logger{
public:
	typedef std::shared_ptr<Logger> ptr;
	// 默认构造
	Logger(const std::string& name = "root" );
	void log(LogLevel::Level level, LogEvent::ptr event);

	// 输出debug级别的日子方法
	void debug(LogEvent::ptr event);
	//输出为info级别的日志
	void info(LogEvent::ptr event);
	// 输出为warn类型的日志
	void warn(LogEvent::ptr event);
	// 输出为error类型的日志
	void error(LogEvent::ptr event);
	// 输出为fatal类型的日志
	void fatal(LogEvent::ptr event);

	//添加和删除append
	void addAppender(LogAppender::ptr appender);
	void delAppender(LogAppender::ptr appender);

	// 设置和获取日志级别
	void setLevel(LogLevel::Level level){
		m_level = levle;
	}
	LogLevel::Level getLevel() const{
		return m_level;
	}
private:
	std::string m_name;//日志名称
	LogLevel::Level m_level;//日志级别
	std::list<LogAppender::ptr> m_appenders;//Appender集合 输出的目的地的集合
};

// 输出到控制台的Appender
class StdoutLogAppender : public LogAppender{
public:
	typedef std::shared_ptr<StdoutLogAppender> ptr;
	virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
};

// 定义输出到文件的Appender
class FileLogAppender : public LogAppender{
public:
	typedef std::shared_ptr<FileLogAppender> ptr;
	virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
	// 重新打开文件
	bool reopen();
private:
	std::string m_name;
	std::ofstream m_filestream;
};


}
#endif
