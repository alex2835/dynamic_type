#include "any.hpp"

std::string to_string( const Any& any )
{
    std::string str;
    switch ( any.getType() )
    {
    case Any::Type::Map:
    {
        str += "{";
        bool first = true;
        for ( auto& [key, val] : any.getMap() )
        {
            if ( not first )
                str += ",";
            first = false;
            str += std::format( "\"{}\"", key );
            str += ":" ;
            str += to_string( val );
        }
        str += "}";
        break;
    }
    case Any::Type::Array:
    {
        str += "[";
        bool first = true;
        for ( auto& val : any.getArray() )
        {
            if ( not first )
                str += ", ";
            first = false;
            str += to_string( val );
        }
        str += "]";
        break;
    }
    case Any::Type::None:
        str += "null";
        break;
    case Any::Type::Bool:
        str += any.getBool() ? "true" : "false";
        break;
    case Any::Type::Int:
        str += std::to_string( any.getInt() );
        break;
    case Any::Type::Float:
        str += std::to_string( any.getFloat() );
        break;
    case Any::Type::String:
        str += std::format( "\"{}\"", any.getString() );
        break;
    }
    return str;
}
