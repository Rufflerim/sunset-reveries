//
// Created by gaetz on 29/08/2023.
//

#include "FileTypes.hpp"

namespace gfile {
    str FileTypePath(FileType fileType) {
        switch (fileType) {
            case FileType::Data: {
                return "assets/data/";
            }
            default: {
                return "";
            }
        }
    }
}