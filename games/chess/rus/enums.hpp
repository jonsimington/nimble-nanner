#ifndef RUS_ENUMS_HPP
#define RUS_ENUMS_HPP

namespace rus {

    enum Player_enum {
        white_idx,
        black_idx,
        player_enum_size,
    };

    enum Board_enum: unsigned char {
        pawn_idx,
        knight_idx,
        bishop_idx,
        rook_idx,
        queen_idx,
        king_idx,

        board_enum_size,
    };



}

#endif