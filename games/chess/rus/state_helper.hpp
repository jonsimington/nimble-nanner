#ifndef RUS_CHESS_BOARD_HPP
#define RUS_CHESS_BOARD_HPP

#include "chess_state.hpp"
#include "piece_board.hpp"

namespace rus {

class State_helper {
public:
    Chess_state state;

    board::Piece_board empty; // preprocessed
    board::Piece_board occupied; // preprocessed
    board::Piece_board any_piece[player_enum_size];

    void pre_process();
};

}

#endif