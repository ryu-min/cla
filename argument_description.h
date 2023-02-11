#pragma once

#include <string>

namespace cla
{
    struct ArgumentDesctiption {

        std::string argName;
        char argShortName;
        bool required;
        std::string desciption;

        bool operator==(ArgumentDesctiption other ) const {
            return argName == other.argName;
        }
    };
}

namespace std {
    template <>
    struct hash<cla::ArgumentDesctiption> {
        size_t operator ()( const cla::ArgumentDesctiption & value) const {
            std::hash<std::string> hasher;
            return hasher( value.argName );
        }
    };
}
