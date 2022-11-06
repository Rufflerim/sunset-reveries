//
// Created by gaetz on 05/11/2022.
//

#ifndef GAEMI_DEFINES_H
#define GAEMI_DEFINES_H

#include <string>

#define GAPI __declspec(dllexport)

// Types
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

// Platform specific
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define GPLATFORM_WINDOWS 1
#endif

// Debug
#if defined(_DEBUG) || defined(DEBUG)
    #define GDEBUG 1
#endif

#endif //GAEMI_DEFINES_H
