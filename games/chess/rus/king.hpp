#ifndef RUS_KING_HPP
#define RUS_KING_HPP

#include "piece_board.hpp"

namespace rus {
namespace king {

extern board::Piece_board movesTbl[64];

board::Piece_board calcMoves(board::Piece_board kings);
board::Piece_board moves(board::Piece_board kings, board::Piece_board any_friendly);
board::Piece_board captures(const board::Piece_board kings, const board::Piece_board any_friendly, const board::Piece_board any_enemy);
void pre_process();

}
}

#endif