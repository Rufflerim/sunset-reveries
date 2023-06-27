//
// Created by gaetz on 05/11/2022.
//
#ifndef GPLATFORM_DEFINES_HPP
#define GPLATFORM_DEFINES_HPP

#include <string>
#include <vector>
#include <array>
#include <memory>

/**
 * Types
 */

using u8 = unsigned char;
using u16 = unsigned short;
using u32 = unsigned int;
using u64 = unsigned long long;

using i8 = signed char;
using i16 = signed short;
using i32 = signed int;
using i64 = signed long long;

using f32 = float;
using f64 = double;

using str = std::string;
using std::vector;
using std::array;

template<class T>
using uptr = std::unique_ptr<T>;

template<class T>
using sptr = std::shared_ptr<T>;

/**
 * Debug
 */
#if defined(_DEBUG) || defined(DEBUG)
    #define GDEBUG 1
#endif

/**
 * Platform specific macros
 *
 * Do not use except for tests.
 */

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(_MSC_VER)
#define GPLATFORM_WINDOWS 1
#elif defined(__GNUC__)
#define GPLATFORM_LINUX 1
#endif

#if defined(GPLATFORM_WINDOWS)
//  Microsoft
#define GAPI __declspec(dllexport)
#define GIMPORT __declspec(dllimport)
#elif defined(GPLATFORM_LINUX)
//  GCC
    #define GAPI __attribute__((visibility("default")))
    #define GIMPORT
#else
//  Do nothing and hope for the best?
    #define GAPI
    #define GIMPORT
    #pragma warning Unknown dynamic link import/export semantics.
#endif

/**
 * Assertions
 */

#define GASSERTIONS_ENABLED

#if _MSC_VER
    #include <intrin.h>
    #define DebugBreak() __debugbreak()
#else
    #define DebugBreak() __builtin_trap()
#endif

#endif //GPLATFORM_DEFINES_HPP
