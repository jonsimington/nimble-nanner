#ifndef RUS_KNIGHT_HPP
#define RUS_KNIGHT_HPP

#include "piece_board.hpp"

namespace rus {
namespace knight {

board::Piece_board movesTbl[64];

board::Piece_board calcMoves(const board::Piece_board knights);
board::Piece_board moves(board::Piece_board knights);
board::Piece_board captures(const board::Piece_board knights, const board::Piece_board any_enemy);
void pre_process();

}
}

#endif