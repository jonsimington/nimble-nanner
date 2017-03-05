#include "pawn.hpp"

namespace rus {
namespace pawn {

board::Piece_board move1(const Player_enum player, const board::Piece_board pawns, const board::Piece_board empty) {
    if(player == white_idx) {
        return board::step_n(pawns) & empty;
    }
    else {
        return board::step_s(pawns) & empty;
    }
}
board::Piece_board move2(const Player_enum player, const board::Piece_board pawns, const board::Piece_board empty) {
    board::Piece_board m1 = move1(player, pawns, empty);

    if(player == white_idx) {
        return board::step_n(m1) & empty & board::rank4;
    }
    else {
        return board::step_s(m1) & empty & board::rank5;
    }
}
board::Piece_board attacks(const Player_enum player, const board::Piece_board pawns) {
    if(player == white_idx) {
        return board::step_ne(pawns) | board::step_nw(pawns);
    }
    else {
        return board::step_se(pawns) | board::step_sw(pawns);
    }
}
board::Piece_board captures(const Player_enum player, const board::Piece_board pawns, const board::Piece_board any_enemy) {
    return attacks(player, pawns) & any_enemy;
}

} // namespace pawn
} // namespace rus

#ifdef RUS_PAWN_TEST

#include <cassert>

int main(int argc, char **argv) {
    // single white movement
    assert(rus::pawn::move1(
        rus::white_idx,
        rus::board::Piece_board(0x20004900),
        ~rus::board::Piece_board(0x20004900)
    ) == 0x2000490000);

    // single black movement
    assert(rus::pawn::move1(
        rus::black_idx,
        rus::board::Piece_board(0x11044000000000),
        ~rus::board::Piece_board(0x11044000000000)
    ) == 0x110440000000);

    // double white movement
    assert(rus::pawn::move2(
        rus::white_idx,
        rus::board::Piece_board(0x1000004500),
        ~rus::board::Piece_board(0x1000004500)
    ) == 0x45000000);

    // double black movement
    assert(rus::pawn::move2(
        rus::black_idx,
        rus::board::Piece_board(0x23000800000000),
        ~rus::board::Piece_board(0x23000800000000)
    ) == 0x2300000000);

    // white attacks
    assert(rus::pawn::move2(
        rus::white_idx,
        rus::board::Piece_board(0x1000004500),
        ~rus::board::Piece_board(0x1000004500)
    ) == 0x45000000);

    // black attacks
    assert(rus::pawn::move2(
        rus::black_idx,
        rus::board::Piece_board(0x47008000080000),
        ~rus::board::Piece_board(0x47008000080000)
    ) == 0x4700000000);


    return 0;
}

#endif
