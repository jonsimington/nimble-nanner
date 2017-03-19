#ifndef RUS_PIECE_BOARD_HPP
#define RUS_PIECE_BOARD_HPP

#include <cstdint>
#include <string>
#include <functional>
#include <cassert>

#include "position.hpp"

namespace rus {
    namespace board {

        using Board = uint64_t;
        const Board empty = 0x00;
        const Board full = ~empty;

        constexpr Board position(Position::Type pos) {
            assert(pos >= 0);
            assert(pos < Position::size);
            return Board(0x1) << pos;
        }

        constexpr Board rank(uint8_t r) {
            assert(r <= 8);
            return Board(0xff) << ((r-1) * 8);
        }

        constexpr Board file(char f) {
            assert(f >= 'a');
            assert(f <= 'h');
            return Board(0x101010101010101) << (f-'a');
        }

        // step north, east, south, west
        constexpr Board step_n(const Board bb) { return bb << 8; }
        constexpr Board step_e(const Board bb) { return (bb << 1) & ~file('a'); }
        constexpr Board step_s(const Board bb) { return bb >> 8; }
        constexpr Board step_w(const Board bb) { return (bb >> 1) & ~file('h'); }

        // step north-east, south-east, north-west, south-west
        constexpr Board step_ne(const Board bb) { return (bb << 9) & ~file('a'); }
        constexpr Board step_se(const Board bb) { return (bb >> 7) & ~file('a'); }
        constexpr Board step_nw(const Board bb) { return (bb << 7) & ~file('h'); }
        constexpr Board step_sw(const Board bb) { return (bb >> 9) & ~file('h'); }

        void rankFileFromIdx(int idx, int & rank, std::string & file);

        void for_each_bit(Board bb, std::function<void(Position::Type)> callback);

    }
}

#endif