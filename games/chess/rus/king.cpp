
#include "bit_scan.hpp"
#include "king.hpp"
#include "enums.hpp"

namespace rus {
namespace king {

board::Piece_board movesTbl[64];

board::Piece_board calcMoves(board::Piece_board kings) {
    board::Piece_board atks = board::step_e(kings) | board::step_w(kings);
    kings |= atks;
    atks |= board::step_n(kings) | board::step_s(kings);
    return atks;
}

board::Piece_board moves(board::Piece_board kings, board::Piece_board any_friendly) {
    board::Piece_board mvs = board::empty_board;
    if(kings) do {
        int idx = bit_scan::scanForward(kings);
        mvs |= movesTbl[idx];
    } while(kings &= kings - 1);
    mvs &= ~any_friendly;
    return mvs;
}

board::Piece_board captures(const board::Piece_board kings, const board::Piece_board any_friendly, const board::Piece_board any_enemy) {
    return moves(kings, any_friendly) & any_enemy;
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


#ifdef RUS_KING_TEST

#include <cassert>
#include <iostream>

int main(int argc, char **argv) {

    // verify calcMoves
    assert(rus::king::calcMoves(0x1) == 0x302);
    assert(rus::king::calcMoves(0x8000000) == 0x1c141c0000);
    assert(rus::king::calcMoves(0x8000) == 0xc040c0);

    rus::king::pre_process();

    // verify movesTbl
    assert(rus::king::movesTbl[rus::d1] == 0x1c14);
    assert(rus::king::movesTbl[rus::h8] == 0x40c0000000000000);
    assert(rus::king::movesTbl[rus::c5] == 0xe0a0e000000);

    // verify moves()
    assert(rus::king::moves(0x8000000400004000) == 0x40c00e0a0ee0a0e0);


    return 0;
}

#endif