#ifndef RUS_PAWN_HPP
#define RUS_PAWN_HPP

#include "Piece_board.hpp"

namespace rus {
namespace pawn {

// Movements
Piece_board wMove1(const Piece_board w_pawn, const Piece_board, empty) {
    return step_n(w_pawn) & empty;
}
Piece_board bMove1(const Piece_board b_pawn, const Piece_board, empty) {
    return step_s(b_pawn) & empty;
}
Piece_board wMove2(const Piece_board w_pawn, const Piece_board, empty) const {
    Piece_board singlePushs = wMoveSingle(w_pawn, empty);
    return step_n(singlePushs) & empty & rank4;
}
Piece_board bMove2(const Piece_board b_pawn, const Piece_board, empty) const {
    Piece_board singlePushs = bMoveSingle(b_pawn, empty);
    return step_s(singlePushs) & empty & rank5;
}

// Attacks
Piece_board wAttacks(const Piece_board w_pawn) {
   return step_ne(w_pawn) | step_nw(w_pawn);
}
Piece_board bAttacks(const Piece_board w_pawn) {
    return step_se(w_pawn) | step_sw(w_pawn);
}

// Captures
Piece_board wCaptures(const Piece_board w_pawns, const Piece_board b_any) {
   return wAttacks(bpieces) & b_any;
}
Piece_board bCaptures(const Piece_board b_pawns, const Piece_board w_any) {
   return bAttacks(bpieces) & w_any;
}


} // namespace pawn
} // namespace rus



#endif