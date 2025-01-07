////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: world.inl
//
////////////////////////

#include "world.hpp"

#include <iostream>
#include <algorithm>

namespace mce
{
    template<typename T>
    Components<T> World::getComponents()
    {
        component::Manager<T> &manager = getComponentManager<T>();

        return manager.getComponents();
    }

    template<typename T, typename ... ARGS>
    Component<T> World::addComponent(const Entity &entity, ARGS &&... args)
    {
        component::Manager<T> &manager = getComponentManager<T>();

        if (manager.containEntity(entity))
            return manager.get(entity);

        manager.insertEntity(entity, args...);

        if constexpr(HasApplyRequiredComponents<T>)
            manager.get(entity)->applyRequiredComponents(*this, entity);

        return manager.get(entity);
    }

    template<typename T>
    Component<T> World::getComponent(const Entity &entity)
    {
        component::Manager<T> &manager = getComponentManager<T>();

        if (!manager.containEntity(entity))
            return NULL_COMPONENT;

        return manager.get(entity);
    }

    template<typename T>
    bool World::hasComponent(const Entity &entity)
    {
        return getComponent<T>(entity) != NULL_COMPONENT;
    }

    template<typename T>
    void World::requestRemoveComponent(const Entity &entity, bool &&force)
    {
        RemoveComponentRequest request = RemoveComponentRequest();

        request.request = &World::removeComponent<T>;
        request.entity = entity;
        request.force = force;
        _remove_component_requests.push_back(request);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::registerCustomMethod(std::size_t id)
    {
        auto custom_methods = _custom_methods.find(id);

        if (custom_methods == _custom_methods.end()) {
            _custom_methods.insert({id, Methods<World, void, ARGS...>()});
            custom_methods = _custom_methods.find(id);
        }

        std::any_cast<Methods<World, void, ARGS...> &>(custom_methods->second).push_back(&World::executeMethod<T, M, ARGS...>);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::unregisterCustomMethod(std::size_t id)
    {
        auto custom_methods = _custom_methods.find(id);

        if (custom_methods == _custom_methods.end())
            return;

        Methods<World, void, ARGS...> &container = std::any_cast<Methods<World, void, ARGS...> &>(custom_methods->second);

        container.erase(
            std::remove_if(container.begin(), container.end(),
                [&](Method<World, void, ARGS...> custom_method) {
                    return custom_method == &World::executeMethod<T, M, ARGS...>;
                }
            ),
            container.end()
        );

        if (!container.size())
            _custom_methods.erase(custom_methods);
    }

    template<typename ... ARGS>
    void World::executeCustomMethod(std::size_t id, ARGS &&... args)
    {
        auto custom_methods = _custom_methods.find(id);

        if (custom_methods == _custom_methods.end())
            return;

        for (auto &custom_method: std::any_cast<Methods<World, void, ARGS...> &>(custom_methods->second))
            (this->*custom_method)(std::forward<ARGS>(args)...);
    }

    template<typename T>
    void World::removeComponent(const Entity &entity, bool &&force)
    {
        component::Manager<T> &manager = getComponentManager<T>();

        if (!manager.containEntity(entity)) {
            if (!manager.getComponents().size())
                unregisterComponent<T>();

            return;
        }

        ComponentsDependency &components_dependency = manager.getComponentsDependency();

        if (force)
            for (auto &remove_main_component: components_dependency.re)

        if (!manager.removeEntity(entity))
            return;

        if (!manager.getComponents().size())
            unregisterComponent<T>();
    }

    template<typename T>
    void World::registerComponent()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it != _component_managers.end())
            return;

        _component_managers.insert({std::type_index(typeid(T)), component::Manager<T>()});
        _entity_manager.addRemoveComponentMethod(&World::requestRemoveComponent<T>);
        registerCustomMethods<T>(*this);

        if constexpr(HasInitDependency<T>)
            T::template initDependency<T>(*this);
    }

    template<typename T>
    void World::unregisterComponent()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it == _component_managers.end())
            return;

        _component_managers.erase(std::type_index(typeid(T)));
        _entity_manager.removeRemoveComponentMethod(&World::requestRemoveComponent<T>);
        unregisterCustomMethods<T>(*this);

        if constexpr(HasRemoveDependency<T>)
            T::template removeDependency<T>(*this);
    }

    template<typename T>
    component::Manager<T> &World::getComponentManager()
    {
        auto it = _component_managers.find(std::type_index(typeid(T)));

        if (it == _component_managers.end()) {
            registerComponent<T>();
            return getComponentManager<T>();
        }

        return std::any_cast<component::Manager<T> &>(it->second);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethod(ARGS &&... args)
    {
        component::Manager<T> &manager = getComponentManager<T>();

        for (Component<T> &component: manager.getComponents())
            (component->*M)(std::forward<ARGS>(args)...);
    }

    template<typename T, typename REQUIRED>
    void World::initDependency()
    {
        component::Manager<T> &manager = getComponentManager<T>();

        manager.template initDependency<REQUIRED>(&World::hasComponent<T>, &World::removeComponent<T>);
    }

    template<typename T, typename REQUIRED>
    void World::removeDependency()
    {
        component::Manager<T> &manager = getComponentManager<T>();

        manager.template removeDependency<REQUIRED>(&World::hasComponent<T>, &World::removeComponent<T>);
    }
}
