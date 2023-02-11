#pragma once

#include <string>

namespace cla
{
    struct ArgumentDesctiption {
        std::string argName;
        char argShortName;
        bool required;
        std::string desciption;

        bool operator==( const ArgumentDesctiption & other ) const {
            return argName == other.argName;
        }
    };

    struct PositionalArgumentDescription
    {
        unsigned int pos; // start from 0
        std::string description;

        bool operator<( const PositionalArgumentDescription & other ) const {
            return pos < other.pos;
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
