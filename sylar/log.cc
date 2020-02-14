#include"log.h"
namespace sylar{

// -----LogAppender模块----
FileLogAppender::FileLogAppender(const std::string & filename) : m_filename(filename){

}
void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        m_filestream<<m_formatter->format(event);
    }
}

bool FileLogAppender::reopen(){
    if(m_filestream){
        // m_filestream
        m_filename
        m_filestream.close();
    }
    m_filestream.open(m_filename);
    return !!m_filestream;
}
void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        std::cout<<m_formatter->format(event);
    }
}


// -----Logger模块---
void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        for(auto& i : m_appenders){
            i->log(level, event);
        }
    }
}
// debug方法
void Logger::debug(LogEvent::ptr event){
    log(LogLevel::DEBUG, event);
}
//输出为info级别的日志
void Logger::info(LogEvent::ptr event){
    log(LogLevel::INFO,event);
}
// 输出为warn类型的日志
void Logger::warn(LogEvent::ptr event){
    log(LogLevel::WARN, event);
}
// 输出为error类型的日志
void Logger::error(LogEvent::ptr event){
    log(LogLevel::ERROR, event);
}
// 输出为fatal类型的日志
void Logger::fatal(LogEvent::ptr event){
    log(LogLevel::FATAL, event);
}

//添加和删除日志输出地 
void Logger::addAppender(LogAppender::ptr appender){
    m_appenders.push_back(appender);
}
void Logger::delAppender(LogAppender::ptr appender){
    // 寻找日志输出地 并删除
    auto ite = m_appenders.begin();
    while(ite != m_appenders.end()){
        if(*ite == appender){
            m_appenders.erase(ite);
            break;
        }
        ite++;
    }
}







}
