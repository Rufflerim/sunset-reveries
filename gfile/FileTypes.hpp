//
// Created by gaetz on 30/07/2023.
//

#ifndef GFILE_FILETYPES_HPP
#define GFILE_FILETYPES_HPP

#include "Defines.hpp"

namespace gfile {

    enum class FileType {
        Data = 0
    };

    str FileTypePath(FileType fileType);

}

#endif //GFILE_FILETYPES_HPP
