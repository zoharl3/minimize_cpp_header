
#define _CRT_SECURE_NO_WARNINGS
#pragma warning( disable : 4996 )

#include "stdafx.h"

#include <boost/asio.hpp>
#include <boost/test/utils/nullstream.hpp>
#include <boost/array.hpp>

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <ctime>
#include <string>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <assert.h>
#include <sstream>

#ifdef _WINDOWS
#include <windows.h>
#endif

#include "logger.h"
#include "logging.h"

// zl I add explicit test of this code in Logger::println()
#define OUTPUT_VA_LIST(VA_message_prefix, type, VA_format) \
    va_list ap; \
    va_start(ap, VA_format); \
    vprintf_helper(VA_message_prefix, type, true, VA_format, ap); \
    va_end(ap); \

//#define OUTPUT_VA_LIST(VA_message_prefix, type, VA_format) ""; // ZL: disable this when it gives weird stack overflow error

#define OUTPUT_VA_LIST2(VA_message_prefix, type, VA_format) \
    va_list ap; \
    va_start(ap, VA_format); \
    vprintf_helper(VA_message_prefix, type, false, VA_format, ap); \
    va_end(ap); \

static const int  INDENT_SIZE = 2;
static const int  BUFFER_SIZE = 
    //4096*256; // stack overflow unless special compiler switch
    4096*4;
static const char START_PREFIX[] = ">> ";
static const char END_PREFIX[] = "<< ";
static const char WARNING_PREFIX[] = "!!! WARNING !!! ";
static const char ERROR_PREFIX[] = "!!! ERROR !!! ";

Logger::Logger() : std::ostream(new CLogBuf(*this))
{
    initialize();
}

Logger::Logger(int log_level) : std::ostream(new CLogBuf(*this))
{
    initialize();
    m_logging_level = log_level;
}

Logger::~Logger()
{
    flush();
    deinitialize();
    delete rdbuf();
}

// Note that this should not be called except by the construction or after
// an object had already been deinitialized. Otherwise, it is possible for a
// file not be closed.
void Logger::initialize()
{
    m_bSendToViewer = 0;
    m_quiet = false;
    m_cout_sbuf = m_cerr_sbuf = NULL;
    m_indent = 0;  // indentation level
    m_logging_level = 0;   // logging level
    m_normal_prefix = ""; //"~~ ";  // prefix (displayed before indentation)
    m_warn_prefix = "WW ";  // prefix (displayed before indentation)
    m_error_prefix = "EE ";  // prefix (displayed before indentation)
    //m_out = dynamic_cast<std::ostream *>(&std::cerr);  // logging destination
    m_out = &std::cout;
    (*m_out) << std::unitbuf; // turn buffering off
    m_clean_up_file = false;
    m_auto_flush = true;
    m_null = std::unique_ptr<std::ostream>(new boost::onullstream()); // acts like /dev/null
}

// must have been initialized first
void Logger::deinitialize()
{
    restore_cout();
    m_indent = 0;  // indentation level
    m_logging_level = 0;   // logging level
    m_normal_prefix = "~~ ";  // prefix (displayed before indentation)
    m_warn_prefix = "WW ";  // prefix (displayed before indentation)
    m_error_prefix = "EE ";  // prefix (displayed before indentation)
    clean_up_file();   // close the file if opened by Logger
    //m_auto_flush = false;
}

void Logger::output_header()
{
    std::stringstream ss;
    ss << "============================================================\n";
    time_t rawtime;
    struct tm *timeinfo;
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    ss << asctime(timeinfo) << std::endl;
    ss << "============================================================\n";

    (*m_out) << ss.str();
    if ( m_auto_flush ) {
        m_out->flush(); // doesn't work?
        (*m_out) << std::flush;
    }

    send_to_viewer(ss.str(), 1);
}

void Logger::clean_up_file()
{
    if (m_clean_up_file)
    {
        m_out->flush();
        delete m_out;
    }
    m_out = &std::cerr;
    m_clean_up_file = false;
}

// returns 0 on success
int Logger::use_file(const char *fname, bool append)
{
    std::ofstream *pfout = append ? new std::ofstream(fname, std::ios::app)
                                  : new std::ofstream(fname);
    if (pfout->fail()) {
        std::cerr << "Unable to open file fname for writing." << std::endl;
        delete pfout;
        return 1;
    }

    clean_up_file(); // close current logging file if needed

    m_out = dynamic_cast<std::ostream *>(pfout); // set new logging file
    m_clean_up_file = true;
    return 0;
}

void Logger::use_stream(std::ostream &out)
{
    clean_up_file();
    m_out = dynamic_cast<std::ostream *>(&out); // set new logging file
    m_clean_up_file = false;
}

// logging level
int Logger::level() const { return m_logging_level; }
int Logger::set_level(int log_level) { m_logging_level = log_level; return m_logging_level; }

// logging quiet
void Logger::restore_cout()
{
    if ( m_cerr_sbuf ) {
        std::cerr.rdbuf(m_cerr_sbuf);
        m_cerr_sbuf = NULL;
    }
    if ( m_cout_sbuf ) {
        std::cout.rdbuf(m_cout_sbuf);
        m_cout_sbuf = NULL;
    }
}

bool Logger::quiet() const { return m_quiet; }
bool Logger::quiet(bool q) 
{ 
    if ( q == m_quiet )
        return q;

    //printf("Logger::quiet(%d)\n", q);
    //assert(0);

    bool oldq = m_quiet;
    m_quiet = q;
    m_out = q ? &m_stbuf : dynamic_cast<std::ostream *>(&std::cerr);

    // Redirect for all those who don't use logger...
    // Muffling cout doesn't muffle printf.
    if ( m_quiet ) {
        assert(m_cerr_sbuf == NULL && m_cout_sbuf == NULL);
        m_cerr_sbuf = std::cerr.rdbuf();
        m_cout_sbuf = std::cout.rdbuf();
        std::cout.rdbuf(m_stbuf.rdbuf()); // redirect
        std::cerr.rdbuf(m_stbuf.rdbuf()); // redirect
    } else {
        restore_cout();
    }

    return oldq;
}

// logging prefix
std::string &Logger::set_normal_prefix(const char *log_prefix) { m_normal_prefix = log_prefix; return m_normal_prefix; }
const std::string &Logger::normal_prefix() const { return m_normal_prefix; }
std::string &Logger::set_warn_prefix(const char *log_prefix) { m_warn_prefix = log_prefix; return m_warn_prefix; }
const std::string &Logger::warn_prefix() const { return m_warn_prefix; }
std::string &Logger::set_error_prefix(const char *log_prefix) { m_error_prefix = log_prefix; return m_error_prefix; }
const std::string &Logger::error_prefix() const { return m_error_prefix; }

// auto-flush
bool Logger::auto_flush() const { return m_auto_flush; }
bool Logger::set_auto_flush(bool auto_flush) { m_auto_flush = auto_flush; return m_auto_flush; }

// force-flush the output
Logger& Logger::flush()
{
    m_out->flush();
    return *this;
}

// output the indented prefix
void Logger::output_prefix(const char *message_prefix, int type)
{
    std::stringstream ss;
    ss << (
            (type == MSG_WARN) ? m_warn_prefix :
            (type == MSG_ERROR) ? m_error_prefix : m_normal_prefix
            );

    ss << std::setw(m_indent * INDENT_SIZE) << "" << message_prefix;

    (*m_out) << ss.str();
    if ( m_auto_flush ) {
        m_out->flush();
        (*m_out) << std::flush;
    }

    send_to_viewer(ss.str(), 1);
}

////////////////////////////////////////////////////////////
// FORMATTED OUTPUT VERSIONS

// output functions
void Logger::vprintf_helper(const char *message_prefix, int type, bool bNewLine, const char *str)
{
    if ( m_quiet )
        return;
    output_prefix(message_prefix, type);

    std::stringstream ss;
    ss << str;
    if ( bNewLine )
        ss << std::endl;
    (*m_out) << ss.str();
    if ( m_auto_flush ) {
        m_out->flush();
        (*m_out) << std::flush;
    }

    send_to_viewer(ss.str(), 1);
}

void
Logger::vprintf_helper(const char *message_prefix, int type, bool bNewLine, const char *format, va_list ap)
{
    char str[BUFFER_SIZE+1];
    ::vsnprintf(str, BUFFER_SIZE, format, ap);

    vprintf_helper(message_prefix, type, bNewLine, str);
}

Logger& Logger::start_block(int level, const char *format, ...)
{
    if (m_logging_level < level) return *this;
    OUTPUT_VA_LIST(START_PREFIX, MSG_NORMAL, format);
    ++m_indent;
    return *this;
}

Logger& Logger::end_block(int level, const char *format, ...)
{
    if (m_logging_level < level) return *this;
    --m_indent;
    OUTPUT_VA_LIST(END_PREFIX, MSG_NORMAL, format);
    return *this;
}

Logger& Logger::print(const char *format, ...)
{
    OUTPUT_VA_LIST2("", MSG_NORMAL, format);
    return *this;
}

Logger& Logger::println(const char *format, ...)
{
    if ( 0 ) {
        OUTPUT_VA_LIST("", MSG_NORMAL, format);
    } else {
        va_list ap;
        va_start(ap, format);
        vprintf_helper( "", MSG_NORMAL, true, format, ap );
        va_end(ap);
    }
    return *this;
}

Logger& Logger::println(int level, const char *format, ...)
{
    if (m_logging_level < level) return *this;
    OUTPUT_VA_LIST("", MSG_NORMAL, format);
    return *this;
}

Logger& Logger::start_block(const char *format, ...)
{
    OUTPUT_VA_LIST(START_PREFIX, MSG_NORMAL, format);
    //vprintf_helper("", 0, "", "");
    ++m_indent;
    return *this;
}

Logger& Logger::end_block(const char *format, ...)
{
    --m_indent;
    OUTPUT_VA_LIST(END_PREFIX, MSG_NORMAL, format);
    return *this;
}

Logger& Logger::emphasize(const char *format, ...)
{
    (*m_out) << ANSI_COLOR_BOLDGREEN;
    send_color_to_viewer("lime");
    OUTPUT_VA_LIST("", MSG_NORMAL, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

Logger& Logger::emphasize2(const char *format, ...)
{
    (*m_out) << ANSI_COLOR_BOLDCYAN;
    send_color_to_viewer("cyan");
    OUTPUT_VA_LIST("", MSG_NORMAL, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

// Doesn't work with conemu
void SetConsoleColors(unsigned long colorref) 
{
#ifdef _WINDOWS
    // default palette for reference
    COLORREF palette[16] = {
        0x00000000, 0x00800000, 0x00008000, 0x00808000,
        0x00000080, 0x00800080, 0x00008080, 0x00c0c0c0,
        0x00808080, 0x00ff0000, 0x0000ff00, 0x00ffff00,
        0x000000ff, 0x00ff00ff, 0x0000ffff, 0x00ffffff
    };
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(info);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if ( !GetConsoleScreenBufferInfoEx(hConsole, &info) )
        std::cout << "SetConsoleColors(), error " << GetLastError() << std::endl;
    info.ColorTable[12] = colorref;  // Replace red
    info.wAttributes = 12;
    if ( !SetConsoleScreenBufferInfoEx(hConsole, &info) )
        std::cout << "SetConsoleColors(), error " << GetLastError() << std::endl;
#endif
}

// The reset changes the whole console, so in practice there can be only 16 colors at a time
// https://stackoverflow.com/questions/48071097/windows-c-change-console-color-values
void ResetConsoleColors() 
{
#ifdef _WINDOWS
    CONSOLE_SCREEN_BUFFER_INFOEX info;
    info.cbSize = sizeof(info);
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfoEx(hConsole, &info);
    info.ColorTable[12] = RGB(255, 0, 0);  // Restore red
    info.wAttributes = 7;
    SetConsoleScreenBufferInfoEx(hConsole, &info);
#endif
}

Logger& Logger::emphasize3(const char *format, ...)
{
    (*m_out) << ANSI_COLOR_BOLDMAGENTA;
    send_color_to_viewer("purple");
    OUTPUT_VA_LIST("", MSG_NORMAL, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

Logger& Logger::soft(const char *format, ...)
{
    (*m_out) << ANSI_COLOR_YELLOW;
    send_color_to_viewer("yellow");
    OUTPUT_VA_LIST(WARNING_PREFIX, MSG_WARN, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

Logger& Logger::warn(const char *format, ...)
{
    (*m_out) << ANSI_COLOR_BOLDYELLOW;
    send_color_to_viewer("yellow");
    OUTPUT_VA_LIST(WARNING_PREFIX, MSG_WARN, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}


Logger& Logger::error(const char *format, ...)
{
    bool q = quiet(false);
    (*m_out) << ANSI_COLOR_BOLDRED;
    send_color_to_viewer("red");
    OUTPUT_VA_LIST(ERROR_PREFIX, MSG_ERROR, format);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    quiet(q);
    return *this;
}


////////////////////////////////////////////////////////////
// OSTREAM VERSIONS

std::ostream& Logger::print(int level)
{
    if (m_logging_level < level) return *m_null;
    output_prefix("", MSG_NORMAL);
    return *this;
}


std::ostream& Logger::print()
{
    output_prefix("", MSG_NORMAL);
    return *this;
}


std::ostream& Logger::start_block(int level)
{
    if (m_logging_level < level) return *m_null;
    output_prefix(START_PREFIX, MSG_NORMAL);
    ++m_indent;
    return *this;
}


std::ostream& Logger::start_block()
{
    output_prefix(START_PREFIX, MSG_NORMAL);
    ++m_indent;
    return *this;
}


std::ostream& Logger::end_block(int level)
{
    if (m_logging_level < level) return *m_null;
    --m_indent;
    output_prefix(END_PREFIX, MSG_NORMAL);
    return *this;
}


std::ostream& Logger::end_block()
{
    --m_indent;
    output_prefix(END_PREFIX, MSG_NORMAL);
    return *this;
}


std::ostream& Logger::emphasize()
{
    (*m_out) << ANSI_COLOR_BOLDGREEN;
    send_color_to_viewer("lime");
    output_prefix("", MSG_NORMAL);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

std::ostream& Logger::emphasize2()
{
    (*m_out) << ANSI_COLOR_BOLDCYAN;
    send_color_to_viewer("cyan");
    output_prefix("", MSG_NORMAL);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

std::ostream& Logger::warn()
{
    (*m_out) << ANSI_COLOR_BOLDYELLOW;
    send_color_to_viewer("yellow");
    output_prefix(WARNING_PREFIX, MSG_WARN);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    return *this;
}

// When writing to it, finish with endl, or call flush.
std::ostream& Logger::error()
{
    bool q = quiet(false);
    (*m_out) << ANSI_COLOR_BOLDRED;
    send_color_to_viewer("red");
    output_prefix(ERROR_PREFIX, MSG_ERROR);
    (*m_out) << ANSI_COLOR_RESET;
    send_color_to_viewer("silver");
    quiet(q);
    return *this;
}

void Logger::send_color_to_viewer(std::string color)
{
    char cmd[1024];
    sprintf(cmd, "<font size=\"6\" face=\"Consolas\" color=\"%s\">|", color.c_str());
    send_to_viewer(cmd, 0);
}

void Logger::send_to_viewer(std::string msg, bool bAddSeparator)
{
    if ( m_bSendToViewer == 0 )	
        return;

    if ( bAddSeparator )
        msg = "|" + msg;

    int port = 32313;
    std::string host = "127.0.0.1";

    // copy msg to buf
    const size_t max_size = 100000;
    boost::array<char, max_size> buf;
    size_t len = std::min(max_size, msg.size());
//len = 99999; // test
    for ( int i = 0 ; i < len ; i++ )
        buf[i] = msg[i];
//buf[i] = '|'; // test

    boost::asio::io_service ios;
    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(host), port);
    boost::asio::ip::tcp::socket socket(ios);
    try {
        socket.open(boost::asio::ip::tcp::v4());

        // Not working on connect. Make the send work in async-mode. Without this, the connection seems to be lost sometimes, usually when the matlab API has unexplained eval errors.
        if ( 0 ) {
            boost::asio::ip::tcp::no_delay option(true);
            socket.set_option(option);
        }

        socket.connect(endpoint);

        boost::system::error_code error;
        size_t lw = socket.write_some(boost::asio::buffer(buf, len), error);
        //std::cout << "Wrote " << lw << " bytes" << std::endl;
    } catch(boost::system::system_error &e) {
        std::cout << std::endl << "Logger::send_to_viewer() socket failed - " << e.what() << "-" << e.code();
        if ( m_bSendToViewer != 2 ) {
            m_bSendToViewer = 0;
            std::cout << ", disabling send to viewer.";
        }
        std::cout << std::endl;
    }
    socket.close();
}

////////////////////////////////////////////////////////////
// Block object (RAII-based indentation block)

Logger::Block Logger::block(int log_level, const char *message)
{
    return Block(*this, log_level, message);
}
Logger::Block Logger::block(int log_level, const std::string &message)
{
    return Block(*this, log_level, message.c_str());
}

Logger::Block::Block(Logger &log, int log_level, const char *message)
{
    m_pLog = &log;
    strncpy(m_message, message, sizeof(m_message)/sizeof(char)); // not automatically null-terminated if overflowing
    m_message[sizeof(m_message)/sizeof(char)-1] = '\0';

    m_enabled = (log_level <= m_pLog->m_logging_level);
    if (m_enabled)
        m_pLog->start_block(m_message);
}

/*
// A block that is copied using a copy-constructor is disabled since control
// is passed to the block that got the copy. This avoids outputting stuff
// twice when using the factory pattern to create an object.
Logger::Block::Block(Block &block)
{
    m_enabled = block.m_enabled;
    block.m_enabled = false;

    // copy the contents
    m_pLog = block.m_pLog;
    strncpy(m_message, block.message, sizeof(m_message)/sizeof(char)); // not automatically null-terminated if overflowing
    m_message[sizeof(m_message)/sizeof(char)-1] = '\0';

    // no need to output again since block's construct would have already
    // output the header
}
*/

Logger::Block::~Block()
{
    if (m_enabled)
        m_pLog->end_block(m_message);
}

