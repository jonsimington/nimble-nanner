#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include "piece_board.hpp"
#include "enums.hpp"

// Forward declare AI
namespace cpp_client {
namespace chess {
class AI;
}
}

namespace rus {

class Chess_state {
public:
    board::Piece_board boards[player_enum_size][board_enum_size];

    void clear();
    void construct_from(cpp_client::chess::AI* ai);
};

}

#endif