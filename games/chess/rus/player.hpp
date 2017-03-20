#ifndef RUS_PLAYER_HPP
#define RUS_PLAYER_HPP

#include <cstdint>

namespace rus {
    namespace Player {

        using Type = uint8_t;

        enum : Type {
            white,
            black,

            size,
        };

        Type opponent(const Type player);
    }
}

#endif