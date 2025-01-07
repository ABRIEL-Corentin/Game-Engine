////////////////////////
//
//  Created: Sat Jan 04 2025
//  File: require.hpp
//
////////////////////////

#pragma once

#include "world.hpp"

namespace mce
{
    template<typename ... COMPONENTS>
    class Require
    {
        friend class World;

        private:
            inline void applyRequiredComponents(World &world, const Entity &entity);

            template<typename T>
            inline static void initDependency(World &world);

            template<typename T>
            inline static void removeDependency(World &world);
    };
}

#include "inl/require.inl"
