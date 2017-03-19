#ifndef RUS_UTIL_HPP
#define RUS_UTIL_HPP

#include <string>

namespace rus {
    namespace util {

        template<typename T>
        T from_string(const std::string & str);

    }
}

#include "impl/util.hpp"

#endif