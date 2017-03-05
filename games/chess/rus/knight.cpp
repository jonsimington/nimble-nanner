#include "knight.hpp"
#include "bit_scan.hpp"

#include <iostream>

namespace rus {
namespace knight {

board::Piece_board movesTbl[64];

board::Piece_board calcMoves(const board::Piece_board knights) {
    board::Piece_board l1 = (knights >> 1) & board::notHFile;
    board::Piece_board l2 = (knights >> 2) & board::notGHFile;
    board::Piece_board r1 = (knights << 1) & board::notAFile;
    board::Piece_board r2 = (knights << 2) & board::notABFile;
    board::Piece_board h1 = l1 | r1;
    board::Piece_board h2 = l2 | r2;
    return (h1 << 16) | (h1 >> 16) | (h2 << 8) | (h2 >> 8);
}

board::Piece_board moves(board::Piece_board knights, board::Piece_board any_friendly) {
    board::Piece_board ms = board::empty_board;
    if(knights) do {
        int idx = bit_scan::scanForward(knights);
        ms |= movesTbl[idx];
    } while(knights &= knights-1);
    ms &= ~any_friendly;
    return ms;
}

board::Piece_board captures(const board::Piece_board knights, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    return moves(knights, any_friendly) & any_enemy;
}

void pre_process() {
    board::Piece_board bb = 0x1;
    for(int i = 0; i < 64; i++) {
        movesTbl[i] = calcMoves(bb);
        bb <<= 0x1;
    }
}

}
}

#ifdef RUS_KNIGHT_TEST

#include <cassert>
#include <iostream>
#include "enums.hpp"

int main(int argc, char **argv) {

    rus::knight::pre_process();

    // verify calc
    assert(rus::knight::calcMoves(0x40000000000) == 0xa1100110a000000);
    assert(rus::knight::calcMoves(0x80) == 0x402000);
    assert(rus::knight::calcMoves(0x81000000) == 0x422400244200);

    // verify table
    assert(rus::knight::movesTbl[rus::a1] == 0x20400);
    assert(rus::knight::movesTbl[rus::c6] == 0xa1100110a000000);
    assert(rus::knight::movesTbl[rus::f8] == 0x88500000000000);

    // verify moves()
    assert(rus::knight::moves(0x10000000004200) == 0x44004428a5180018);

    return 0;
}

#endif