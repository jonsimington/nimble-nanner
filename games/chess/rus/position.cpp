#include <cassert>

#include "position.hpp"

namespace rus {
    namespace Position {

        Type from_rf(const int rank, const char file) {
            assert(rank >= 1);
            assert(rank <= 8);
            assert(file >= 'a');
            assert(file <= 'h');

            return static_cast<Type>((file-'a') + (8 * (rank-1)));
        }

    }
}