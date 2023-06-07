# Header-only Library for Command Line Argument Parsing (Under Development)

## Library description
This is a header-only library for parsing command line arguments in C++. It provides an easy-to-use interface for registering and accessing command line arguments
**Note:** This library is currently under development and not ready for use.

## Usage
To use this library, include the `parser.h` header file in your C++ project. 

**Usage example:** 
```c++
#include <cla/parser.h>
#include <iostream>

using namespace cla;

int main(int argc, char **argv)
{
    std::string stringValue;
    int intValue;
    double doubleValue;

    parser()
            .addRequired("string", 's', stringValue, "some description")
            .addRequired("int", 'i', intValue,"some description")
            .addRequired("double", 'd', doubleValue, "some description")
            .parse(argc, argv);
    
    std::cout << "string arg value is " << stringValue  << std::endl;
    std::cout << "int arg value is "    << intValue     << std::endl;
    std::cout << "double arg value is " << doubleValue  << std::endl;
    return 0;
}
```
