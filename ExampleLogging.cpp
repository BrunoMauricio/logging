#include "Logging.cpp"


int main(int argc, char* argv[]){
    //Logger::start("/some/file/log");
    Logger::start(std::cout);
    LOG(log_debug) << "debug message" << std::flush;
    LOG(log_informational) << "informational message" << std::flush;
    LOG(log_notice) << "notice message" << std::flush;
    LOG(log_warning) << "warning message" << std::flush;
    LOG(log_error) << "error message" << std::flush;
    LOG(log_critical) << "critical message" << std::flush;
    LOG(log_alert) << "alert message" << std::flush;
    LOG(log_emergency) << "emergency message" << std::flush;

    uint8_t u8Buffer[6] = {0x01, 0x02, 0x03, 0x30, 0x20, 0x10};
    LOGBUFF(log_debug, u8Buffer, 6) << "debug message" << std::flush;

    /*    Expected output
    25878 25878
    [38719 | debug 25878] main() debug message 
    [38719 | informational 25878] main() informational message 
    [38719 | notice 25878] main() notice message 
    [38719 | warning 25878] main() warning message 
    [38719 | error [perror: Success:0] 25878] main() error message 
    [38719 | critical 25878] main() critical message 
    [38719 | alert 25878] main() alert message 
    [38719 | emergency 25878] main() emergency message 
    [38719 | debug 25878] main() debug message  0x01 0x02 0x03 0x30 0x20 0x10 
    */
}