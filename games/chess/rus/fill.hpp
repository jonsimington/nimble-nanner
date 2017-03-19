#ifndef RUS_FILL_HPP
#define RUS_FILL_HPP

#include "board.hpp"

namespace rus {
namespace fill {

// https://chessprogramming.wikispaces.com/Kogge-Stone+Algorithm#Fillonanemptyboard

board::Board nortFill(board::Board gen);
board::Board soutFill(board::Board gen);
board::Board eastFill(board::Board gen);
board::Board westFill(board::Board gen);

board::Board noEaFill(board::Board gen);
board::Board soEaFill(board::Board gen);
board::Board soWeFill(board::Board gen);
board::Board noWeFill(board::Board gen);

}
}

#endif