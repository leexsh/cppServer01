#include"log.h"
namespace sylar{

// ----LogFromat模块的实现----
LogFormatter::LogFormatter(const string & pattern)
    : m_pattern(pattern){

 }
    // 根据pattern格式解析出日志 
string LogFormatter::format(LogEvent::ptr event){
    stringstream ss;
    for(auto &i : m_items){
        i->format(ss,event);
    }
}
    // log4j日志格式输出一览
/*
    %c 输出日志信息所属的类的全名
    %d 输出日志时间点的日期或时间，默认格式为ISO8601，也可以在其后指定格式，比如：%d{yyy-MM-dd HH:mm:ss }，输出类似：2002-10-18- 22：10：28
    %f 输出日志信息所属的类的类名
    %l 输出日志事件的发生位置，即输出日志信息的语句处于它所在的类的第几行
    %m 输出代码中指定的信息，如log(message)中的message
    %n 输出一个回车换行符，Windows平台为“rn”，Unix平台为“n”
    %p 输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推
    %r 输出自应用启动到输出该日志信息所耗费的毫秒数
    %t 输出产生该日志事件的线程名
    所以：
        %5p [%t] (%F:%L) - %m%n 就表示
        宽度是5的优先等级 线程名称 (文件名:行号) - 信息 回车换行
*/
// %
void LogFormatter::init(){

}
// ----LogAppender模块的实现----
void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event){
    // TODO：format函数没写
    // if(level >= m_level){
    //     cout<<m_formatter->format(event);
    // }
}
    // 重新打开文件
bool FileLogAppender::reopen(){
    if(m_filestream){
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
FileLogAppender::FileLogAppender(const string &filename)
 : m_filename(filename){

}

void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event){
    // TODO：format函数没写
    // if(level >= m_level){
    //     m_filestream<<m_formatter->format(event);
    // }
}

// ----Logger模块的实现----
Logger::Logger(const string& name)
    : m_name(name){

}
void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        for(auto &i : m_appenders){
            // TODO：添加内容
            // 死循环
            // i->log(level, event);
        }
    }
}

void  Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG, event);
}
void  Logger::info(LogEvent::ptr event){
    log(LogLevel::INFO, event);
}
void  Logger::warn(LogEvent::ptr event){
    log(LogLevel::WARN, event);
}
void  Logger::error(LogEvent::ptr event){
    log(LogLevel::ERROR, event);
}
void  Logger::fatal(LogEvent::ptr event){
    log(LogLevel::FATAL, event);
}

    // 添加和删除日志输出地
void  Logger::addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
}
void  Logger::delAppender(LogAppender::ptr appender){
    auto ite = m_appenders.begin();
    while(ite != m_appenders.end()){
        if(*ite == appender){
            m_appenders.erase(ite);
            break;
        }
        ++ite;
    }
}

}

int main(){
    vector<int> vec{1,23,4};
    std::shared_ptr<int> ptr = make_shared<int>(34);
    // *ptr = 1;
    cout<<"ptr = "<<*ptr<<endl;
    cout<<"aa:"<<vec[1]<<endl;
    return 0;
}