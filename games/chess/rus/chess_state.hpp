#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include "piece_board.hpp"
#include "enums.hpp"

namespace rus {

class Chess_state {
public:
    board::Piece_board boards[player_enum_size][board_enum_size];
};

}

#endif