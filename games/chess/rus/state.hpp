#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include "board.hpp"
#include "player.hpp"
#include "piece.hpp"

namespace rus {

class State {
public:
    enum Castling_enum {
        queen_side,
        king_side,

        castling_enum_size,
    };

    board::Board empty; // preprocessed
    board::Board occupied; // preprocessed
    board::Board any_piece[Player::size];
    board::Board boards[Player::size][Piece::size];

    bool castling_ability[Player::size][castling_enum_size];
    bool ep_able;
    Player::Type player;
    Position::Type ep_pos;

    Piece::Type find_piece_type(const Position::Type sq) const;

    void print() const;
    void pre_process();
    int material(const Player::Type player) const;
    void clear();

};

}

#endif