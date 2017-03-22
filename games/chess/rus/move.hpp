#ifndef RUS_MOVE_HPP
#define RUS_MOVE_HPP

#include <cstdint>
#include <iostream>
#include <type_traits>

#include "enums.hpp"

namespace rus {

    class Move {
    public:

        enum class Flag: uint8_t {
            quiet,
            dbl_pawn,
            king_castle,
            queen_castle,
            capture,
            ep_capture,
            knight_promo,
            bishop_promo,
            rook_promo,
            queen_promo,
        };

        Move(const Position to = Position::a1, const Position from = Position::a1, const Flag flag = Flag::quiet): to(to), from(from), flag(flag) {}
        //TODO: Add default constructor to take from, to, and flag



        Position to;
        Position from;
        Flag flag;


        friend bool operator==(const Move& lhs, const Move& rhs) {
            return lhs.to == rhs.to && lhs.from == rhs.from && lhs.flag == rhs.flag;
        }

        friend std::ostream & operator<<(std::ostream& stream, const Move & obj) {
            char from_file, to_file;
            int from_rank, to_rank;

            rus::fr_from_pos(obj.from, from_file, from_rank);
            rus::fr_from_pos(obj.to, to_file, to_rank);

            stream << from_file << from_rank <<  "->" << to_file << to_rank;
            return stream;
        }
    };

}

#endif