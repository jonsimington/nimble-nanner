#ifndef RUS_BISHOP_HPP
#define RUS_BISHOP_HPP

#include "piece_board.hpp"
#include "enums.hpp"

namespace rus {
namespace bishop {

enum Ray_direction {
    nort_east,
    sout_east,
    sout_west,
    nort_west,

    ray_direction_size,
};

board::Piece_board raysTbl[ray_direction_size][64];

board::Piece_board calcRay(const int sq, const int dir);
board::Piece_board calcPositiveRayAttacks(int sq, const int dir, board::Piece_board occupied);
board::Piece_board calcNegativeRayAttacks(int sq, const int dir, board::Piece_board occupied);

board::Piece_board moves(board::Piece_board bishops, const board::Piece_board any_friendly, const board::Piece_board any_enemy);
board::Piece_board captures(const board::Piece_board bishops, const board::Piece_board any_enemy);

void pre_process();

}
}

#endif