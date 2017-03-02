#ifndef RUS_PIECE_BOARD_HPP
#define RUS_PIECE_BOARD_HPP

#include <cstdint>

namespace rus {

using Piece_board = uint64_t;
const Piece_board empty_board = Piece_board(0);
const Piece_board full_board = ~ Piece_board(0);

// lookups
Piece_board knightMovesTbl[64];
Piece_board kingMovesTbl[64];

// files
const Piece_board aFile = Piece_board(0x0101010101010101);
const Piece_board hFile = Piece_board(0x8080808080808080);

const Piece_board notAFile = Piece_board(~aFile);
const Piece_board notHFile = Piece_board(~hFile);

const Piece_board notABFile = Piece_board(0xfcfcfcfcfcfcfcfc);
const Piece_board notGHFile = Piece_board(0x3f3f3f3f3f3f3f3f);

// ranks
const Piece_board rank4 = Piece_board(0x00000000FF000000);
const Piece_board rank5 = Piece_board(0x000000FF00000000);

// step north, east, south, west
Piece_board step_n(const Piece_board bb) { return bb << 8; }
Piece_board step_e(const Piece_board bb) { return (bb << 1) & notAFile; }
Piece_board step_s(const Piece_board bb) { return bb >> 8; }
Piece_board step_w(const Piece_board bb) { return (bb >> 1) & notHFile; }

// step north-east, south-east, north-west, south-west
Piece_board step_ne(const Piece_board bb) { return (bb << 9) & notAFile; }
Piece_board step_se(const Piece_board bb) { return (bb >> 7) & notAFile; }
Piece_board step_nw(const Piece_board bb) { return (bb << 7) & notHFile; }
Piece_board step_sw(const Piece_board bb) { return (bb >> 9) & notHFile; }




}

#endif