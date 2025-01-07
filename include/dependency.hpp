////////////////////////
//
//  Created: Sat Jan 04 2025
//  File: dependency.hpp
//
////////////////////////

#pragma once

#include "entity.hpp"

#include <unordered_map>
#include <typeindex>

namespace mce
{
    using ContainMainComponents = Methods<World, bool, const Entity &>;
    using ContainMainComponent = Method<World, bool, const Entity &>;
    using RemoveMainComponents = Methods<World, void, const Entity &, bool &&>;
    using RemoveMainComponent = Method<World, void, const Entity &, bool &&>;

    struct ComponentDependency
    {
        ComponentDependency();

        ContainMainComponents contain_main_components;
        RemoveMainComponents remove_main_components;
    };

    using ComponentsDependency = std::unordered_map<std::type_index, ComponentDependency>;

    template<typename T>
    concept HasInitDependency = HasStaticMethod<&T::template initDependency<T>, void, World &>;

    template<typename T>
    concept HasRemoveDependency = HasStaticMethod<&T::template removeDependency<T>, void, World &>;

    template<typename T>
    concept HasApplyRequiredComponents = HasMethod<T, &T::applyRequiredComponents, void, World &, const Entity &>;
}
