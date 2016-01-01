# Logging

A simple logging library to be used in C++ projects

## Features

- lightweight
- thread-safe code
- simple

## Initialization

No need to initialize anything just use the library as you wish. This is achieved using the singleton pattern, that means that once the the library is used for the first time it will initialize itself.

## Usage

Several macros have been defined to make the usage of the library as simple as possible, they are:

- LOG_VERBOSE
- LOG_INFO
- LOG_DEBUG
- LOG_WARNING
- LOG_ERROR
- LOG_FATAL

To use it you just need to call these macros with any number and type of arguments, like this:

- LOG_DEBUG("This is debug message number : ",1)
- LOG_INFO("Any number of parameters, be it ",1,2,3,4,"...","can be used.")
- LOG_ERROR("Any type of parameters as well.",&p,5,std::this_threas::get_id)

If you ever had the displeasure of working with printf from C these macros are essentiatly the same as printf (but without that ugly formatting).
