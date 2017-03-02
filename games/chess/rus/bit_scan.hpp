#ifndef RUS_BIT_SCAN
#define RUS_BIT_SCAN

#include <cassert>
#include "piece_board.hpp"

// https://chessprogramming.wikispaces.com/BitScan
// https://chessprogramming.wikispaces.com/De+Bruijn+sequence
// De Bruijn multiplication for the purposes of isolated least significant bitscanning
const Piece_board debruijn64 = Piece_board(0x03f79d71b4cb0a89);
const int index64[64] = {
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};
 
/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int bitScanForward(Piece_board bb) {
   assert (bb != 0);
   return index64[((bb ^ (bb-1)) * debruijn64) >> 58];
}