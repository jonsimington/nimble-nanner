#ifndef RUS_PAWN_HPP
#define RUS_PAWN_HPP

#include "enums.hpp"
#include "piece_board.hpp"

namespace rus {
namespace pawn {

board::Piece_board move1(const Player_enum player, const board::Piece_board pawns, const board::Piece_board empty);
board::Piece_board move2(const Player_enum player, const board::Piece_board pawns, const board::Piece_board empty);
board::Piece_board attacks(const Player_enum player, const board::Piece_board pawns);
board::Piece_board captures(const Player_enum player, const board::Piece_board pawns, const board::Piece_board any_enemy);

} // namespace pawn
} // namespace rus



#endif