#include "state_helper.hpp"

void State_helper::preprocess() {
    this->occupied = 0;
    for(int player = 0; player < player_enum_size; p++) {
        this->any_piece[player] = 0;

        for(int piece = 0; piece < board_enum_size; piece++) {
            this->any_piece[player] |= this->state[player][piece];
        }

        this->occupied |= this->any_piece[player];
    }

    empty = ~ occupied;
}