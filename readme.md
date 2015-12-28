# Logging

A simple logging library to be used in C++ projects

## Features

- lightweight
- thread-safe code
- simple

## Usage

Several macros have been defined to make the usage of the library as simple as possible, they are:

- LOG_VERBOSE
- LOG_INFO
- LOG_DEBUG
- LOG_WARNING
- LOG_ERROR
- LOG_FATAL

To use it you just need to call this macros with any number and type of arguments, like this:

- LOG_DEBUG("This is debug message number : ",1)
