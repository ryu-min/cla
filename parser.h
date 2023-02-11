#pragma once

#include "argument_description.h"

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cassert>

/// cla - command line arguments
namespace cla {

    class parser {

    private:

        /**
         * @brief m_parsedNamedArgs longArgName -> argValue
         */
        std::unordered_map<std::string, std::string> m_parsedNamedArgs;

        std::unordered_set<std::string> m_parsedPositionalArgs;
        std::unordered_set<ArgumentDesctiption> m_registeredArgs;

        bool isLongNameRegistered( const std::string & longName )
        {
            auto argIt = std::find_if( m_registeredArgs.begin(), m_registeredArgs.end(),
                                            [&longName](const ArgumentDesctiption & arg) {
                 return arg.argName == longName;
            });
            return argIt != m_registeredArgs.end();
        }

        bool isShortNameRegistered( char shortName )
        {
            auto argIt = std::find_if( m_registeredArgs.begin(), m_registeredArgs.end(),
                                            [shortName](const ArgumentDesctiption & arg) {
                 return arg.argShortName == shortName;
            });
            return argIt != m_registeredArgs.end();
        }

        std::string shortNameToLong( char shortName )
        {
            std::string longName;
            auto argIt = std::find_if( m_registeredArgs.begin(), m_registeredArgs.end(),
                                            [shortName](const ArgumentDesctiption & arg) {
                 return arg.argShortName == shortName;
            });
            if ( argIt != m_registeredArgs.end() ) {
                longName = argIt->argName;
            }
            return longName;
        }

        ArgumentDesctiption getArgForShortName( char shortName )
        {
            ArgumentDesctiption result;
            auto argIt = std::find_if( m_registeredArgs.begin(), m_registeredArgs.end(),
                                       [shortName](const ArgumentDesctiption & arg) {
                return arg.argShortName == shortName;
            });
            if ( argIt != m_registeredArgs.end() ) {
                result = *argIt;
            }
            return result;
        }


        ArgumentDesctiption getArgForLongName( const std::string & longName )
        {
            ArgumentDesctiption result;
            auto argIt = std::find_if( m_registeredArgs.begin(), m_registeredArgs.end(),
                                       [longName](const ArgumentDesctiption & arg) {
                return arg.argName == longName;
            });
            if ( argIt != m_registeredArgs.end() ) {
                result = *argIt;
            }
            return result;
        }

        enum class ParsingState {
            READ_ARG_NAME,
            READ_LONG_NAMED_ARG_VALUE,
            READ_SHORT_NAMED_ARG_VALUE
        };

    public :

        parser() {}

        parser & addPositional( const std::string & longName,
                            char shortName,
                            const std::string & description ) {
            return *this;

        }

        parser & addOptional( const std::string & longName,
                          char shortName,
                          const std::string & description )
        {
            if ( isLongNameRegistered(longName) )
            {
                std::cout << "argument with long name " << std::string(longName) << " already registered" << std::endl;
                exit(-1);
            }
            else if ( isShortNameRegistered(shortName) )
            {
                std::cout << "argument with short name " << shortName << " already registered" << std::endl;
                exit(-1);
            }
            else
            {
                m_registeredArgs.insert( ArgumentDesctiption { longName, shortName, false, description} );
            }
            return *this;
        }

        parser & addRequired( const std::string & longName,
                          char shortName,
                          const std::string & description )
        {
            if ( isLongNameRegistered(longName) ) {
                std::cout << "argument with long name " << std::string(longName) << " already registered" << std::endl;
                exit(-1);
            }
            else  if ( isShortNameRegistered(shortName) )
            {
                std::cout << "argument with short name " << shortName << " already registered" << std::endl;
                exit(-1);
            }
            else {
                m_registeredArgs.insert( ArgumentDesctiption { longName, shortName, true, description} );
            }
            return *this;
        }


        void parse( int argc, const char ** argv ) {

            int index = 0;
            ParsingState state = ParsingState::READ_ARG_NAME;
            std::string currentArgName;

            while ( index < argc ) {

                std::string currentString( argv[index] );

                switch (state) {
                    case ParsingState::READ_ARG_NAME:
                    {
                        if ( currentString.substr(0, 2) == "--" ) {
                            currentArgName = currentString.substr(2);
                            state = ParsingState::READ_LONG_NAMED_ARG_VALUE;
                        }
                        else if ( currentString.substr(0, 1) == "-") {
                            currentArgName = currentString.substr(1);
                            if ( currentArgName.size() != 1 ) {
                                std::cout << "wrong name format for " << currentString;
                                exit(-1);
                            }
                            state = ParsingState::READ_SHORT_NAMED_ARG_VALUE;
                        }
                        else {
                            m_parsedPositionalArgs.insert( currentString );
                        }
                    } break;

                    case ParsingState::READ_LONG_NAMED_ARG_VALUE: {
                        if ( currentString != "=" ) {

                            if ( isLongNameRegistered( currentArgName ) )
                            {
                                m_parsedNamedArgs[currentArgName] =  currentString;
                            }
                            else
                            {
                                std::cout << "detected unregistered argument with long name" << currentArgName << std::endl;
                                exit(-1);
                            }
                            state = ParsingState::READ_ARG_NAME;
                        }
                    } break;


                    case ParsingState::READ_SHORT_NAMED_ARG_VALUE: {
                        if ( currentString != "=" )
                        {
                            std::cout << currentArgName << std::endl;
                            assert( currentArgName.size() == 1 );
                            char shortName = currentArgName[0];
                            if ( isShortNameRegistered( shortName ) )
                            {
                                m_parsedNamedArgs[shortNameToLong( shortName )] = currentString;
                            }
                            else
                            {
                                std::cout << "detected unregistered argument with short name " << currentArgName;
                                exit(-1);
                            }
                            state = ParsingState::READ_ARG_NAME;
                        }
                    } break;
                }
                index++;
                continue;
            }
        }


        void dump() const {
            std::cout << "NAMED ARGS" << std::endl;
            for ( const auto & i : m_parsedNamedArgs ) {
                std::cout << i.first << " -> " << i.second << std::endl;
            }

            std::cout << "POSITINAL ARGS" << std::endl;
            for ( const auto & arg : m_parsedPositionalArgs ) {
                std::cout << arg << std::endl;
            }
        }

    };
}


