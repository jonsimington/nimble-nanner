#ifndef RUS_ENUMS_HPP
#define RUS_ENUMS_HPP

#include <cstdint>

namespace rus {

    // Position
    using Position_type = uint8_t;
    enum class Position : Position_type {
        a1, b1, c1, d1, e1, f1, g1, h1,
        a2, b2, c2, d2, e2, f2, g2, h2,
        a3, b3, c3, d3, e3, f3, g3, h3,
        a4, b4, c4, d4, e4, f4, g4, h4,
        a5, b5, c5, d5, e5, f5, g5, h5,
        a6, b6, c6, d6, e6, f6, g6, h6,
        a7, b7, c7, d7, e7, f7, g7, h7,
        a8, b8, c8, d8, e8, f8, g8, h8,

        end,
        start = 0,
    };
    const std::size_t Position_size = static_cast<std::size_t>(Position::end);
    void operator++(Position & p);
    constexpr Position_type to_i(const Position p) { return (Position_type)p; }
    Position pos_from_fr(const char file, const int rank);
    void fr_from_pos(const Position p, char & file, int & rank);

    // Player
    using Player_type = uint8_t;
    enum class Player: Player_type {
        white,
        black,

        end,
        start = 0,
    };
    const std::size_t Player_size = static_cast<std::size_t>(Player::end);
    void operator++(Player & p);
    constexpr Player_type to_i(const Player p) { return (Player_type)p; }
    Player operator!(const Player p);

    // Castling
    using Castle_type = uint8_t;
    enum class Castle: Castle_type {
        queen,
        king,

        end,
        start = 0,
    };
    const std::size_t Castle_size = static_cast<std::size_t>(Castle::end);
    void operator++(Castle & c);
    constexpr Castle_type to_i(const Castle c) { return (Castle_type)c; }

    // Piece
    using Piece_type = uint8_t;
    enum class Piece: Piece_type {
        pawn,
        knight,
        bishop,
        rook,
        queen,
        king,

        end,
        start = 0,
    };
    const std::size_t Piece_size = static_cast<std::size_t>(Piece::end);
    void operator++(Piece & p);
    constexpr Piece_type to_i(const Piece p) { return (Piece_type)p; }

    // Rays
    using Ray_type = uint8_t;
    enum class Ray: Ray_type {
        nort,
        nort_east,
        sout_east,
        east,
        sout,
        nort_west,
        sout_west,
        west,

        end,
        start = 0
    };
    const std::size_t Ray_size = static_cast<std::size_t>(Ray::end);
    void operator++(Ray & r);
    constexpr Ray_type to_i(const Ray r) { return (Ray_type)r; }

}

#endif