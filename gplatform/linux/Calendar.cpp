//
// Created by gaetz on 16/05/2023.
//

#include "Calendar.hpp"
#include <ctime>

namespace gplatform {

    array<char, 19> GetDate() {
        array <char, 19> date {};
        std::time_t t = std::time(nullptr);
        std::tm tm = *std::localtime(&t);
        std::strftime(date.data(), sizeof(date), "%y-%m-%d %H:%M:%S", &tm);
        return date;
    }

}