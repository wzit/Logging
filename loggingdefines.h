#ifndef LOGGINGDEFINES_H
#define LOGGINGDEFINES_H

/// Project Headers
#include "loggingpolicy.h"
#include "logging.h"
using namespace ExMachina::Logging;

/// General macros
#define CAPTURE_VARIABLE_NAME(x) #x

/// These macros are defined to ease the use of the library
/// Each one of them represents a level of severity
#ifndef LOGGING_MACROS
    /// The two macros below should be used only when a complete understanding of the system is required
    #define LOG_VERBOSE Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Verbose>
    #define LOG_INFO Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Info>
    /// The name says it all...
    #define LOG_DEBUG Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Debug>
    #define LOG_WARNING Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Warning>
    /// There is a remarkable difference between ERROR and FATAL
    /// When an error occurs the system may go on keep this in mind while logging
    #define LOG_ERROR Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Error>
    #define LOG_FATAL Logging<FileLoggingPolicy>::instance().print<LoggingLevel::Fatal>
#endif

#endif // LOGGINGDEFINES_H
