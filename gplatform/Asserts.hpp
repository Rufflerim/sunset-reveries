//
// Created by gaetz on 27/06/2023.
//

#ifndef GPLATFORM_ASSERTS_HPP
#define GPLATFORM_ASSERTS_HPP

#include <string>
#include "Defines.hpp"

// Break function
// Under windows, clang emulates msvc compiler
#ifdef GASSERTIONS_ENABLED

    /**
     * Report when an assertion do not comply
     *
     * Must be defined in the logging file.
     *
     * @param expression Tested expression
     * @param message Message you want to pass
     * @param codeFile File the error is in
     * @param codeLine Line number
     */
    GAPI void ReportAssertionFailure(const str& expression, const str& message, const char* codeFile, i32 codeLine);


    // Assertions for all builds
    #define GASSERT(expr)                                                       \
    {                                                                           \
            if(expr) {                                                          \
            } else {                                                            \
                ReportAssertionFailure(#expr, "", __FILE__, __LINE__);          \
                DebugBreak();                                                   \
            }                                                                   \
    }

    // Assertions with message for all builds
    #define GASSERT_MSG(expr, message)                                          \
    {                                                                           \
        if(expr) {                                                              \
        } else {                                                                \
            ReportAssertionFailure(#expr, message, __FILE__, __LINE__);         \
            DebugBreakpoint();                                                  \
        }                                                                       \
    }

    // Debug build assertion
    #ifdef GDEBUG
    #define GASSERT_DEBUG(expr)                                                 \
        {                                                                       \
            if(expr) {                                                          \
            } else {                                                            \
                ReportAssertionFailure(#expr, "", __FILE__, __LINE__);          \
                DebugBreakpoint();                                              \
            }                                                                   \
        }
    #else
    #define GASSERT_DEBUG(expr)     // Does nothing
    #endif

#else
    #define GASSERT(expr)               // Does nothing
    #define GASSERT_MSG(expr, message)  // Does nothing
    #define GASSERT_DEBUG(expr)         // Does nothing
#endif

#endif //GPLATFORM_ASSERTS_HPP
