//
// Created by gaetz on 16/05/2023.
//

#include "Calendar.hpp"
#include <ctime>

namespace gplatform {

    array<char, 19> GetDate() {
        time_t now;
        array<char, 19> date {};
        struct tm timeInfo {};
        time(&now);
        localtime_s(&timeInfo, &now);
        strftime(date.data(), date.size(), "%y-%m-%d %H:%M:%S", &timeInfo);
        return date;
    }

}