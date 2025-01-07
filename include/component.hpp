////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: component.hpp
//
////////////////////////

#pragma once

#include "entity.hpp"
#include "dependency.hpp"

#include <vector>
#include <map>

#define NULL_COMPONENT nullptr;

namespace mce
{
    class World;

    template<typename T>
    using Component = T *;

    template<typename T>
    using Components = std::vector<Component<T>>;

    class ComponentManagerException : public std::exception
    {
        public:
            ComponentManagerException(const std::string &message);

            virtual const char *what() const throw() override;

        private:
            std::string _message;
    };

    namespace component
    {
        using ComponentIndex = std::size_t;
        using ComponentEntityMap = std::map<Entity, ComponentIndex>;

        template<typename T>
        class Manager
        {
            public:
                Manager();
                ~Manager();

                template<typename ... ARGS>
                Component<T> insertEntity(const Entity &entity, ARGS &&... args);

                bool removeEntity(const Entity &entity);

                inline std::size_t size() const;
                inline bool containEntity(const Entity &entity) const;
                inline Component<T> get(const Entity &entity) const;
                inline Components<T> &getComponents();
                inline ComponentsDependency &getComponentsDependency();

                template<typename REQUIRED>
                void initDependency(ContainMainComponent contain_main_component, RemoveMainComponent remove_main_component);

                template<typename REQUIRED>
                void removeDependency(ContainMainComponent contain_main_component, RemoveMainComponent remove_main_component);

            private:
                Components<T> _components;
                ComponentEntityMap _map;
                ComponentsDependency _components_dependency;
        };
    }
}

#include "inl/component.inl"
