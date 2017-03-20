#ifndef RUS_MOVE_HPP
#define RUS_MOVE_HPP

#include <cstdint>

#include "position.hpp"

namespace rus {

    class Move {
    public:

        enum class Flag: uint8_t {
            quiet,
            dbl_pawn,
            king_castle,
            queen_castle,
            capture,
            ep_capture,
            knight_promo,
            bishop_promo,
            rook_promo,
            queen_promo,
        };

        Position::Type to;
        Position::Type from;
        Flag flag;


    };

}

#endif