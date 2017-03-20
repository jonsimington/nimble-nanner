#include <cassert>

#include "player.hpp"

namespace rus {
    namespace Player {

        Type opponent(const Type player) {
            assert(player < size);
            return static_cast<Type>(player ^ 0x1);
        }

    }
}