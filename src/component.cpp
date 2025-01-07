////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: component.cpp
//
////////////////////////

#include "component.hpp"

namespace mce
{
    ComponentManagerException::ComponentManagerException(const std::string &message)
        : _message(message)
    { }

    const char *ComponentManagerException::what() const throw()
    {
        return _message.c_str();
    }
}
