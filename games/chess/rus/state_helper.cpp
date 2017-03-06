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
    this->occupied = 0x0;
    this->any_piece[white_idx] = 0x0;
    this->any_piece[black_idx] = 0x0;

    for (int piece = 0; piece < board_enum_size; piece++) {
        this->any_piece[white_idx] |= this->state.boards[white_idx][piece];
        this->any_piece[black_idx] |= this->state.boards[black_idx][piece];
    }

    this->occupied = this->any_piece[white_idx] | this->any_piece[black_idx];

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

void State_helper:: applyMove(const Move move) {
    board::Piece_board fromBB = board::from_idx(move.from);
    board::Piece_board toBB = board::from_idx(move.to);

    // make sure it exists
    assert((this->state.boards[move.player][move.piece] & fromBB) > 0);

    // capture any guys in my way
    for(int i = 0; i < board_enum_size; i++) {
        this->state.boards[white_idx][i] &= ~fromBB;
        this->state.boards[black_idx][i] &= ~fromBB;
        this->state.boards[white_idx][i] &= ~toBB;
        this->state.boards[black_idx][i] &= ~toBB;
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
    Player_enum enemy = static_cast<Player_enum>(player ^ 0x1);
    board::Piece_board enemyMoves = board::empty_board;

    std::cout << "Checking checks..." << std::endl;

    enemyMoves |= pawn::captures(enemy, this->state.boards[enemy][pawn_idx], this->any_piece[player]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by pawn" << std::endl;
    }

    enemyMoves |= knight::moves(this->state.boards[enemy][knight_idx], this->any_piece[enemy]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by knight" << std::endl;
    }

    enemyMoves |= bishop::moves(this->state.boards[enemy][bishop_idx], this->any_piece[enemy], this->any_piece[player]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by bishop" << std::endl;
    }

    enemyMoves |= rook::moves(this->state.boards[enemy][rook_idx], this->any_piece[enemy], this->any_piece[player]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by rook" << std::endl;
    }

    enemyMoves |= queen::moves(this->state.boards[enemy][queen_idx], this->any_piece[enemy], this->any_piece[player]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by queen" << std::endl;
    }

    enemyMoves |= king::moves(this->state.boards[enemy][king_idx], this->any_piece[enemy]);
    if(enemyMoves & this->state.boards[player][king_idx]) {
        std::cout << "checked by king" << std::endl;
    }

    std::cout << "Enemy moves: " << enemyMoves << std::endl;
    std::cout << "King pos: " << this->state.boards[player][king_idx] << std::endl;
    return (enemyMoves & this->state.boards[player][king_idx]) > 0;

}

std::vector<Move> State_helper::pieceMoves(const Player_enum player, const int idx) {
    assert(idx >= 0);
    assert(idx < 64);

    std::vector<Move> moves;
    std::vector<Move> legal_moves;

    board::Piece_board curr = board::Piece_board(0x1) << idx;

    // Assert piece to be moved is owned by requested player
    assert((this->any_piece[player] & curr) > 0);

    if(curr & this->empty) return legal_moves; // idx isn't actually a piece


    Board_enum  type = static_cast<Board_enum>(-1);

    for(int i = 0; i < board_enum_size; i++) {
        if(this->state.boards[player][i] & curr) {
            type = static_cast<Board_enum >(i);
            break;
        }
    }

    assert(type != -1);

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
        case king_idx: {
            auto m = king::moves(curr, this->any_piece[player]);
            std::cout << "King Moves: " << m << std::endl;
            pl |= m;
            } break;
        default:
            assert(false); // ???
    }


    bit_scan::forEachBit(pl, [&moves, player, type, idx](int i){
        Move m;
        m.player = player;
        m.from = idx;
        m.to = i;
        m.piece = type;
        moves.push_back(m);
    });

    for(auto& move: moves) {
        State_helper s = this->forkMove(move);

        if(! s.isInCheck(move.player)) {
            legal_moves.push_back(move);
        }
    }

    return legal_moves;
}

std::vector<Move> State_helper::playerMoves(const Player_enum player) {
    std::vector<Move> moves;

    bit_scan::forEachBit(this->any_piece[player], [this, &moves, player](int i){
        auto m = this->pieceMoves(player, i);
        std::cout << "Inserting " << m.size() << " moves..." << std::endl;
        moves.insert(moves.begin(), m.begin(), m.end());
    });
    std::cout << "Got " << moves.size() << " moves" << std::endl;
    return moves;
}

}
