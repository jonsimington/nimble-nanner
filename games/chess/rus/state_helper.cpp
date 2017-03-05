#include <cassert>
#include <iostream>

#include "state_helper.hpp"
#include "../ai.hpp"

#include "pawn.hpp"
#include "knight.hpp"
#include "bishop.hpp"
#include "rook.hpp"
#include "queen.hpp"
#include "king.hpp"
#include "bit_scan.hpp"

namespace rus {

void State_helper::pre_process() {
    this->occupied = 0;
    for (int player = 0; player < player_enum_size; player++) {
        this->any_piece[player] = 0;

        for (int piece = 0; piece < board_enum_size; piece++) {
            this->any_piece[player] |= this->state.boards[player][piece];
        }

        this->occupied |= this->any_piece[player];
    }

    empty = ~occupied;
}

void State_helper::clear() {
    for(int player = 0; player < player_enum_size; player++) {
        this->any_piece[player] = 0x0;
    }

    this->occupied = 0x0;
    this->empty = ~this->occupied;
}

void State_helper::construct_from(cpp_client::chess::AI *ai) {
    this->clear();
    this->state.construct_from(ai);
    this->pre_process();
}

void State_helper::applyMove(const Move move) {
    board::Piece_board fromBB = board::Piece_board(0x1) << move.from;
    board::Piece_board toBB = board::Piece_board(0x1) << move.to;

    std::cout << "fromBB" << fromBB << std::endl;

    // make sure it exists
    assert(this->state.boards[move.player][move.piece] & fromBB > 0);

    this->state.boards[move.player][move.piece] &= ~fromBB;


    // capture any guys in my way
    for(int i = 0; i < board_enum_size; i++) {
        this->state.boards[white_idx][move.piece] &= ~toBB;
        this->state.boards[black_idx][move.piece] &= ~toBB;
    }

    this->state.boards[move.player][move.piece] |= toBB;

    this->pre_process();
}

State_helper State_helper::forkMove(const Move move) {
    State_helper newState(*this);
    newState.applyMove(move);

    return newState;
}

bool State_helper::isInCheck(const Player_enum player) {
    Player_enum enemy = player == white_idx ? black_idx : white_idx;
    board::Piece_board enemyMoves = board::empty_board;

    enemyMoves |= pawn::captures(enemy, this->state.boards[enemy][pawn_idx], this->any_piece[player]);
    enemyMoves |= knight::captures(this->state.boards[enemy][knight_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= bishop::captures(this->state.boards[enemy][bishop_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= rook::captures(this->state.boards[enemy][rook_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= queen::captures(this->state.boards[enemy][queen_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= king::captures(this->state.boards[enemy][king_idx], this->any_piece[enemy], this->any_piece[player]);

    return (enemyMoves & this->state.boards[player][king_idx]) > 0;

}

std::vector<Move> State_helper::pieceMoves(const Player_enum player, const int idx) {
    assert(idx >= 0);
    assert(idx < 64);

    std::vector<Move> moves;
    std::vector<Move> legal_moves;

    board::Piece_board curr = board::Piece_board(0x1) << idx;

    // Assert piece to be moved is owned by requested player
    assert(this->any_piece[player] & curr > 0);

    if(curr & this->empty) return legal_moves; // idx isn't actually a piece


    Board_enum  type;

    for(int i = 0; i < board_enum_size; i++) {
        if(this->state.boards[player][i] & curr) {
            type = static_cast<Board_enum >(i);
            break;
        }
    }

    // psudo legal moves
    board::Piece_board pl = 0x0;

    switch(type) {
        case pawn_idx:
            pl |= pawn::move1(player, curr, this->empty);
            pl |= pawn::move2(player, curr, this->empty);
            pl |= pawn::captures(player, curr, this->any_piece[player ^ 0x1]);
            break;
        case knight_idx:
            pl |= knight::moves(curr, this->any_piece[player]);
            break;
        case bishop_idx:
            pl |= bishop::moves(curr, this->any_piece[player], this->any_piece[player ^ 0x1]);
            break;
        case rook_idx:
            pl |= rook::moves(curr, this->any_piece[player], this->any_piece[player ^ 0x1]);
            break;
        case queen_idx:
            pl |= queen::moves(curr, this->any_piece[player], this->any_piece[player ^ 0x1]);
            break;
        case king_idx:
            pl |= king::moves(curr, this->any_piece[player]);
            break;
    }


    bit_scan::forEachBit(pl, [&moves, player, type](int i){
        Move m;
        m.player = player;
        m.from = i;
        m.to = i;
        m.piece = type;
        moves.push_back(m);
    });

    for(auto& move: moves) {
        auto state = this->forkMove(move);

        if(! state.isInCheck(move.player)) {
            legal_moves.push_back(move);
        }
    }

    return legal_moves;
}

std::vector<Move> State_helper::playerMoves(const Player_enum player) {
    std::vector<Move> moves;

    bit_scan::forEachBit(this->any_piece[player], [this, &moves, player](int i){
        auto m = this->pieceMoves(player, i);
        moves.insert(moves.begin(), m.begin(), m.end());
    });

    return moves;
}

}
