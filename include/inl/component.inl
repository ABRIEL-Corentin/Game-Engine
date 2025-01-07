////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: component.inl
//
////////////////////////

#pragma once

#include "component.hpp"

#include <algorithm>

namespace mce::component
{
    template<typename T>
    Manager<T>::Manager()
        : _components()
        , _map()
        , _components_dependency()
    { }

    template<typename T>
    Manager<T>::~Manager()
    {
        for (auto component: _components)
            delete component;
    }

    template<typename T>
    template<typename ... ARGS>
    Component<T> Manager<T>::insertEntity(const Entity &entity, ARGS &&... args)
    {
        auto it = _map.find(entity);

        if (it != _map.end())
            return _components.at(it->second);

        _map.insert({entity, _components.size()});
        _components.emplace_back(new T(std::forward<ARGS>(args)...));
        return _components.back();
    }

    template<typename T>
    bool Manager<T>::removeEntity(const Entity &entity)
    {
        auto entity_it = _map.find(entity);

        if (entity_it == _map.end())
            return false;

        delete _components.at(entity_it->second);
        for (auto it = std::next(entity_it); it != _map.end(); ++it)
            it->second -= 1;

        _components.erase(_components.begin() + entity_it->second);
        _map.erase(entity_it);
        return true;
    }

    template<typename T>
    inline std::size_t Manager<T>::size() const
    {
        return _components.size();
    }

    template<typename T>
    inline bool Manager<T>::containEntity(const Entity &entity) const
    {
        return _map.find(entity) != _map.end();
    }

    template<typename T>
    inline Component<T> Manager<T>::get(const Entity &entity) const
    {
        auto it = _map.find(entity);

        if (it == _map.end())
            throw ComponentManagerException("Invalid entity to get the component");

        return _components.at(it->second);
    }

    template<typename T>
    inline Components<T> &Manager<T>::getComponents()
    {
        return _components;
    }

    template<typename T>
    inline ComponentsDependency &Manager<T>::getComponentsDependency()
    {
        return _components_dependency;
    }

    template<typename T>
    template<typename REQUIRED>
    void Manager<T>::initDependency(ContainMainComponent contain_main_component, RemoveMainComponent remove_main_component)
    {
        auto components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));

        if (components_dependency == _components_dependency.end()) {
            _components_dependency.insert({std::type_index(typeid(REQUIRED)), ComponentDependency()});
            components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));
        }

        components_dependency->second.contain_main_components.push_back(contain_main_component);
        components_dependency->second.remove_main_components.push_back(remove_main_component);
    }

    template<typename T>
    template<typename REQUIRED>
    void Manager<T>::removeDependency(ContainMainComponent contain_main_component, RemoveMainComponent remove_main_component)
    {
        auto components_dependency = _components_dependency.find(std::type_index(typeid(REQUIRED)));

        if (components_dependency == _components_dependency.end())
            return;

        ContainMainComponents contain_main_components = components_dependency->second.contain_main_components;
        RemoveMainComponents remove_main_components = components_dependency->second.remove_main_components;

        contain_main_components.erase(
            std::remove_if(contain_main_components.begin(), contain_main_components.end(),
                [&](ContainMainComponent method) {
                    return method == contain_main_component;
                }
            ),
            contain_main_components.end()
        );

        remove_main_components.erase(
            std::remove_if(remove_main_components.begin(), remove_main_components.end(),
                [&](RemoveMainComponent method) {
                    return method == remove_main_component;
                }
            ),
            remove_main_components.end()
        );

        if (!contain_main_components.size() && !remove_main_components.size())
            _components_dependency.erase(components_dependency);
    }
}
