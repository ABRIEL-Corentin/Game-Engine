////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: entity.hpp
//
////////////////////////

#pragma once

#include "types.hpp"

#include <string>
#include <vector>

namespace mce
{
    using Entity = std::size_t;
    using Entities = std::vector<Entity>;

    class World;

    class EntityManagerException : std::exception
    {
        public:
            EntityManagerException(const std::string &message);

            virtual const char *what() const throw() override;

        private:
            std::string _message;
    };

    namespace entity
    {
        using RemoveComponentMethod = Method<World, void, const Entity &, bool &&>;
        using RemoveComponentMethods = Methods<World, void, const Entity &, bool &&>;

        class Manager
        {
            public:
                Manager(World &world);

                bool hasAvailableEntity() const;
                void addEntityToAvailable(const Entity &entity);
                void clearEntityComponents(const Entity &entity);
                void addRemoveComponentMethod(RemoveComponentMethod remove_component_method);
                void removeRemoveComponentMethod(RemoveComponentMethod remove_component_method);
                Entity extractAvailableEntity();
                Entity generateNewEntity();

            private:
                World &_world;
                Entity _current;
                Entities _available;
                RemoveComponentMethods _remove_component_methods;
        };
    }
}
