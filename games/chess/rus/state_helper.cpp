#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

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

    std::stringstream ss(ai->game->fen);

    std::string section;

    // piece placement
    ss >> section;

    // side to move
    ss >> section;

    // castling ability
    ss >> section;

    this->castlingAbility[white_idx][king_side] = (section.find("K") != std::string::npos);
    this->castlingAbility[white_idx][queen_side] = (section.find("Q") != std::string::npos);

    this->castlingAbility[black_idx][king_side] = (section.find("k") != std::string::npos);
    this->castlingAbility[black_idx][queen_side] = (section.find("q") != std::string::npos);

    // en passant target square
    ss >> section;

    if(section == "-") {
        this->enPessantable = false;
    }
    else {
        this->enPessantIdx = board::idxFromRankFile(section[1] - '1' + 1, std::to_string(section[0]));
    }


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

    enemyMoves |= pawn::captures(enemy, this->state.boards[enemy][pawn_idx], this->any_piece[player]);
    enemyMoves |= knight::moves(this->state.boards[enemy][knight_idx], this->any_piece[enemy]);
    enemyMoves |= bishop::moves(this->state.boards[enemy][bishop_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= rook::moves(this->state.boards[enemy][rook_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= queen::moves(this->state.boards[enemy][queen_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= king::moves(this->state.boards[enemy][king_idx], this->any_piece[enemy]);
    return (enemyMoves & this->state.boards[player][king_idx]) > 0;

}

std::vector<Move> State_helper::castlingMoves(const Player_enum player) {
    Player_enum enemy = static_cast<Player_enum>(player ^ 0x1);
    board::Piece_board enemyMoves = board::empty_board;

    std::vector<Move> moves;

    // get all enemy attacks
    enemyMoves |= pawn::attacks(enemy, this->state.boards[enemy][pawn_idx]);
    enemyMoves |= knight::moves(this->state.boards[enemy][knight_idx], this->any_piece[enemy]);
    enemyMoves |= bishop::moves(this->state.boards[enemy][bishop_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= rook::moves(this->state.boards[enemy][rook_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= queen::moves(this->state.boards[enemy][queen_idx], this->any_piece[enemy], this->any_piece[player]);
    enemyMoves |= king::moves(this->state.boards[enemy][king_idx], this->any_piece[enemy]);

    auto kingBB = this->state.boards[player][king_idx];

    //check if king in check
    if(kingBB & enemyMoves) return moves;

    //check both sides
    for(int side = 0; side < castling_enum_size; side++) {
        Move m;
        // the king and the relevant rook must not be moved, considered as castling rights inside a chess position
        if (!this->castlingAbility[player][side]) continue;
        if (side == queen_side) {
            // 5) There are pieces standing between your king and rook.
            if (this->occupied & (player==white_idx?0x0e:0xe00000000000000)) continue;

            // The king moves through a square that is attacked by a piece of the opponent.
            if (enemyMoves & (player==white_idx?0x30:0x3000000000000000)) continue;

            m.player = player;
            m.piece = king_idx;
            m.from = (player==white_idx)?e1:e8;
            m.to = (player==white_idx)?c1:c8;

            //std::cout << "Adding castling move " << m.from << "->" << m.to << std::endl;

        } else if (side == king_side) {
            // 5) There are pieces standing between your king and rook.
            if (this->occupied & (player==white_idx?0x60:0x6000000000000000)) continue;

            // The king moves through a square that is attacked by a piece of the opponent.
            if (enemyMoves & ((player==white_idx)?0x60:0x6000000000000000)) continue;

            m.player = player;
            m.piece = king_idx;
            m.from = (player==white_idx)?e1:e8;
            m.to = (player==white_idx)?g1:g8;

            //std::cout << "Adding castling move " << m.from << "->" << m.to << std::endl;
        }
        moves.push_back(m);
    }

    return moves;
}

std::vector<Move> State_helper::enPassantMoves(const Player_enum player) {
    Player_enum enemy = static_cast<Player_enum>(player ^ 0x1);
    std::vector<Move> moves;

    if(! this->enPessantable) return moves;

    auto epBB = board::from_idx(enPessantIdx);
    board::Piece_board pawns = this->state.boards[player][pawn_idx];

    board::Piece_board mvs = board::empty_board;
    if(pawns) do {
        int idx = bit_scan::scanForward(pawns);
        auto idxBB = board::from_idx(idx);

        // if attack
        if(pawn::attacks(player, idxBB) & epBB) {
            // log en passant
            Move m;
            m.player = player;
            m.from = idx;
            m.to = this->enPessantIdx;
            m.piece = pawn_idx;

            moves.push_back(m);
            return moves; // there can only be one en passant anyways
        }

    } while(pawns &= pawns-1);

    return moves;
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
        case king_idx:
            pl |= king::moves(curr, this->any_piece[player]);
            break;
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
        moves.insert(moves.begin(), m.begin(), m.end());
    });

    // handle castling
    auto pl_moves = this->castlingMoves(player);

    // handle en passant
    auto ep_moves = this->enPassantMoves(player);
    pl_moves.insert(pl_moves.begin(), ep_moves.begin(), ep_moves.end());
    for(auto& move: pl_moves) {
        State_helper s = this->forkMove(move);

        if(! s.isInCheck(move.player)) {
            moves.push_back(move);
        }
    }

    return moves;
}

}
