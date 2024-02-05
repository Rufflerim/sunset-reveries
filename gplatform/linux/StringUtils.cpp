//
// Created by Gaëtan Blaise-Cazalet on 18/12/2023.
//

#include "StringUtils.hpp"
#include <cstring>

namespace gplatform {

    void StrCpy(char* dst, u32 length, const char* src) {
        strcpy(dst, src);
    }

    size_t StrnLen(char* s, size_t max) {
        return strnlen(s, max);
    }

    size_t StrLen(char* s) {
        return strlen(s);
    }
}