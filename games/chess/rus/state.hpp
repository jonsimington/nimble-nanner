#ifndef RUS_CHESS_STATE_HPP
#define RUS_CHESS_STATE_HPP

#include <vector>

#include "board.hpp"
#include "enums.hpp"
#include "move.hpp"

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

    // if for the last eight moves no capture, promotions, or pawn movement has happened and moves 0, 1, 2, and 3
    // are identical to moves 4, 5, 6, and 7
    std::vector<Move> last_moves;

    // respectively, then a draw has occurred.
    int turns_since_stuff;

    board::Board & get_board(const Player player, const Piece piece);
    const board::Board & get_board(const Player player, const Piece piece) const;

    board::Board & get_any(const Player player);
    const board::Board & get_any(const Player player) const;

    /**
     * Determines if the current state is actually a draw
     * @return is the current state actually a draw
     */
    bool is_draw() const;

    Piece find_piece_type(const Position sq) const;

    void print() const;
    void pre_process();
    int heuristic() const;
    void clear();

};

}

#endif