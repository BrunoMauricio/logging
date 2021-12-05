#ifndef LOGGING_LOGGING
#define LOGGING_LOGGING

#include <fstream>
#include <iostream>
#include <string>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <sstream>
#include <mutex>

// The log levels
enum LOG_Level {
    log_debug, log_informational, log_notice, log_warning, log_error, log_critical, log_alert, log_emergency
};

// Based on https://newbedev.com/overload-handling-of-std-endl
class Logger: public std::ostream
{
    // Write a stream buffer that prefixes each line with Plop
    class InnerBuf: public std::stringbuf
    {
        std::ostream& roOutputStream;
        public:
            InnerBuf(std::ostream& oStream);
            InnerBuf(std::string oPath);
            ~InnerBuf();
   
        // When we sync the stream with the output. 
        // 1) Output the buffer
        // 2) Reset the buffer
        // 3) flush the actual output stream we are using.
        virtual int sync();

        void putOutput();
    };

    // Buffer to support std::flush/std::endl, and other stream buffer functions
    // is hooked to the loggers' ostream parent
    InnerBuf m_oOutputBuffer;

    // Buffer to hold binary data
    uint8_t* m_pu8HexaDataBuffer = nullptr;
    uint32_t m_u32HexaDataBufferSize = 0;

    // Lock to prevent concurrency issues
    static std::mutex oMutexLogLock;

    // The current logger level
    static LOG_Level m_enLogLevel;

    // The actual logger
    static Logger* poGlobalLogger;

    Logger(std::ostream& oStream);
    Logger(std::string oPath);

    public:

    // Sets up the logger
    static void start(std::ostream& oStream);
    static void start(std::string oPath);

    // Closes the debugger
    static void close();
    
    // Flushes any existing data
    static void flushPrevious();

    // Sets the debugging level, a buffer to output in hexa and returns appropriate logger
    static Logger& setBuffer(LOG_Level lvl, uint8_t* pu8HexaDataBuffer, uint32_t u32HexaDataBufferSize);

    // Sets the debugging level and returns appropriate logger
    static Logger& setLevel(LOG_Level lvl);

};

//Use flush instead of endl to avoid unnecessary newlines

// Returns an appropriate stream to output logs to
#define LOG(level) Logger::setLevel(level) << __func__ << "() "

// Flushes the buffer to avoid eventual frees from segfaulting the print data
#define LOGBUFF(level, buff, size) Logger::setBuffer(level, buff, size)  << __func__ << "() "


#endif
