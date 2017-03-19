#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include "board.hpp"
#include "enums.hpp"

namespace rus {

class State {
public:
    board::Board empty; // preprocessed
    board::Board occupied; // preprocessed
    board::Board any_piece[player_enum_size];
    board::Board boards[player_enum_size][board_enum_size];

    void print() const;
    void pre_process();
    int material(const Player_enum player) const;
    void clear();
};

}

#endif