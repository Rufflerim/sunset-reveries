//
// Created by Gaëtan Blaise-Cazalet on 18/12/2023.
//

#include "Defines.hpp"

#ifndef GPLATFORM_STRINGUTILS_HPP
#define GPLATFORM_STRINGUTILS_HPP

namespace gplatform {
    /**
     * Copy a string to another
     * @param dst Destination string
     * @param length Length of the destination string
     * @param src Source string
     */
    void StrCpy(char* dst, u32 length, const char* src);

    /**
     * Calculate the length of a string, and avoid buffer overflow by stopping at max
     * @param str String to measure
     * @param max Maximum length
     * @return Size of the string
     */
    size_t StrnLen(char* str, size_t max);

    /**
     * Calculate the length of a string
     * @param str String to measure
     * @return Size of the string
     */
    size_t StrLen(char* str);
}

#endif //GPLATFORM_STRINGUTILS_HPP
