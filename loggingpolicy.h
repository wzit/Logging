/// Federal University of Rio de Janeiro
/// Author : Julio de Castro Vargas Fernandes
/// Project : ExMachina

#ifndef LOGGINGPOLICY_H
#define LOGGINGPOLICY_H

/// C++ Headers
#include <string>
#include <fstream>

namespace ExMachina {
namespace Logging {

/// Why Policies ???
/// In a grid setup we could send the logs by email, to do so using policies we would only need to implement the write function differently

class LoggingPolicyInterface
{
public:
    LoggingPolicyInterface();
    virtual ~LoggingPolicyInterface();
    virtual void openOstream(const std::string& name) = 0;
    virtual void closeOstream() = 0;
    virtual void write(const std::string& message) = 0;
};

class FileLoggingPolicy : public LoggingPolicyInterface
{
public:
    FileLoggingPolicy();
    ~FileLoggingPolicy();
    void openOstream(const std::string& name);
    void closeOstream();
    void write(const std::string& message);
private:
    std::ofstream _outputFileStream;
};

}
}

#endif // LOGGINGPOLICY_H
