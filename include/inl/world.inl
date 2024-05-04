////////////////////////
//
//  Created: Wed May 01 2024
//  File: world.inl
//
////////////////////////

#include <algorithm>
#include "engine/world.hpp"

namespace engine
{
    inline const std::size_t &World::getID() const
    {
        return _id;
    }

    template<typename T, typename ... ARGS>
    Component<T> &World::addComponent(const Entity &entity, ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity)
            components.resize(entity + 1);

        if (components.at(entity).has_value())
            return components.at(entity);

        components.at(entity).emplace(std::forward<ARGS>(args)...);

        if constexpr(HasInit<T>)
            components.at(entity)->init(this, entity);

        return components.at(entity);
    }

    template<typename T>
    inline void World::requestRemoveComponent(const Entity &entity)
    {
        _remove_component_requests.push_back({entity, &World::removeComponent<T>});
    }

    template<typename T>
    Component<T> &World::getComponent(const Entity &entity)
    {
        static Component<T> empty_component = std::nullopt;
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity)
            return empty_component;

        return components.at(entity);
    }

    template<typename T>
    Components<T> &World::getComponents()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components == _components.end()) {
            registerComponent<T>();
            return getComponents<T>();
        }

        return std::any_cast<Components<T> &>(components->second);
    }

    template<typename T>
    void World::registerComponent()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components != _components.end())
            return;

        _components.insert({std::type_index(typeid(T)), Components<T>()});
        _remove_component_methods.push_back(&World::requestRemoveComponent<T>);

        registerCustomMethods<T>(this);
    }

    template<typename T>
    void World::unregisterComponent()
    {
        auto components = _components.find(std::type_index(typeid(T)));

        if (components == _components.end())
            return;

        _components.erase(std::type_index(typeid(T)));
        _remove_component_methods.erase(
            std::remove_if(_remove_component_methods.begin(), _remove_component_methods.end(),
                [&](void (World::*method)(const Entity &entity)) {
                    return method == &World::requestRemoveComponent<T>;
                }),
            _remove_component_methods.end()
        );

        unregisterCustomMethods<T>(this);
    }

    template<typename T>
    void World::optimizeComponents()
    {
        Components<T> &components = getComponents<T>();

        if (!components.size())
            return;

        for (long i = components.size() - 1; i >= 0; --i) {
            if (components.at(i).has_value()) {
                components.resize(i + 1);
                return;
            }
        }
        unregisterComponent<T>();
    }

    template<typename T, auto M>
    void World::registerCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end()) {
            _custom_methods.insert({id, MethodContainer<World, void>()});
            methods = _custom_methods.find(id);
        }

        methods->second.push_back(&World::executeMethod<T, M>);
    }

    template<typename T, auto M>
    void World::unregisterCustomMethod(std::size_t id)
    {
        auto methods = _custom_methods.find(id);

        if (methods == _custom_methods.end())
            return;

        methods->second.erase(
            std::remove_if(methods->second.begin(), methods->second.end(),
                [&](void (World::*method)()) {
                    return method == &World::executeMethod<T, M>;
                }),
            methods->second.end()
        );

        if (!methods->second.size())
            _custom_methods.erase(methods);
    }

    template<typename T, auto M, typename ... ARGS>
    void World::executeMethod(ARGS &&... args)
    {
        Components<T> &components = getComponents<T>();

        for (auto component = components.begin(); component != components.end(); ++component) {
            if (!component->has_value())
                continue;

            (component->value().*M)(std::forward<ARGS>(args)...);
        }
    }

    template<typename T>
    void World::removeComponent(const Entity &entity)
    {
        Components<T> &components = getComponents<T>();

        if (components.size() <= entity || !components.at(entity).has_value())
            return;

        components.at(entity).reset();
        optimizeComponents<T>();
    }
}
