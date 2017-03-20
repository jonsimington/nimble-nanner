#include <cstring>

#include "state.hpp"
#include "bit_scan.hpp"

namespace rus {

    Piece State::find_piece_type(const Position sq) const {
        auto sq_bb = board::position(sq);
        for(auto p = Piece::start; p != Piece::end; ++p) {
            if(boards[to_i(Player::white)][to_i(p)] & sq_bb > 0) return p;
            if(boards[to_i(Player::black)][to_i(p)] & sq_bb > 0) return p;
        }

        assert(false); // Did not find piece
        return Piece::end;
    }

    void State::print() const {

    }

    void State::pre_process() {
        memset(any_piece, 0x00, sizeof(any_piece));
        for(auto player = Player::start; player != Player::end; ++player) {
            for(auto piece = Piece::start; piece != Piece::end; ++piece) {
                any_piece[to_i(player)] |= boards[to_i(player)][to_i(piece)];
            }
            occupied |= any_piece[to_i(player)];
        }
        empty = ~occupied;
    }

    int State::material(const Player player) const {
        const int mat_vals[Piece_size] = {
                1, // pawn
                3, // knight
                3, // bishop
                5, // rook
                9, // queen
                1000000, // king
        };

        int mat = 0;
        for(auto piece = Piece::start; piece != Piece::end; ++piece) {
            board::Board bb = boards[to_i(player)][to_i(piece)];
            if(bb) {
                do {
                    mat += mat_vals[to_i(piece)];
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