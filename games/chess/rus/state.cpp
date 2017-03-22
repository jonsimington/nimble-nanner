#include <cstring>
#include <type_traits>

#include "state.hpp"
#include "bit_scan.hpp"

namespace rus {

    board::Board & State::get_board(const Player player, const Piece piece) {
        return boards[static_cast< std::underlying_type_t<Player> >(player)]
                     [static_cast< std::underlying_type_t<Piece> >(piece)];
    }
    const board::Board & State::get_board(const Player player, const Piece piece) const {
        return boards[static_cast< std::underlying_type_t<Player> >(player)]
                     [static_cast< std::underlying_type_t<Piece> >(piece)];
    }

    board::Board & State::get_any(const Player player) {
        return any_piece[static_cast< std::underlying_type_t<Player> >(player)];
    }
    const board::Board & State::get_any(const Player player) const {
        return any_piece[static_cast< std::underlying_type_t<Player> >(player)];
    }

    bool State::is_draw() const {
        if(last_moves.size() < 8) return false;

        if(last_moves[0] == last_moves[2] && last_moves[2] == last_moves[4]) return true;
        if(turns_since_stuff > 8) return true;

        return false;
    }

    Piece State::find_piece_type(const Position sq) const {
        auto sq_bb = board::position(sq);
        for(auto p = Piece::start; p != Piece::end; ++p) {
            if(boards[to_i(Player::white)][to_i(p)] & sq_bb) return p;
            if(boards[to_i(Player::black)][to_i(p)] & sq_bb) return p;
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

    int State::heuristic() const {
        const int mat_vals[Piece_size] = {
                1, // pawn
                3, // knight
                3, // bishop
                5, // rook
                9, // queen
                1000000, // king
        };

        int mat_white = 0;
        int mat_black = 0;

        for(auto piece = Piece::start; piece != Piece::end; ++piece) {
            auto wbb = get_board(Player::white, piece);
            auto bbb = get_board(Player::black, piece);

            const auto money = mat_vals[static_cast<std::underlying_type_t<Piece>>(piece)];

            if(wbb) do {
                mat_white += money;
            } while(wbb &= wbb-1);

            if(bbb) do {
                mat_black += money;
            } while(bbb &= bbb-1);
        }

        return mat_white - mat_black;
    }

    void State::clear() {
        memset(any_piece, 0x00, sizeof(any_piece));
        memset(boards, 0x00, sizeof(boards));
        empty = 0x00;
        empty = ~occupied;
    }

}