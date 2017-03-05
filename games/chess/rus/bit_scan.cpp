#include <cassert>

#include "bit_scan.hpp"
#include "piece_board.hpp"

namespace rus {
namespace bit_scan {

// https://chessprogramming.wikispaces.com/BitScan

int scanForward(board::Piece_board bb) {
    assert (bb != 0);
    return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
}

int scanReverse(board::Piece_board bb) {
    assert (bb != 0);
    bb |= bb >> 1;
    bb |= bb >> 2;
    bb |= bb >> 4;
    bb |= bb >> 8;
    bb |= bb >> 16;
    bb |= bb >> 32;
    return index64[(bb * debruijn64) >> 58];
}

}
}