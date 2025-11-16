#include <map>
#include <string>
#include <vector>
#include <memory>
#include <optional>

// Metadata describing the structure without values
class AnyMeta
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
    using MapMeta = std::map<AnyMeta, AnyMeta>;
    using ArrayMeta = std::vector<AnyMeta>;

    AnyMeta() = default;
    explicit AnyMeta(Type type) : mType(type) {}

    // Factory methods for each type
    static AnyMeta none() { return AnyMeta(Type::None); }
    static AnyMeta boolean() { return AnyMeta(Type::Bool); }
    static AnyMeta integer() { return AnyMeta(Type::Int); }
    static AnyMeta floating() { return AnyMeta(Type::Float); }
    static AnyMeta string() { return AnyMeta(Type::String); }
    
    static AnyMeta array(const AnyMeta& element_type) {
        AnyMeta meta(Type::Array);
        meta.mArrayElementType = std::make_shared<AnyMeta>(element_type);
        return meta;
    }
    
    static AnyMeta map(const AnyMeta& key_type, const AnyMeta& value_type) {
        AnyMeta meta(Type::Map);
        meta.mMapKeyType = std::make_shared<AnyMeta>(key_type);
        meta.mMapValueType = std::make_shared<AnyMeta>(value_type);
        return meta;
    }

    // Named fields for structured objects (like struct/class)
    static AnyMeta object(const std::map<String, AnyMeta>& fields) {
        AnyMeta meta(Type::Map);
        meta.mObjectFields = fields;
        return meta;
    }

    Type getType() const { return mType; }
    bool isNone() const { return mType == Type::None; }
    bool isBool() const { return mType == Type::Bool; }
    bool isInt() const { return mType == Type::Int; }
    bool isFloat() const { return mType == Type::Float; }
    bool isString() const { return mType == Type::String; }
    bool isArray() const { return mType == Type::Array; }
    bool isMap() const { return mType == Type::Map; }
    bool isObject() const { return mType == Type::Map && !mObjectFields.empty(); }

    // Get array element type
    const AnyMeta* getArrayElementType() const {
        return mArrayElementType.get();
    }

    // Get map key/value types
    const AnyMeta* getMapKeyType() const {
        return mMapKeyType.get();
    }

    const AnyMeta* getMapValueType() const {
        return mMapValueType.get();
    }

    // Get object fields (for structured data)
    const std::map<String, AnyMeta>& getObjectFields() const {
        return mObjectFields;
    }

    // Comparison for use in map keys
    bool operator<(const AnyMeta& other) const {
        if (mType != other.mType)
            return mType < other.mType;
        
        // For complex types, compare structure
        if (mType == Type::Array && mArrayElementType && other.mArrayElementType)
            return *mArrayElementType < *other.mArrayElementType;
        
        if (mType == Type::Map) {
            if (mMapKeyType && other.mMapKeyType) {
                if (*mMapKeyType < *other.mMapKeyType)
                    return true;
                if (*other.mMapKeyType < *mMapKeyType)
                    return false;
            }
            if (mMapValueType && other.mMapValueType)
                return *mMapValueType < *other.mMapValueType;
        }
        
        return false;
    }

    // Convert to JSON schema
    std::string to_json_schema() const {
        std::string schema = "{";
        
        switch (mType) {
            case Type::None:
                schema += "\"type\":\"null\"";
                break;
            case Type::Bool:
                schema += "\"type\":\"boolean\"";
                break;
            case Type::Int:
                schema += "\"type\":\"integer\"";
                break;
            case Type::Float:
                schema += "\"type\":\"number\"";
                break;
            case Type::String:
                schema += "\"type\":\"string\"";
                break;
            case Type::Array:
                schema += "\"type\":\"array\"";
                if (mArrayElementType) {
                    schema += ",\"items\":" + mArrayElementType->to_json_schema();
                }
                break;
            case Type::Map:
                schema += "\"type\":\"object\"";
                if (!mObjectFields.empty()) {
                    schema += ",\"properties\":{";
                    bool first = true;
                    for (const auto& [name, field_meta] : mObjectFields) {
                        if (!first) schema += ",";
                        first = false;
                        schema += "\"" + name + "\":" + field_meta.to_json_schema();
                    }
                    schema += "}";
                } else if (mMapKeyType && mMapValueType) {
                    schema += ",\"additionalProperties\":" + mMapValueType->to_json_schema();
                }
                break;
        }
        
        schema += "}";
        return schema;
    }

    // Human-readable type description
    std::string to_string() const {
        switch (mType) {
            case Type::None: return "None";
            case Type::Bool: return "Bool";
            case Type::Int: return "Int";
            case Type::Float: return "Float";
            case Type::String: return "String";
            case Type::Array:
                if (mArrayElementType)
                    return "Array<" + mArrayElementType->to_string() + ">";
                return "Array";
            case Type::Map:
                if (!mObjectFields.empty()) {
                    std::string str = "Object{";
                    bool first = true;
                    for (const auto& [name, field_meta] : mObjectFields) {
                        if (!first) str += ",";
                        first = false;
                        str += name + ":" + field_meta.to_string();
                    }
                    str += "}";
                    return str;
                }
                if (mMapKeyType && mMapValueType)
                    return "Map<" + mMapKeyType->to_string() + "," + 
                           mMapValueType->to_string() + ">";
                return "Map";
        }
        return "Unknown";
    }

private:
    Type mType = Type::None;
    
    // For Array: element type
    std::shared_ptr<AnyMeta> mArrayElementType;
    
    // For Map: key and value types
    std::shared_ptr<AnyMeta> mMapKeyType;
    std::shared_ptr<AnyMeta> mMapValueType;
    
    // For structured objects (named fields)
    std::map<String, AnyMeta> mObjectFields;
};
