#pragma once
#include <map>
#include <string>
#include <vector>
#include <variant>
#include <format>

class Any
{
public:
    enum class Type
    {
        None,
        Bool,
        Int,
        Float,
        String,
        Map,
        Array
    };
    using String = std::string;
    using Map = std::map<Any, Any>;
    using Array = std::vector<Any>;

    Any() = default;
    ~Any() = default;
    Any(const Any &) = default;
    Any &operator=(const Any &) = default;
    Any(Any &&) = default;
    Any &operator=(Any &&) = default;

    Any(bool value)
    {
        mType = Type::Bool;
        mStorage = value;
    }
    Any(int value)
    {
        mType = Type::Int;
        mStorage = value;
    }
    Any(long long value)
    {
        mType = Type::Int;
        mStorage = value;
    }
    Any(float value)
    {
        mType = Type::Float;
        mStorage = value;
    }
    Any(double value)
    {
        mType = Type::Float;
        mStorage = value;
    }
    Any(const char *value)
    {
        mType = Type::String;
        mStorage = value;
    }
    Any(const std::string_view value)
    {
        mType = Type::String;
        mStorage = String(value);
    }
    Any(const String &value)
    {
        mType = Type::String;
        mStorage = value;
    }
    Any(const Map &value)
    {
        mType = Type::Map;
        mStorage = value;
    }
    Any(const Array &value)
    {
        mType = Type::Array;
        mStorage = value;
    }

    Any &operator=(bool value)
    {
        mType = Type::Bool;
        mStorage = value;
        return *this;
    }
    Any &operator=(int value)
    {
        mType = Type::Int;
        mStorage = value;
        return *this;
    }
    Any &operator=(long long value)
    {
        mType = Type::Int;
        mStorage = value;
        return *this;
    }
    Any &operator=(float value)
    {
        mType = Type::Float;
        mStorage = value;
        return *this;
    }
    Any &operator=(double value)
    {
        mType = Type::Float;
        mStorage = value;
        return *this;
    }
    Any &operator=(const String &value)
    {
        mType = Type::String;
        mStorage = value;
        return *this;
    }
    Any &operator=(const Map &value)
    {
        mType = Type::Map;
        mStorage = value;
        return *this;
    }
    Any &operator=(const Array &value)
    {
        mType = Type::Array;
        mStorage = value;
        return *this;
    }

    Type getType() const { return mType; }
    bool isBool() const { return mType == Type::Bool; }
    bool isNone() const { return mType == Type::None; }
    bool isInt() const { return mType == Type::Int; }
    bool isFloat() const { return mType == Type::Float; }
    bool isString() const { return mType == Type::String; }
    bool isArray() const { return mType == Type::Array; }
    bool isMap() const { return mType == Type::Map; }

    bool &getBool() { return std::get<bool>(mStorage); }
    long long &getInt() { return std::get<long long>(mStorage); }
    double &getFloat() { return std::get<double>(mStorage); }
    String &getString() { return std::get<String>(mStorage); }
    Map &getMap() { return std::get<Map>(mStorage); }
    Array &getArray() { return std::get<Array>(mStorage); }

    const bool &getBool() const { return std::get<bool>(mStorage); }
    const long long &getInt() const { return std::get<long long>(mStorage); }
    const double &getFloat() const { return std::get<double>(mStorage); }
    const String &getString() const { return std::get<String>(mStorage); }
    const Map &getMap() const { return std::get<Map>(mStorage); }
    const Array &getArray() const { return std::get<Array>(mStorage); }

    bool operator<(const Any &other) const
    {
        if (mType != other.mType)
            return mType < other.mType;

        switch (mType)
        {
        case Type::None:
            return false;
        case Type::Bool:
            return getBool() < other.getBool();
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
        bool,
        long long,
        double,
        String,
        Map,
        Array>
        mStorage;
};

std::string to_string(const Any &any);

namespace std
{
    template <>
    struct std::formatter<Any>
    {
        constexpr auto parse(format_parse_context &ctx) { return ctx.begin(); }

        auto format(const Any &any, format_context &ctx) const
        {
            return std::format_to(ctx.out(), "{}", to_string(any));
        }
    };

}