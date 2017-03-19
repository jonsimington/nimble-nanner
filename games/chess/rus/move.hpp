#ifndef RUS_MOVE_HPP
#define RUS_MOVE_HPP

#include <cstdint>

namespace rus {

    // F = flags
    // f = from
    // t = to
    // FFFF ffff fftt tttt
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

        Move(uint16_t  from, uint16_t  to, uint16_t  flags) {
            mov = ((flags & 0xf)<<12) | ((from & 0x3f)<<6) | (to & 0x3f);
        }
        void operator=(Move a) {mov = a.mov;}

        uint16_t to()   const { return mov & 0x3f; }
        uint16_t from() const { return (mov >> 6) & 0x3f; }
        Flag flag()     const { return static_cast<Flag>((mov >> 12) & 0x0f); }

        void to(uint16_t to)        { mov &= ~0x3f; mov |= to & 0x3f; }
        void from(uint16_t from)    { mov &= ~0xfc0; mov |= (from & 0x3f) << 6; }
        void flag(Flag flag)        { mov &= ~0xf000; mov |= (static_cast<uint16_t>(flag) & 0x0f) << 12; }

        unsigned int butterfly_idx() const { return mov & 0x0fff; }
        bool operator==(Move a) const { return (mov & 0xffff) == (a.mov & 0xffff); }
        bool operator!=(Move a) const { return (mov & 0xffff) != (a.mov & 0xffff); }

    protected:
        uint16_t mov; // or short or template type
    };

}

#endif