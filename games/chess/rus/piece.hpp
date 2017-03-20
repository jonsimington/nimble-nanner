#ifndef RUS_PIECE_HPP
#define RUS_PIECE_HPP

#include <cstdint>

namespace rus {

    namespace Piece {
        using Type = uint8_t;

        enum : Type {
            pawn,
            knight,
            bishop,
            rook,
            queen,
            king,

            size,
        };

    }

}

#endif