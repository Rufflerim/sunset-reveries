//
// Created by Gaëtan Blaise-Cazalet on 18/12/2023.
//

#include "StringUtils.hpp"

namespace gplatform {

    void StrCpy(char* dst, u32 length, char* src) {
        strcpy_s(dst, length, src);
    }

}

