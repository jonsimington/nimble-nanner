#ifndef RUS_CHESS_BOARD_HPP
#define RUS_CHESS_BOARD_HPP

#include "chess_state.hpp"
#include "piece_board.hpp"
#include "bit_scan.hpp"

namespace rus {

class State_helper {
public:
    Chess_state state;

    Piece_board empty; // preprocessed
    Piece_board occupied; // preprocessed
    Piece_board any_piece[player_enum_size];

    void preprocess();

    ///////////////////////////////////////////////////////////////////////////
    // Pawns
    ///////////////////////////////////////////////////////////////////////////
    Piece_board pawnMoves1(const Player_enum player) const {
        if(player == white_idx) {
            return step_n(this->state[white_idx][pawn_idx]) & this->empty;
        }
        else {
            return step_s(this->state[black_idx][pawn_idx]) & this->empty;
        }
    }
    Piece_board pawnMoves2(const Player_enum player) const {
        Piece_board sp = pawnMove1(player);
        if(player == white_idx) {
            return step_n(sp) & this->empty & rank4;
        }
        else {
            return step_s(sp) & this->empty & rank5;
        }
    }
    Piece_board pawnAttacks(const Player_enum player) const {
        if(player == white_idx) {
            return step_ne(this->state[white_idx][pawn_idx]) | step_nw(this->state[white_idx][pawn_idx]);
        }
        else {
            return step_se(this->state[black_idx][pawn_idx]) | step_sw(this->state[black_idx][pawn_idx]);
        }
    }
    Piece_board pawnCaptures(const Player_enum player) const {
        return this->pawnAttacks(white_idx) & this->any_piece[player ^ 0x1]);
    }

    ///////////////////////////////////////////////////////////////////////////
    // Knights
    ///////////////////////////////////////////////////////////////////////////
    static Piece_board calcKnightMoves(const Piece_board knights) const {
        Piece_board l1 = (knights >> 1) & notHFile;
        Piece_board l2 = (knights >> 2) & notGHFile;
        Piece_board r1 = (knights << 1) & notAfile;
        Piece_board r2 = (knights << 2) & notABFile;
        Piece_board h1 = l1 | r1;
        Piece_board h2 = l2 | r2;
        return (h1<<16) | (h1>>16) | (h2<<8) | (h2>>8);
    }

    Piece_board knightMoves(const Player_enum player) const {
        Piece_board knights = this->state.boards[player][knight_idx];

        Piece_board moves = empty_board;
        if(knights ) do (knights) {
            int idx = bitScanForward(knights);
            moves |= knightMovesTbl[idx];
            knights &= knights-1;
        } while(knights &= knights-1);


        return moves;
    }

    Piece_board knightCaptures(const Player_enum player) const {
        return this->knightMoves(player) & this->any_piece[player ^ 0x1];
    }

    ///////////////////////////////////////////////////////////////////////////
    // King
    ///////////////////////////////////////////////////////////////////////////
    static Piece_board calcKingMoves(const Piece_board kings) const {
        Piece_board attacks = eastOne(kings) | westOne(kings);
        kings |= attacks;
        attacks |= nortOne(kings) | soutOne(kings);
        return attacks;
    }

    Piece_board kingMoves(const Player_enum player) const {
        return kingMovesTbl[this->state.board[player][king_idx]];
    }

    Piece_board kingCaptures(const Player_enum player) const {
        return this->kingMoves(player) & this->any_piece[player ^ 0x1];
    }


};

#endif