///////////////////////////////////////////////////////////////////////////////
/// @file: Logger.hpp                                                       ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_LOGGING_LOGGER_HPP_INCLUDED
#define NOO_LOGGING_LOGGER_HPP_INCLUDED


/// Forward declarations


/// Includes
#include <iostream>

/// Using declarations



namespace noo {
namespace logging {

class Logger
{
public:

    enum class LogLevel
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    static constexpr char const *
    logLevelToAnsiColor( LogLevel ll )
    {
        switch( ll )
        {
            // see https://en.wikipedia.org/wiki/ANSI_escape_code

            case LogLevel::TRACE: return "\033[1m[TRACE]\033[0m ";
            case LogLevel::DEBUG: return "\033[37;1m[DEBUG]\033[0m\033[37m ";
            case LogLevel::INFO : return "\033[36;1m[INFO] \033[0m\033[36m ";
            case LogLevel::WARN : return "\033[33;1m[WARN] \033[0m\033[33m ";
            case LogLevel::ERROR: return "\033[31;1m[ERROR]\033[0m\033[31m ";
        }
    }

    static void
    print( std::string const & msg, LogLevel ll, bool lineBreak = true, bool flush = true )
    {
        std::cout << logLevelToAnsiColor( ll ) << msg << "\033[0m" << ( lineBreak ? "\n" : "" );
        if ( flush ) std::cout.flush();
    }

    static void
    trace( std::string const & msg, bool lineBreak = true )
    {
        print( msg, LogLevel::TRACE, lineBreak );
    }

    static void
    debug( std::string const & msg, bool lineBreak = true )
    {
        print( msg, LogLevel::DEBUG, lineBreak );
    }

    static void
    info( std::string const & msg, bool lineBreak = true )
    {
        print( msg, LogLevel::INFO, lineBreak );
    }

    static void
    warn( std::string const & msg, bool lineBreak = true )
    {
        print( msg, LogLevel::WARN, lineBreak );
    }

    static void
    error( std::string const & msg, bool lineBreak = true )
    {
        print( msg, LogLevel::ERROR, lineBreak );
    }
};

} // - namespace logging
} // - namespace noo


#endif /* NOO_LOGGING_LOGGER_HPP_INCLUDED */

