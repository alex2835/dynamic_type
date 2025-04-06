#include <iostream>
#include <map>
#include <string>
#include <variant>

class Any
{
public:
    using String = std::string;
    using Map = std::map<Any, Any>;
    using Array = std::vector<Any>;

    enum class Type
    {
        None,
        Int,
        Float,
        String,
        Map,
        Array
    };

    Any(){}
    Any( int value ) { mType = Type::Int; mStorage = value; }
    Any( long long value ) { mType = Type::Int; mStorage = value; }
    Any( float value ) { mType = Type::Float; mStorage = value; }
    Any( double value ) { mType = Type::Float; mStorage = value; }
    Any( const char* value ) { mType = Type::String; mStorage = value; }
    Any( const std::string_view value ) { mType = Type::String; mStorage = String(value); }
    Any( const String& value ) { mType = Type::String; mStorage = value; }
    Any( const Map& value ) { mType = Type::Map; mStorage = value; }
    Any( const Array& value ) { mType = Type::Array; mStorage = value; }

    Any& operator=( int value ) { mType = Type::Int; mStorage = value; return *this; }
    Any& operator=( long long value ) { mType = Type::Int; mStorage = value; return *this; }
    Any& operator=( float value ) { mType = Type::Float; mStorage = value; return *this; }
    Any& operator=( double value ) { mType = Type::Float; mStorage = value; return *this; }
    Any& operator=( const String& value ) { mType = Type::String; mStorage = value; return *this; }
    Any& operator=( const Map& value ) { mType = Type::Map; mStorage = value; return *this; }
    Any& operator=( const Array& value ) { mType = Type::Array; mStorage = value; return *this; }

    Type getType() const { return mType; }
    long long& getInt() { return std::get<long long>( mStorage ); }
    double& getFloat() { return std::get<double>( mStorage ); }
    String& getString() { return std::get<String>( mStorage ); }
    Map& getMap() { return std::get<Map>( mStorage ); }
    Array& getArray() { return std::get<Array>( mStorage ); }

    const long long& getInt() const { return std::get<long long>( mStorage ); }
    const double& getFloat() const { return std::get<double>( mStorage ); }
    const String& getString() const { return std::get<String>( mStorage ); }
    const Map& getMap() const { return std::get<Map>( mStorage ); }
    const Array& getArray() const { return std::get<Array>( mStorage ); }

    bool operator< ( const Any& other ) const
    {
        if ( mType != other.mType )
            return mType < other.mType;
        switch (mType)
        {
        case Type::None:
            return false;
        case Type::Int:
            return getInt() < other.getInt();
        case Type::Float:
            return getFloat() < other.getFloat();
        case Type::String:
            return getString() < other.getString();
        case Type::Array:
            return getArray() < other.getArray();
        case Type::Map:
            return getMap() < other.getMap();
        }
    }

    private:
    Type mType = Type::None;
    std::variant<
        long long,
        double,
        String,
        Map,
        Array
    > mStorage;
};


void printAny( const Any& any ) 
{
    switch ( any.getType() )
    {
    case Any::Type::Map:
    {
        std::print("{{");
        bool first = true;
        for ( auto& [key, val] : any.getMap() )
        {
            if ( not first )
                std::print(", ");
            first = false;
            printAny( key );
            std::print( ":" );
            printAny( val );
        }
        std::print("}}");
        break;
    }
    case Any::Type::Array:
    {
        std::print("[");
        bool first = true;
        for ( auto& val : any.getArray() )
        {
            if ( not first )
                std::print(", ");
            first = false;
            printAny( val );
        }
        std::print("]");
        break;
    }
    case Any::Type::None:
        std::print("None");
        break;
    case Any::Type::Int:
        std::print("{}", any.getInt() );
        break;
    case Any::Type::Float:
        std::print("{:.2f}", any.getFloat() );
        break;
    case Any::Type::String:
        std::print("'{}'", any.getString() );
        break;
    }
}

int main(int, char**){
    Any object( Any::Array{ Any(), 1, 2, 3, 4.0f, "hello",
                            Any::Array{ "string", 5, 10.0 },
                            Any::Map{ { "str", 1 }, { 2, Any::Array{ 1, 2 } } } } );
    printAny( object );
}
