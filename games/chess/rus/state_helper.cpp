#include "state_helper.hpp"

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

}
