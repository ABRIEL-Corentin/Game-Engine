////////////////////////
//
//  Created: Fri Jan 03 2025
//  File: types.hpp
//
////////////////////////

#pragma once

#include <vector>

#define FORMAT_CUSTOM_METHOD_CONCEPT_NAME(NAME) Has##NAME##CustomMethod

#define DECLARE_CUSTOM_METHOD(NAME, ...) \
    template<typename T> \
    concept FORMAT_CUSTOM_METHOD_CONCEPT_NAME(NAME) = mce::HasMethod<T, &T::NAME, void, ##__VA_ARGS__>; \

#define REGISTER_CUSTOM_METHOD(ID, NAME, ...) \
    if constexpr(FORMAT_CUSTOM_METHOD_CONCEPT_NAME(NAME)<T>) \
        world.registerCustomMethod<T, &T::NAME, ##__VA_ARGS__>(ID); \

#define UNREGISTER_CUSTOM_METHOD(ID, NAME, ...) \
    if constexpr(FORMAT_CUSTOM_METHOD_CONCEPT_NAME(NAME)<T>) \
        world.unregisterCustomMethod<T, &T::NAME, ##__VA_ARGS__>(ID); \

namespace mce
{
    template<typename R, typename ... ARGS>
    using Function = R (*)(ARGS &&...);

    template<typename R, typename ... ARGS>
    using Functions = std::vector<Function<R, ARGS...>>;

    template<typename T, typename R, typename ... ARGS>
    using Method = R (T::*)(ARGS &&...);

    template<typename T, typename R, typename ... ARGS>
    using Methods = std::vector<Method<T, R, ARGS...>>;

    template<typename T, auto M, typename R, typename ... ARGS>
    concept HasMethod = requires(T &t, ARGS &&... args)
    {
        { (t.*M)(std::forward<ARGS>(args)...) } -> std::same_as<R>;
    };

    template<auto M, typename R, typename ... ARGS>
    concept HasStaticMethod = requires(ARGS &&... args)
    {
        { M(std::forward<ARGS>(args)...) } -> std::same_as<R>;
    };
}
