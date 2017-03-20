#include <cassert>

#include "enums.hpp"

namespace rus {

    // Position
    void operator++(Position & p) {
        p = static_cast<Position>(
                static_cast<uint8_t>(p)+1
        );
    }

    Position pos_from_rf(const int rank, const char file) {
        assert(rank >= 1);
        assert(rank <= 8);
        assert(file >= 'a');
        assert(file <= 'h');

        return static_cast<Position>(
                (file-'a') + (8*(rank-1))
        );
    }

    // Player
    void operator++(Player & p) {
        p = static_cast<Player>(
                static_cast<Player_type >(p)+1
        );
    }

    Player operator!(const Player p) {
        return static_cast<Player>(
                static_cast<Player_type>(p) ^ 0x1
        );
    }

    // Castle
    void operator++(Castle & c) {
        c = static_cast<Castle>(
                static_cast<Castle_type>(c)+1
        );
    }

    // Piece
    void operator++(Piece & p) {
        p = static_cast<Piece>(
                static_cast<Piece_type>(p)+1
        );
    }

    // Ray
    void operator++(Ray & r) {
        r = static_cast<Ray>(
                static_cast<Ray_type >(r)+1
        );
    }

}