////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: world.cpp
//
////////////////////////

#include "world.hpp"

namespace mce
{
    World::World()
        : _entity_manager(*this)
        , _component_managers()
        , _remove_component_requests()
        , _custom_methods()
    { }

    Entity World::createEntity()
    {
        if (_entity_manager.hasAvailableEntity())
            return _entity_manager.extractAvailableEntity();

        return _entity_manager.generateNewEntity();
    }

    void World::requestDestroyEntity(const Entity &entity)
    {
        DestroyEntityRequest request = DestroyEntityRequest();

        request.request = &World::destroyEntity;
        request.entity = entity;
        _destroy_entity_requests.push_back(request);
    }

    void World::applyRequests()
    {
        for (auto request: _destroy_entity_requests)
            (this->*request.request)(request.entity);

        for (auto request: _remove_component_requests)
            (this->*request.request)(request.entity, std::move(request.force));

        _destroy_entity_requests.clear();
        _remove_component_requests.clear();
    }

    void World::destroyEntity(const Entity &entity)
    {
        _entity_manager.clearEntityComponents(entity);
        _entity_manager.addEntityToAvailable(entity);
    }
}
