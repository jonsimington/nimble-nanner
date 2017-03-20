#include <cstring>

#include "state.hpp"
#include "bit_scan.hpp"

namespace rus {

    Piece::Type State::find_piece_type(const Position::Type sq) const {
        auto sq_bb = board::position(sq);
        for(Piece::Type p = 0; p < Piece::size; p++) {
            if(boards[Player::white][p] & sq_bb > 0) return p;
            if(boards[Player::black][p] & sq_bb > 0) return p;
        }

        assert(false); // Did not find piece
        return Piece::size;
    }

    void State::print() const {

    }

    void State::pre_process() {
        memset(any_piece, 0x00, sizeof(any_piece));
        for(int player = 0; player < Player::size; player++) {
            for(int piece = 0; piece < Piece::size; piece++) {
                any_piece[player] |= boards[player][piece];
            }
            occupied |= any_piece[player];
        }
        empty = ~occupied;
    }

    int State::material(const Player::Type player) const {
        const int mat_vals[Piece::size] = {
                1, // pawn
                3, // knight
                3, // bishop
                5, // rook
                9, // queen
                1000000, // king
        };

        int mat = 0;
        for(int piece = 0; piece < Piece::size; piece ++) {
            board::Board bb = boards[player][piece];
            if(bb) {
                do {
                    mat += mat_vals[piece];
                } while(bb &= bb-1);
            }
        }

        return mat;
    }

    void State::clear() {
        memset(any_piece, 0x00, sizeof(any_piece));
        memset(boards, 0x00, sizeof(boards));
        empty = 0x00;
        empty = ~occupied;
    }

}