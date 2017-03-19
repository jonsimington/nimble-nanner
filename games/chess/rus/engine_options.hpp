#ifndef RUS_ENGINE_OPTIONS_HPP
#define RUS_ENGINE_OPTIONS_HPP

#include <memory>
#include <string>
#include "optional.hpp"

namespace rus {

    class Engine_options {
    public:

        std::experimental::optional<std::string> fen;
        std::experimental::optional<int> id_depth;

    };

}

#endif