#ifndef RUS_BIT_SCAN
#define RUS_BIT_SCAN

#include <cassert>
#include "piece_board.hpp"

namespace rus {
namespace bit_scan {

// https://chessprogramming.wikispaces.com/De+Bruijn+sequence
// De Bruijn multiplication for the purposes of isolated least significant bitscanning
const board::Piece_board debruijn64 = board::Piece_board(0x03f79d71b4cb0a89);
const int index64[64] = {
        0, 47, 1, 56, 48, 27, 2, 60,
        57, 49, 41, 37, 28, 16, 3, 61,
        54, 58, 35, 52, 50, 42, 21, 44,
        38, 32, 29, 23, 17, 11, 4, 62,
        46, 55, 26, 59, 40, 36, 15, 53,
        34, 51, 20, 43, 31, 22, 10, 45,
        25, 39, 14, 33, 19, 30, 9, 24,
        13, 18, 8, 12, 7, 6, 5, 63
};

/**
 * scanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int scanForward(board::Piece_board bb);

/**
 * scanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int scanReverse(board::Piece_board bb);

}
}

#endif