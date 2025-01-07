////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: entity.cpp
//
////////////////////////

#include "entity.hpp"
#include "world.hpp"

#include <algorithm>

namespace mce
{
    EntityManagerException::EntityManagerException(const std::string &message)
        : _message(message)
    { }

    const char *EntityManagerException::what() const throw()
    {
        return _message.c_str();
    }

    namespace entity
    {
        Manager::Manager(World &world)
            : _world(world)
            , _current()
            , _available()
        { }

        bool Manager::hasAvailableEntity() const
        {
            return _available.size();
        }

        void Manager::addEntityToAvailable(const Entity &entity)
        {
            _available.push_back(entity);
        }

        void Manager::clearEntityComponents(const Entity &entity)
        {
            for (auto remove_component_method: _remove_component_methods)
                (_world.*remove_component_method)(entity, true);
        }

        void Manager::addRemoveComponentMethod(RemoveComponentMethod remove_component_method)
        {
            _remove_component_methods.push_back(remove_component_method);
        }

        void Manager::removeRemoveComponentMethod(RemoveComponentMethod remove_component_method)
        {
            _remove_component_methods.erase(
                std::remove_if(_remove_component_methods.begin(), _remove_component_methods.end(),
                    [&](RemoveComponentMethod method) {
                        return method == remove_component_method;
                    }
                ),
                _remove_component_methods.end()
            );
        }

        Entity Manager::extractAvailableEntity()
        {
            if (!_available.size())
                throw EntityManagerException("No Entity to extract");

            Entity entity = _available.back();

            _available.pop_back();
            return entity;
        }

        Entity Manager::generateNewEntity()
        {
            return _current++;
        }
    }
}
