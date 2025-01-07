////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: world.hpp
//
////////////////////////

#pragma once

#include "entity.hpp"
#include "component.hpp"
#include "types.hpp"
#include "request.hpp"
#include "dependency.hpp"

#include <unordered_map>
#include <typeindex>
#include <any>

namespace mce
{
    class World
    {
        template<typename ... COMPONENTS>
        friend class Require;

        public:
            World();

            Entity createEntity();
            void requestDestroyEntity(const Entity &entity);
            void applyRequests();

            template<typename T>
            Components<T> getComponents();

            template<typename T, typename ... ARGS>
            Component<T> addComponent(const Entity &entity, ARGS &&... args);

            template<typename T>
            Component<T> getComponent(const Entity &entity);

            template<typename T>
            bool hasComponent(const Entity &entity);

            template<typename T>
            void requestRemoveComponent(const Entity &entity, bool &&force = false);

            template<typename T, auto M, typename ... ARGS>
            void registerCustomMethod(std::size_t id);

            template<typename T, auto M, typename ... ARGS>
            void unregisterCustomMethod(std::size_t id);

            template<typename ... ARGS>
            void executeCustomMethod(std::size_t id, ARGS &&... args);

        private:
            entity::Manager _entity_manager;
            std::unordered_map<std::type_index, std::any> _component_managers;
            Requests<RemoveComponentRequest> _remove_component_requests;
            Requests<DestroyEntityRequest> _destroy_entity_requests;
            std::unordered_map<std::size_t, std::any> _custom_methods;

            void destroyEntity(const Entity &entity);

            template<typename T>
            void removeComponent(const Entity &entity, bool &&force);

            template<typename T>
            void registerComponent();

            template<typename T>
            void unregisterComponent();

            template<typename T>
            component::Manager<T> &getComponentManager();

            template<typename T, auto M, typename ... ARGS>
            void executeMethod(ARGS &&... args);

            template<typename T, typename REQUIRED>
            void initDependency();

            template<typename T, typename REQUIRED>
            void removeDependency();
    };
}

#include "inl/world.inl"
