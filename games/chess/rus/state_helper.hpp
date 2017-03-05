#ifndef RUS_CHESS_BOARD_HPP
#define RUS_CHESS_BOARD_HPP

#include <vector>
#include "chess_state.hpp"
#include "piece_board.hpp"
#include "move.hpp"

namespace cpp_client {
namespace chess {
    class AI;
}
}

namespace rus {

class State_helper {
public:
    Chess_state state;

    board::Piece_board empty; // preprocessed
    board::Piece_board occupied; // preprocessed
    board::Piece_board any_piece[player_enum_size];

    void pre_process();
    void clear();
    void construct_from(cpp_client::chess::AI* ai);

    void applyMove(const Move move);
    State_helper forkMove(const Move move);

    bool isInCheck(const Player_enum player);

    std::vector<Move> pieceMoves(const Player_enum player, const int idx);
    std::vector<Move> playerMoves(const Player_enum player);
};

}

#endif