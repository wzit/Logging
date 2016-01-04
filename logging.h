/// Federal University of Rio de Janeiro
/// Author : Julio de Castro Vargas Fernandes
/// Project : ExMachina

#ifndef LOGGING_H
#define LOGGING_H

/// C++ Headers
#include <map>
#include <ctime>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <chrono>
#include <cstring>
#include <sstream>
/// Project Headers
#include "loggingpolicy.h"

namespace ExMachina {
namespace Logging {

enum class LoggingLevel { Verbose = 0, Info = 1, Debug = 2, Warning = 3, Error = 4, Fatal = 5 };

template <typename LoggingPolicy>
class Logging;

template <typename LoggingPolicy>
void loggingDaemon(Logging<LoggingPolicy>* logger)
{
    std::unique_lock<std::timed_mutex> lock{logger->_writeMutex,std::defer_lock};

    do
    {
        std::this_thread::sleep_for(std::chrono::milliseconds{50});
        if(logger->_logBuffer.size()) {
            if(!lock.try_lock_for(std::chrono::milliseconds{50}))
                continue;

            for( auto& elem : logger->_logBuffer ) {
                logger->_policy.write( elem );
            }

            logger->_logBuffer.clear();
            lock.unlock();
        }
    } while(logger->_isRunning.test_and_set() || logger->_logBuffer.size());

    logger->_policy.write("Terminating the daemon...");
}

template <typename LoggingPolicy>
class Logging
{
public:
    static Logging<LoggingPolicy> &instance();
    ~Logging();

    /// This ensures that will be only a single log class
    /// This is the best way to implement the singleton pattern
    Logging(const Logging&) = delete;
    void operator=(const Logging&) = delete;

    /// Defining the friend function so we can use it to run the daemon
    template<typename Policy>
    friend void loggingDaemon(Logging<Policy>* logger);

    void terminateLogging();
    void assignThreadName(const std::string& threadName);

    /// Printing functions
    /// These functions are categorized by severity level
    template<LoggingLevel severity,typename...Args>
    void print(Args&&...args);
protected:
    /// Checking if the daemon is already running
    /// This is a very low-level construct, proceed with caution
    std::atomic_flag _isRunning{ ATOMIC_FLAG_INIT };

    /// Since it is possible that several threads will be running it is desirable to know which one of them the message is comming
    /// To that end we will store the uniques thread ids on this variable
    std::map<std::thread::id,std::string> _threadName;

    /// This variable will act like a small buffer
    /// On every call to log(.), the logger will not immediately write it on the log, it will buffer it here
    std::vector<std::string> _logBuffer;

    /// This thread will act as a daemon writting the log on the background
    std::thread _daemon;

    /// Since the log, in any kind of policy is using a resource (standard output, file, internet connection...) race conditions might occur
    /// To avoid those this mutex will control the writing on the resource
    std::timed_mutex _writeMutex;

    /// Time reference so we can log the instructions time
    std::chrono::high_resolution_clock::time_point _referenceTime;

    /// Holds the severity level as strings
    std::vector<std::string> _logSeverity;
private:
    Logging();
    LoggingPolicy _policy;
    LoggingLevel _threshold;
    LoggingLevel _current;
    unsigned int _logLineNumber;

    /// Implementation of the printing functions
    /// Remember that since variadic functions need a contour parameter we need to define the function twice
    void printImpl(std::stringstream&& logStream);
    template <typename First, typename...Rest>
    void printImpl(std::stringstream&& logStream,First&& firstParameters,Rest&&... restParameters);
};

template <typename LoggingPolicy>
Logging<LoggingPolicy> &Logging<LoggingPolicy>::instance()
{
   static Logging<LoggingPolicy> _instance;
   return _instance;
}

template <typename LoggingPolicy>
Logging<LoggingPolicy>::Logging() : _logLineNumber(0),_logSeverity{"<VERBOSE>","<INFO>","<DEBUG>","<WARNING>","<ERROR>","<FATAL>"}
{
    /// TODO: Make the log name/policy more customizable
    _policy.openOstream("runningLog_Exmachina.log");

    /// Initializing reference time
    _referenceTime = std::chrono::high_resolution_clock::now();

    /// Setting the atomic flag and spawning the daemon
    /// The usage of the daemon may be changed in the future
    _isRunning.test_and_set();
    _daemon = std::move(std::thread{loggingDaemon<LoggingPolicy>,this});
}

template <typename LoggingPolicy>
Logging<LoggingPolicy>::~Logging()
{
    /// Terminating the log
    /// Since this is a singleton this logger will only goes out of scope when the program finishes(i think this is desirable)
    terminateLogging();

    /// Releasing resources
    _policy.write("Daemon terminated...");
    _policy.closeOstream();
}

template <typename LoggingPolicy>
void Logging<LoggingPolicy>::terminateLogging()
{
    /// Releasing the atomic flag
    _isRunning.clear();

    /// Waiting for the thread to finish
    _daemon.join();
}

template <typename LoggingPolicy>
void Logging<LoggingPolicy>::assignThreadName(const std::string& threadName)
{
    _threadName[std::this_thread::get_id()] = threadName;
}

template <typename LoggingPolicy>
template<LoggingLevel severity,typename...Args>
void Logging<LoggingPolicy>::print(Args &&...args)
{
    std::stringstream logStream;

    /// Header
    std::chrono::high_resolution_clock::time_point currTime = std::chrono::high_resolution_clock::now();
    std::time_t currentTime = std::chrono::high_resolution_clock::to_time_t(currTime);
    char* currentTimeString = ctime(&currentTime);
    currentTimeString[strlen(currentTimeString) -1] =0;
    logStream << _logLineNumber++ << " < " << currentTimeString << " - ";
    logStream << std::chrono::duration_cast<std::chrono::milliseconds>(currTime - _referenceTime).count() << " ms >";
    logStream << _logSeverity[severity] << _threadName[std::this_thread::get_id()] << " ";
    printImpl(std::forward<std::stringstream>(logStream),std::move(args)...);
}

template <typename LoggingPolicy>
void Logging<LoggingPolicy>::printImpl(std::stringstream&& logStream)
{
    std::lock_guard<std::timed_mutex> lock(_writeMutex);
    _logBuffer.push_back(logStream.str());
}

template <typename LoggingPolicy>
template <typename First, typename...Rest>
void Logging<LoggingPolicy>::printImpl(std::stringstream &&logStream, First &&firstParameters, Rest &&...restParameters)
{
    logStream << firstParameters;
    printImpl(std::forward<std::stringstream>(logStream),std::move(restParameters)...);
}

}
}

#endif // LOGGING_H
