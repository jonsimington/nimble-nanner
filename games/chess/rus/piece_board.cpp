#include <cassert>

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


int idxFromRankFile(const int rank, const std::string file) {
    int file_offset = file[0] - 'a';
    int rank_offset = rank - 1;
    return file_offset + (8 * rank_offset);
}

void rankFileFromIdx(int idx, int & rank, std::string & file) {
    const std::string files[] = { "a", "b", "c", "d", "e", "f", "g", "h" };

    file = files[idx % 8];
    rank = (idx / 8) + 1;
}

Piece_board from_idx(const int idx) {
    assert(idx >= 0);
    assert(idx < 64);

    return Piece_board(0x1) << idx;
}

}
}

