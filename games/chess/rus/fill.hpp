#ifndef RUS_FILL_HPP
#define RUS_FILL_HPP

#include "piece_board.hpp"

namespace rus {
namespace fill {

// https://chessprogramming.wikispaces.com/Kogge-Stone+Algorithm#Fillonanemptyboard

board::Piece_board nortFill(board::Piece_board gen);
board::Piece_board soutFill(board::Piece_board gen);
board::Piece_board eastFill(board::Piece_board gen);
board::Piece_board westFill(board::Piece_board gen);

board::Piece_board noEaFill(board::Piece_board gen);
board::Piece_board soEaFill(board::Piece_board gen);
board::Piece_board soWeFill(board::Piece_board gen);
board::Piece_board noWeFill(board::Piece_board gen);

}
}

#endif