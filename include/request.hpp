////////////////////////
//
//  Created: Sat Jan 04 2025
//  File: request.hpp
//
////////////////////////

#pragma once

#include "types.hpp"
#include "entity.hpp"

namespace mce
{
    template<typename T, typename R, typename ... ARGS>
    struct Request
    {
        Method<T, R, ARGS...> request;
    };

    struct RemoveComponentRequest : public Request<World, void, const Entity &, bool &&>
    {
        Entity entity;
        bool force;
    };

    struct DestroyEntityRequest : public Request<World, void, const Entity &>
    {
        Entity entity;
    };

    template<typename T>
    using Requests = std::vector<T>;
}
