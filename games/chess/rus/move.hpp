#ifndef RUS_MOVE_HPP
#define RUS_MOVE_HPP

#include <cstdint>

#include "enums.hpp"

namespace rus {

    class Move {
    public:

        //TODO: Add default constructor to take from, to, and flag

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

        Position to;
        Position from;
        Flag flag;


    };

}

#endif