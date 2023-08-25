//
// Created by gaetz on 30/07/2023.
//

#ifndef GFILE_FILETYPES_HPP
#define GFILE_FILETYPES_HPP

#include "Defines.hpp"

enum class FileType {
    Data = 0
};

str FileTypePath(FileType fileType) {
    switch (fileType) {
        case FileType::Data: {
            return "assets/data/";
        }
    }
}

#endif //GFILE_FILETYPES_HPP
