#ifndef RUS_QUEEN_HPP
#define RUS_QUEEN_HPP

#include "piece_board.hpp"
#include "enums.hpp"

namespace rus {
namespace queen {

    enum Ray_direction {
        nort,
        nort_east,
        sout_east,
        east,
        sout,
        nort_west,
        sout_west,
        west,

        ray_direction_size,
    };

    extern board::Piece_board raysTbl[ray_direction_size][64];

    board::Piece_board calcRay(const int sq, const int dir);
    board::Piece_board calcPositiveRayAttacks(int sq, const int dir, board::Piece_board occupied);
    board::Piece_board calcNegativeRayAttacks(int sq, const int dir, board::Piece_board occupied);

    board::Piece_board moves(board::Piece_board queens, const board::Piece_board any_friendly, const board::Piece_board any_enemy);
    board::Piece_board captures(const board::Piece_board queens, const board::Piece_board any_friendly, const board::Piece_board any_enemy);

    void pre_process();

}
}

#endif