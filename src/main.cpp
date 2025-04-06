#include <iostream>
#include "any.hpp"

int main(int, char**){
    Any object( Any::Array{ Any(), 1, 2, 3, 4.0f, "hello",
                            Any::Array{ "string", 5, 10.0 },
                            Any::Map{ { "str", 1 }, { 2, Any::Array{ 1, 2 } } } } );

    auto copy = object;
    copy.getArray().front() = 0;
    printAny( object );
    std::println("");
    printAny( copy );
}
