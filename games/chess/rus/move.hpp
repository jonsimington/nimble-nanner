#ifndef RUS_MOVE_HPP
#define RUS_MOVE_HPP

#include "enums.hpp"

namespace rus {

class Move {
public:
    Player_enum player;
    Board_enum piece;
    int from;
    int to;
};

}

#endif