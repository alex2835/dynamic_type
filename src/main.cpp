#include <iostream>
#include "any/any.hpp"
#include "any/any_meta.hpp"

void any_meta_test()
{
    AnyMeta int_type = AnyMeta::integer();
    std::cout << "Type: " << int_type.to_string() << "\n";
    std::cout << "Schema: " << int_type.to_json_schema() << "\n\n";

    // Describe an array of strings
    AnyMeta string_array = AnyMeta::array(AnyMeta::string());
    std::cout << "Type: " << string_array.to_string() << "\n";
    std::cout << "Schema: " << string_array.to_json_schema() << "\n\n";

    // Describe a map
    AnyMeta string_to_int = AnyMeta::map(AnyMeta::string(), AnyMeta::integer());
    std::cout << "Type: " << string_to_int.to_string() << "\n";
    std::cout << "Schema: " << string_to_int.to_json_schema() << "\n\n";

    // Describe a structured object (like a Person)
    AnyMeta person_type = AnyMeta::object({{"name", AnyMeta::string()},
                                           {"age", AnyMeta::integer()},
                                           {"active", AnyMeta::boolean()},
                                           {"tags", AnyMeta::array(AnyMeta::string())}});
    std::cout << "Type: " << person_type.to_string() << "\n";
    std::cout << "Schema: " << person_type.to_json_schema() << "\n\n";

    // Nested structure: array of person objects
    AnyMeta people_list = AnyMeta::array(person_type);
    std::cout << "Type: " << people_list.to_string() << "\n";
    std::cout << "Schema: " << people_list.to_json_schema() << "\n";
}

void any_test()
{
    Any object(Any::Array{Any(), 1, 2, 3, 4.0f, "hello",
                          Any::Array{"string", 5, 10.0},
                          Any::Map{{"str", 1}, {2, Any::Array{1, 2}}}});

    auto copy = object;
    copy.getArray().front() = 0;
    std::println("{}", object);
    std::println("");
    std::println("{}", copy);
}

int main(int, char **)
{
    any_meta_test();
    any_test();
    return 0;
}
