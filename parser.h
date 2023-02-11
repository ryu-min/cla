#pragma once

#include "argument_description.h"

#include <functional>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <algorithm>
#include <cassert>

/// cla - command line arguments
namespace cla {

    class parser {

        enum class ParsingState {
            READ_ARG_NAME,
            READ_LONG_NAMED_ARG_VALUE,
            READ_SHORT_NAMED_ARG_VALUE
        };

    public :

        parser() {}

        parser & addPositional( const std::string & description )
        {
            PositionalArgumentDescription argDesctiptions {
                (unsigned int)m_registeredPositionalArgs.size(), description
            };
            m_registeredPositionalArgs.insert( argDesctiptions );
            return *this;
        }

        parser & addOptional( const std::string & longName,
                              char shortName,
                              std::string & valueToStore,
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
                m_stringValueStorage.emplace(longName, std::reference_wrapper<std::string>(valueToStore));
            }
            return *this;
        }


        parser & addOptional( const std::string & longName,
                              char shortName,
                              int & valueToStore,
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
                m_intValueStorage.emplace(longName, std::reference_wrapper<int>(valueToStore));

            }
            return *this;
        }


        parser & addOptional( const std::string & longName,
                              char shortName,
                              double & valueToStore,
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
                m_doubleValueStorage.emplace(longName, std::reference_wrapper<double>(valueToStore));

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


        parser & parse( int argc, const char ** argv ) {

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
                            if ( m_parsedNamedArgs.size() > m_registeredPositionalArgs.size() )
                            {
                                std::cout << "unregistered positional arg " << currentString << std::endl;
                                exit(-1);
                            }
                        }
                    } break;

                    case ParsingState::READ_LONG_NAMED_ARG_VALUE: {
                        if ( currentString != "=" ) {

                            if ( isLongNameRegistered( currentArgName ) )
                            {
                                m_parsedNamedArgs[currentArgName] =  currentString;
                                storeValue(currentArgName, currentString);
                                std::cout << "value stored" << std::endl;
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
                                const std::string fullArgName = shortNameToLong( shortName );
                                m_parsedNamedArgs[fullArgName] = currentString;
                                storeValue( fullArgName, currentString );
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
            return *this;
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

    private:
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

        void storeValue( const std::string & fullArgName, const std::string & value )
        {
            std::cout << "store value with" << fullArgName << " " << value << std::endl;
            if ( m_stringValueStorage.count(fullArgName) )
            {
                std::cout << "before call find" << std::endl;
                m_stringValueStorage.find(fullArgName)->second.get() = value;
                std::cout << "after call find" << std::endl;
                return;
            }
            else if ( m_intValueStorage.count(fullArgName) )
            {
                int intValue = 0;
                try
                {
                    intValue = std::stoi(value);
                    m_intValueStorage.find(fullArgName)->second.get() = intValue;
                    return;
                }
                catch(const std::invalid_argument & ex)
                {
                    std::cout << "can't convert " << value << " to integer value" << std::endl;
                    exit(-1);
                }
                catch( const std::out_of_range & ex)
                {
                    std::cout << "value " << value << " is to large to be stored now" << std::endl;
                }
                return;
            }
            else if ( m_doubleValueStorage.count(fullArgName) )
            {
                double doubleValue = 0;
                try
                {
                    doubleValue = std::stod(value);
                    m_doubleValueStorage.find(fullArgName)->second.get() = doubleValue;
                    return;
                }
                catch(const std::invalid_argument & ex)
                {
                    std::cout << "can't convert " << value << " to double value" << std::endl;
                    exit(-1);
                }
                catch( const std::out_of_range & ex)
                {
                    std::cout << "value " << value << " is to large to be stored now" << std::endl;
                }
                return;
            }

            assert( false );
            return;
        }

    private:

        std::set<std::string> m_parsedPositionalArgs;
        std::set<PositionalArgumentDescription> m_registeredPositionalArgs;

        std::unordered_map<std::string, std::string> m_parsedNamedArgs;
        std::unordered_set<ArgumentDesctiption> m_registeredArgs;

        /**
         * @brief m_stringValueStorage - full arg name -> &string to set arg value
         */
        std::unordered_map<std::string, std::reference_wrapper<std::string> > m_stringValueStorage;

        /**
         * @brief m_stringValueStorage - full arg name -> &string to set arg value
         */
        std::unordered_map<std::string, std::reference_wrapper<int> > m_intValueStorage;

        /**
         * @brief m_stringValueStorage - full arg name -> &string to set arg value
         */
        std::unordered_map<std::string, std::reference_wrapper<double> > m_doubleValueStorage;
    };
}


