#include "chess_state.hpp"
#include "../ai.hpp"

namespace rus {

void Chess_state::clear() {
    for(int player = 0; player < player_enum_size; player++) {
        for(int piece = 0; piece < board_enum_size; piece++) {
            this->boards[player][piece] = 0x0;
        }
    }
}

void Chess_state::construct_from(cpp_client::chess::AI *ai) {
    this->clear();
    for(auto& piece: ai->game->pieces) {
        Player_enum owner;
        Board_enum type;

        owner = piece->owner->color == "White" ? white_idx : black_idx;

        if(piece->type == "King") type = king_idx;
        if(piece->type == "Queen") type = queen_idx;
        if(piece->type == "Knight") type = knight_idx;
        if(piece->type == "Rook") type = rook_idx;
        if(piece->type == "Bishop") type = bishop_idx;
        if(piece->type == "Pawn") type = pawn_idx;

        int file_offset = piece->file[0] - 'a';
        int rank_offset = piece->rank - 1;
        int total_offset = file_offset + (8 * rank_offset);

        this->boards[owner][type] |= board::Piece_board(0x1) << total_offset;
    }

}

}