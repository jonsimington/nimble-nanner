#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include "board.hpp"
#include "enums.hpp"

namespace rus {

class State {
public:

    board::Board empty; // preprocessed
    board::Board occupied; // preprocessed

    //TODO: Move any boards to private, add accessor to pass enums
    board::Board any_piece[Player_size];

    //TODO: Move boards to private, add accessor to pass enums
    board::Board boards[Player_size][Piece_size];

    //TODO: Move castling to private, add accessor to pass enums
    bool castling_ability[Player_size][Castle_size];
    bool ep_able;
    Player player;
    Position ep_pos;

    Piece find_piece_type(const Position sq) const;

    void print() const;
    void pre_process();
    int material(const Player player) const;
    void clear();

};

}

#endif