////////////////////////
//
//  Created: Sat Jan 04 2025
//  File: require.inl
//
////////////////////////

#include "require.hpp"

namespace mce
{
    template<typename ... COMPONENTS>
    inline void Require<COMPONENTS...>::applyRequiredComponents(World &world, const Entity &entity)
    {
        ((world.addComponent<COMPONENTS>(entity)), ...);
    }

    template<typename ... COMPONENTS>
    template<typename T>
    inline void Require<COMPONENTS...>::initDependency(World &world)
    {
        ((world.initDependency<T, COMPONENTS>()), ...);
    }

    template<typename ... COMPONENTS>
    template<typename T>
    inline void Require<COMPONENTS...>::removeDependency(World &world)
    {
        ((world.removeDependency<T, COMPONENTS>()), ...);
    }
}
