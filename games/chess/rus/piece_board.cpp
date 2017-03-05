#include "piece_board.hpp"

namespace rus {
namespace board {

Piece_board step_n(const Piece_board bb) { return bb << 8; }
Piece_board step_e(const Piece_board bb) { return (bb << 1) & notAFile; }
Piece_board step_s(const Piece_board bb) { return bb >> 8; }
Piece_board step_w(const Piece_board bb) { return (bb >> 1) & notHFile; }

// step north-east, south-east, north-west, south-west
Piece_board step_ne(const Piece_board bb) { return (bb << 9) & notAFile; }
Piece_board step_se(const Piece_board bb) { return (bb >> 7) & notAFile; }
Piece_board step_nw(const Piece_board bb) { return (bb << 7) & notHFile; }
Piece_board step_sw(const Piece_board bb) { return (bb >> 9) & notHFile; }

void pre_process() {
}

}
}

