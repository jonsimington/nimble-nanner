#ifndef RUS_KNIGHT_HPP
#define RUS_KNIGHT_HPP

#include "piece_board.hpp"

namespace rus {
namespace knight {

Piece_board attacks[64];
void preProcessAttacks() {
    for(Piece_board i = 0x1; i <= 0x1 << 63; i <<= 1) {
        
    }
}

}
}

#endif