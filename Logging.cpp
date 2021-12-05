#include "Logging.h"

#include <string>
#include <string.h>
#include <errno.h>
#include <time.h>

Logger* Logger::poGlobalLogger = nullptr;
LOG_Level Logger::m_enLogLevel = log_debug;

std::mutex Logger::oMutexLogLock;

Logger::InnerBuf::InnerBuf(std::ostream& oStream)
    :roOutputStream(oStream)
{
    // Register current process id
    roOutputStream << "Started logger on process:" << getpid() << " " << std::endl;
}

Logger::InnerBuf::InnerBuf(std::string oPath)
    :roOutputStream(*(new std::ofstream(oPath, std::ios::app)))
{
    // Register current process id
    roOutputStream << "Started logger on process:" << getpid() << " " << std::endl;
}

Logger::InnerBuf::~InnerBuf() {
    if (pbase() != pptr()) {
        putOutput();
    }
}

// When we sync the stream with the output. 
// 1) Output Plop then the buffer
// 2) Reset the buffer
// 3) flush the actual output stream we are using.
int Logger::InnerBuf::sync() {
    putOutput();
    return 0;
}

void Logger::InnerBuf::putOutput() {
    // Buffer for the binary output
    std::string oHexaOutput = " ";

    // Current time holder
    struct timespec stNow;

    // Prefix for the message
    std::string oPrefixString;
    
    clock_gettime(CLOCK_MONOTONIC, &stNow);

    // Add current time to log output
    oPrefixString = "[" + std::to_string(stNow.tv_sec) + " | ";

    // Lock logger
    oMutexLogLock.lock();

    // Nothing to output
    if("" == str() && nullptr == poGlobalLogger->m_pu8HexaDataBuffer){
        oMutexLogLock.unlock();
        return;
    }

    // Setup the logging prefix
    switch(m_enLogLevel){
        case log_debug:
            oPrefixString += "debug ";
            break;
        
        case log_informational:
            oPrefixString += "informational ";
            break;
        
        case log_notice:
            oPrefixString += "notice ";
            break;
        
        case log_warning:
            oPrefixString += "warning ";
            break;
        
        case log_error:
            oPrefixString += std::string("error [perror: ") + strerror(errno) + ":" + std::to_string(errno) + "] ";
            break;
        
        case log_critical:
            oPrefixString += "critical ";
            break;
        
        case log_alert:
            oPrefixString += "alert ";
            break;
        
        case log_emergency:
            oPrefixString += "emergency ";
            break;
    }

    // There is binary data to output
    if(nullptr != poGlobalLogger->m_pu8HexaDataBuffer){
        //sizeof("0xYY ") == 5

        // Allocate data temporarily to hold hexa output
        char* pcStringBuffer = (char*)malloc(poGlobalLogger->m_u32HexaDataBufferSize * 5 + 1);

        // Parse data into the buffer
        for(uint32_t i = 0; i < poGlobalLogger->m_u32HexaDataBufferSize; i++){
            sprintf(pcStringBuffer + ( i * 5 ), "0x%02X ", poGlobalLogger->m_pu8HexaDataBuffer[i]);
        }

        // Seal the string with null terminator
        pcStringBuffer[poGlobalLogger->m_u32HexaDataBufferSize * 5] = '\0';

        // Append data to buffer
        oHexaOutput += " " + std::string(pcStringBuffer);

        // Free allocated data
        free(pcStringBuffer);

        // Reset the buffer
        poGlobalLogger->m_pu8HexaDataBuffer = nullptr;
    }

    // Output data
    roOutputStream << oPrefixString << gettid() << "] " << str() << oHexaOutput << "\n";

    // Clear data
    str("");

    // Flush buffer
    roOutputStream.flush();

    // Unlock logger
    oMutexLogLock.unlock();
}

Logger::Logger(std::ostream& oStream):
    m_oOutputBuffer(oStream),
    std::ostream(&m_oOutputBuffer)
{}

Logger::Logger(std::string oPath):
    m_oOutputBuffer(oPath),
    std::ostream(&m_oOutputBuffer)
{}


void Logger::start(std::string oPath){
    close();
    poGlobalLogger = new Logger(oPath);
}


void Logger::start(std::ostream& oStream){
    close();
    poGlobalLogger = new Logger(oStream);
}

void Logger::close(){
    if(nullptr != poGlobalLogger){
        free(poGlobalLogger);
        poGlobalLogger = nullptr;
    }
}

void Logger::flushPrevious(){
    poGlobalLogger->m_oOutputBuffer.putOutput();
}


Logger& Logger::setBuffer(LOG_Level enLogLevel, uint8_t* pu8HexaDataBuffer, uint32_t u32HexaDataBufferSize){
    setLevel(enLogLevel);

    poGlobalLogger->m_pu8HexaDataBuffer = pu8HexaDataBuffer;
    poGlobalLogger->m_u32HexaDataBufferSize = u32HexaDataBufferSize;

    return *poGlobalLogger;
}

Logger& Logger::setLevel(LOG_Level enLogLevel){
    flushPrevious();
    m_enLogLevel = enLogLevel;
    // Do something with the log levels
    switch(m_enLogLevel){
        // little - moderate importance
        case log_debug:
        case log_informational:
        case log_notice:
        case log_warning:

            break;

        // high importance
        case log_error:
        case log_critical:
        case log_alert:
        case log_emergency:
            break;
    }
    return *poGlobalLogger;
}


