////////////////////////
//
//  Created: Wed May 01 2024
//  File: tests.hpp
//
////////////////////////

#pragma once

#include <iostream>
#include "mce/mce.hpp"

#define INIT_COUNTER(X, V) std::size_t X = V;
#define INIT_STR(X, V) std::string X = V;

#define TESTING_COUNTER_LABEL testing_counter
#define TOTAL_TESTING_COUNTER_LABEL total_testing_counter

#define CHECK(X) \
    { \
        std::cout << "[" << __FUNCTION__ << ":" << __LINE__ << "] "; \
        ++TOTAL_TESTING_COUNTER_LABEL; \
        if (X) { \
            std::cout << "OK" << std::endl; \
            ++TESTING_COUNTER_LABEL; \
        } else \
            std::cout << "Failed" << std::endl; \
    } \

#define INIT_TEST() \
    INIT_COUNTER(TESTING_COUNTER_LABEL, 0) \
    INIT_COUNTER(TOTAL_TESTING_COUNTER_LABEL, 0) \
    std::cout << "[" << __FUNCTION__ << "] Init testing" << std::endl; \

#define TERMINATE_TEST() \
    std::cout << "[" << __FUNCTION__ << "] Terminate testing: " << \
        TESTING_COUNTER_LABEL << "/" << TOTAL_TESTING_COUNTER_LABEL << std::endl; \

#define UPDATE_METHOD_ID 0
#define UPDATE_WITH_PARAM_METHOD_ID 1
#define REFERENCE_CUSTOM_METHOD_ID 2

DECLARE_CUSTOM_METHOD(update)
DECLARE_CUSTOM_METHOD(updateWithParam, int)
DECLARE_CUSTOM_METHOD(referenceMethod, int &)

namespace mce
{
    template<typename T>
    void registerCustomMethods(World &world)
    {
        REGISTER_CUSTOM_METHOD(UPDATE_METHOD_ID, update)
        REGISTER_CUSTOM_METHOD(UPDATE_WITH_PARAM_METHOD_ID, updateWithParam, int)
        REGISTER_CUSTOM_METHOD(REFERENCE_CUSTOM_METHOD_ID, referenceMethod, int &)
    }

    template<typename T>
    void unregisterCustomMethods(World &world)
    {
        UNREGISTER_CUSTOM_METHOD(UPDATE_METHOD_ID, update)
        UNREGISTER_CUSTOM_METHOD(UPDATE_WITH_PARAM_METHOD_ID, updateWithParam, int)
        UNREGISTER_CUSTOM_METHOD(REFERENCE_CUSTOM_METHOD_ID, referenceMethod, int &)
    }
}

namespace testing
{
    void test_entities();
    void test_components();
}
