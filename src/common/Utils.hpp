///////////////////////////////////////////////////////////////////////////////
/// @file: .hpp                                                             ///
/// @brief:                                                                 ///
/// @author: Ben Schneider                                                  ///
///////////////////////////////////////////////////////////////////////////////


#ifndef NOO_COMMON_UTILS_HPP_INCLUDED
#define NOO_COMMON_UTILS_HPP_INCLUDED


/// Forward declarations


/// Includes
#include <string>
#include <fstream>

#include "../logging/Logger.hpp"

/// Using declarations
using noolog = noo::logging::Logger;


namespace noo {
namespace common {

template< class enumeration >
constexpr std::size_t enum_count() noexcept
{
    static_assert( std::is_enum< enumeration >::value, "Not an enum!" );
    return static_cast< std::size_t >( enumeration::ENUM_END );
}

template< class enumeration >
constexpr std::size_t enum_index( enumeration const value ) noexcept
{
    static_assert( std::is_enum< enumeration >::value, "Not an enum!" );
    return static_cast< std::size_t >( value );
}


std::string readFile( char const * filePath )
{
    std::string content;
    std::ifstream fileStream( filePath, std::ios::in );

    if ( ! fileStream.is_open() )
    {
        noolog::error( "Could not read file " + std::string( filePath ) + ". File does not exist." );
        return "";
    }

    std::string line = "";

    while ( ! fileStream.eof() )
    {
        std::getline( fileStream, line );
        content.append( line + "\n" );
    }

    fileStream.close();
    return content;
}


} // - namespace common
} // - namespace noo


#endif /* NOO_COMMON_UTILS_HPP_INCLUDED */

