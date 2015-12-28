/// Federal University of Rio de Janeiro
/// Author : Julio de Castro Vargas Fernandes
/// Project : ExMachina

/// C++ Headers
#include <string>
#include <cassert>
#include <fstream>
/// Project Headers
#include "loggingpolicy.h"
using namespace ExMachina::Logging;

LoggingPolicyInterface::LoggingPolicyInterface()
{
}

LoggingPolicyInterface::~LoggingPolicyInterface()
{
}

FileLoggingPolicy::FileLoggingPolicy()
{
}

FileLoggingPolicy::~FileLoggingPolicy()
{
}

void FileLoggingPolicy::openOstream(const std::string& name)
{
    /// TODO: Now the log filename and the policy are hardcoded make them more elegant
    _outputFileStream.open(name.c_str(), std::ios_base::binary | std::ios_base::out);

    /// This still needs an ultimate decision
    /// Maybe we can go on even if the log function did not initialized correctly
    assert(_outputFileStream.is_open() == true);

    /// Setting precision for the output file
    _outputFileStream.precision(20);
}

void FileLoggingPolicy::closeOstream()
{
    if(_outputFileStream.is_open())
        _outputFileStream.close();
}

void FileLoggingPolicy::write(const std::string &message)
{
    _outputFileStream << message << std::endl;
}
