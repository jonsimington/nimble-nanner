#include <cassert>

#include "bishop.hpp"
#include "bit_scan.hpp"
#include "fill.hpp"

namespace rus {
namespace bishop {

board::Piece_board calcRay(const int sq, const int dir) {
    board::Piece_board me = board::Piece_board(0x1) << sq;
    switch(dir) {
        case nort_west: return fill::noWeFill(me) & ~me;
        case nort_east: return fill::noEaFill(me) & ~me;
        case sout_west: return fill::soWeFill(me) & ~me;
        case sout_east: return fill::soEaFill(me) & ~me;
        default: assert(false);
    }
    return 0x1;
}

board::Piece_board calcPositiveRayAttacks(int sq, const int dir, board::Piece_board occupied) {
    assert(dir != sout_east);
    assert(dir != sout_west);

    board::Piece_board attacks = raysTbl[dir][sq];
    board::Piece_board blocker = attacks & occupied;
    if ( blocker ) {
        sq = bit_scan::scanForward(blocker);
        attacks ^= raysTbl[dir][sq];
    }
    return attacks;
}

board::Piece_board calcNegativeRayAttacks(int sq, const int dir, board::Piece_board occupied) {
    assert(dir != nort_east);
    assert(dir != nort_west);

    board::Piece_board attacks = raysTbl[dir][sq];
    board::Piece_board blocker = attacks & occupied;
    if ( blocker ) {
        sq = bit_scan::scanReverse(blocker);
        attacks ^= raysTbl[dir][sq];
    }
    return attacks;
}

board::Piece_board moves(board::Piece_board bishops, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    board::Piece_board occupied = any_friendly | any_enemy;
    board::Piece_board mvs = board::empty_board;
    if(bishops) do {
        int idx = bit_scan::scanForward(bishops);
        mvs |= calcPositiveRayAttacks(occupied, idx, nort_east);
        mvs |= calcPositiveRayAttacks(occupied, idx, nort_west);
        mvs |= calcNegativeRayAttacks(occupied, idx, sout_east);
        mvs |= calcNegativeRayAttacks(occupied, idx, sout_west);
        mvs &= ~any_friendly; // don't attack friendly
    } while(bishops &= bishops-1);
    return mvs;
}

board::Piece_board captures(const board::Piece_board bishops, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    return moves(bishops, any_friendly, any_enemy) & any_enemy;
}

void pre_process() {
    for (int i = 0; i < board_square_size; i++) {
        for (int dir = 0; dir < ray_direction_size; dir++) {
            raysTbl[dir][i] = calcRay(i, dir);
        }
    }
}

}
}

#ifdef RUS_BISHOP_TEST

#include "assert.hpp"

int main(int argc, char **argv) {
    rus::board::pre_process();

    using namespace rus::bishop;

    // verify calcRay
    RUS_ASSERT_EQ(calcRay(rus::b4, nort_east), 0x2010080400000000);
    RUS_ASSERT_EQ(calcRay(rus::g7, nort_west), 0x2000000000000000);
    RUS_ASSERT_EQ(calcRay(rus::d5, sout_west), 0x4020100);
    RUS_ASSERT_EQ(calcRay(rus::a8, sout_east), 0x2040810204080);

    rus::bishop::pre_process();

    // verify raysTbl
    RUS_ASSERT_EQ(raysTbl[rus::bishop::nort_east][rus::b4], 0x2010080400000000);
    RUS_ASSERT_EQ(raysTbl[rus::bishop::nort_west][rus::g7], 0x2000000000000000);
    RUS_ASSERT_EQ(raysTbl[rus::bishop::sout_west][rus::d5], 0x4020100);
    RUS_ASSERT_EQ(raysTbl[rus::bishop::sout_east][rus::a8], 0x2040810204080);

    // verify calcPositiveRayAttacks
    RUS_ASSERT_EQ(calcPositiveRayAttacks(rus::c4, nort_east, 0x1e0000000000), 0x100800000000);
    RUS_ASSERT_EQ(calcPositiveRayAttacks(rus::c4, nort_west, 0x1e0000000000), 0x10200000000);

    // verify calcNegativeRayAttacks
    RUS_ASSERT_EQ(calcNegativeRayAttacks(rus::g7, sout_east, 0x44000080041401), 0x800000000000);
    RUS_ASSERT_EQ(calcNegativeRayAttacks(rus::g7, sout_west, 0x44000080041401), 0x201008040000);

    return 0;
}

#endif