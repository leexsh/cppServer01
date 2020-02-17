#include "log.h"
namespace sylar{
// ----LogLevel模块的实现----
const char* LogLevel::ToString(LogLevel::Level level){
    switch(level){
        // 定义了一个宏 返回 "DEBUG" "INFO" "WARN" "ERROR" "FATAL"等字符串
        #define XX(name)\
            case LogLevel::name:\
            return #name; \
            break;
            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
        #undef XX
        default:
            return "UNKNOW";
            break;
    }
    return "UNKNOW";
}

// ----LogFromat模块的实现----
LogFormatter::LogFormatter(const string & pattern)
    : m_pattern(pattern){

 }
    // 根据pattern格式解析出日志 
    //  string format(shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event)
string LogFormatter::format(shared_ptr<Logger> pLogger,LogLevel::Level level,LogEvent::ptr event){
    stringstream ss;
    for(auto &i : m_items){
        // TODO：
        i->format(ss,pLogger,level, event);
        // i->format(ss,pLogger,level,event);
    }
    return "";
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
// %%xxx %xxxx %xxx{xxx}
void LogFormatter::init(){
    // 对象解析
    vector<tuple<string,string, int>> vec;
    size_t last_pos = 0; //unused
    string nstr;
    for(size_t i= 0; i < m_pattern.size(); ++i){
        if(m_pattern[i] != '%'){
            nstr.append(1,m_pattern[i]);
            continue;
        }

        if((i+1) < m_pattern.size()){
            if(m_pattern[i+1] == '%')
            {
                nstr.substr(1,'%');
                continue;
            }
        }

        size_t n = i + 1;
        int fmt_status = 0;
        size_t fmt_begin = 0;

        string str;
        string fmt;
        while(n < m_pattern.size()){
            if(isspace(m_pattern[n])){
                break;
            }
            if(fmt_status == 0){
                if(m_pattern[n] == '{'){
                    str = m_pattern.substr(i+1, n - i - 1);
                    fmt_status = 1;// 解析格式
                    ++n;
                    continue;
                }
            }
           if(fmt_status == 1){
               if(m_pattern[n] == '}'){
                   fmt = m_pattern.substr(fmt_begin +1, n - fmt_begin - 1);
                   fmt_status = 2;
                   break;
               }
           }

        }
        if(fmt_status == 0){
            if(!nstr.empty()){
                vec.push_back(make_tuple(nstr,"",0));
            }
            str = m_pattern.substr(i+1, n - i - 1);
            vec.push_back(make_tuple(str, fmt, 1));
            i = n;
        }
        else if(fmt_status == 1)
        {
            cout<<"pattern parse error"<<m_pattern<<" - "<<m_pattern.substr(i)<<endl;
            vec.push_back(make_tuple("<<pattern_error>>", fmt,1));
        }
        else if(fmt_status == 2){
             if(!nstr.empty()){
                vec.push_back(make_tuple(nstr,"",0));
            }
            vec.push_back(make_tuple(str, fmt, 1));
            i = n;
        }
    }
    if(!nstr.empty()){
        vec.push_back(make_tuple(nstr, "", 0));
    }
    static map<string,function<FormatItem::ptr(const string &str)>> s_format_items={
        #define XX(str, C)\
            {#str, [](const string & fmt){return FormatItem::ptr(new C(fmt));}}

            XX(m,MessageFormatItem),
            XX(p, LevelFormatItem),
            XX(r,ElapseFormatItem),
            XX(c,LoggerNameFormatItem),
            XX(t,ThreadIDFormatItem),
            XX(n,NewLineFormatItem),
            XX(d,DateTimeFormatItem),
            XX(f,FileNameFormatItem),
            XX(l,LineFormatItem),
        #undef XX

    };
    // %m--输出代码中指定的信息，如log(message)中的message
    // %p--输出优先级，即DEBUG，INFO，WARN，ERROR，FATAL。如果是调用debug()输出的，则为DEBUG，依此类推
    // %r--输出自应用启动到输出该日志信息所耗费的毫秒数
    // %t--输出产生该日志事件的线程名
    // %c--日志名称
    // %n--回车
    // %d--日志格式
    // %f--文件名
    // %l--行号

    for(auto &i :vec){
        if(get<2>(i) ==0){
            m_items.push_back(FormatItem::ptr(new StringFormatItem(get<0>(i))));
        }
        else
        {
            auto ite = s_format_items.find(get<0>(i));
            if(ite == s_format_items.end()){
                m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %"+get<0>(i)+">>")));
            }
            else
            {
                m_items.push_back(ite->second(get<1>(i)));
            }
        }
        cout<<get<0>(i)<<" - "<<get<1>(i)<<" - "<<get<2>(i)<<endl;
    }
}

 
// ----LogAppender模块的实现----
void StdoutLogAppender::log(shared_ptr<Logger> pLogger,LogLevel::Level level, LogEvent::ptr event){
    // TODO：format函数没写
    if(level >= m_level){
        // cout<<m_formatter->format(event);
    }
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

void FileLogAppender::log(shared_ptr<Logger> pLogger,LogLevel::Level level, LogEvent::ptr event){
    // TODO：format函数没写
    if(level >= m_level){
    //     m_filestream<<m_formatter->format(event);
    }
}

// ----Logger模块的实现----
Logger::Logger(const string& name)
    : m_name(name){

}
void Logger::log(LogLevel::Level level, LogEvent::ptr event){
    if(level >= m_level){
        for(auto &i : m_appenders){
        //     // TODO：添加内容
        //     // 死循环
        //     // i->log(level, event);
        //     i->log(this,level, event);
        }
    }
    // no suitable constructor 
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
