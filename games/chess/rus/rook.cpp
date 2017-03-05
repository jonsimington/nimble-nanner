#include <cassert>

#include "rook.hpp"
#include "bit_scan.hpp"
#include "fill.hpp"

namespace rus {
namespace rook {

board::Piece_board raysTbl[ray_direction_size][64];

board::Piece_board calcRay(const int sq, const int dir) {
    board::Piece_board me = board::Piece_board(0x1) << sq;
    switch(dir) {
        case nort: return fill::nortFill(me) & ~me;
        case east: return fill::eastFill(me) & ~me;
        case sout: return fill::soutFill(me) & ~me;
        case west: return fill::westFill(me) & ~me;
        default: assert(false);
    }
    return 0x0;
}

board::Piece_board calcPositiveRayAttacks(int sq, const int dir, board::Piece_board occupied) {
    assert(dir != sout);
    assert(dir != west);

    board::Piece_board attacks = raysTbl[dir][sq];
    board::Piece_board blocker = attacks & occupied;
    if ( blocker ) {
        sq = bit_scan::scanForward(blocker);
        attacks ^= raysTbl[dir][sq];
    }
    return attacks;
}

board::Piece_board calcNegativeRayAttacks(int sq, const int dir, board::Piece_board occupied) {
    assert(dir != nort);
    assert(dir != east);

    board::Piece_board attacks = raysTbl[dir][sq];
    board::Piece_board blocker = attacks & occupied;
    if ( blocker ) {
        sq = bit_scan::scanReverse(blocker);
        attacks ^= raysTbl[dir][sq];
    }
    return attacks;
}

board::Piece_board moves(board::Piece_board rooks, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    board::Piece_board occupied = any_friendly | any_enemy;
    board::Piece_board mvs = board::empty_board;
    if(rooks) do {
        int idx = bit_scan::scanForward(rooks);
        mvs |= calcPositiveRayAttacks(idx, nort, occupied);
        mvs |= calcPositiveRayAttacks(idx, east, occupied);
        mvs |= calcNegativeRayAttacks(idx, west, occupied);
        mvs |= calcNegativeRayAttacks(idx, sout, occupied);
    } while(rooks &= rooks-1);
    mvs &= ~any_friendly;
    return mvs;
}

board::Piece_board captures(const board::Piece_board rooks, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    return moves(rooks, any_friendly, any_enemy) & any_enemy;
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

#ifdef RUS_ROOK_TEST

#include "assert.hpp"

int main(int argc, char **argv) {

    using namespace rus::rook;

    // verify calcRay
    RUS_ASSERT_EQ(calcRay(rus::b4, nort), 0x202020200000000);
    RUS_ASSERT_EQ(calcRay(rus::g7, east), 0x80000000000000);
    RUS_ASSERT_EQ(calcRay(rus::d5, sout), 0x8080808);
    RUS_ASSERT_EQ(calcRay(rus::h1, west), 0x7f);

    pre_process();

    // verify raysTbl
    RUS_ASSERT_EQ(raysTbl[nort][rus::b4], 0x202020200000000);
    RUS_ASSERT_EQ(raysTbl[east][rus::g7], 0x80000000000000);
    RUS_ASSERT_EQ(raysTbl[sout][rus::d5], 0x8080808);
    RUS_ASSERT_EQ(raysTbl[west][rus::h1], 0x7f);

    // verify calcPositiveRayAttacks
    RUS_ASSERT_EQ(calcPositiveRayAttacks(rus::c4, nort, 0x4001a0004000000), 0x404040400000000);
    RUS_ASSERT_EQ(calcPositiveRayAttacks(rus::c4, east, 0x4001a0004000000), 0xf8000000);

    // verify calcNegativeRayAttacks
    RUS_ASSERT_EQ(calcNegativeRayAttacks(rus::g7, sout, 0x44000080041401), 0x404040404040);
    RUS_ASSERT_EQ(calcNegativeRayAttacks(rus::g7, west, 0x44000080041401), 0x3c000000000000);

    // verify move
    RUS_ASSERT_EQ(moves(0x8000000, 0x4a000000, 0x800000020000000), 0x808080834080808);

    return 0;
}

#endif