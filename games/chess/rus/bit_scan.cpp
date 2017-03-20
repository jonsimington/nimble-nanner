#include <cassert>
#include <functional>

#include "bit_scan.hpp"
#include "board.hpp"
#include "enums.hpp"

namespace rus {
    namespace bit_scan {

        // https://chessprogramming.wikispaces.com/BitScan

        int scan_fwd(board::Board bb) {
            assert (bb != 0);
            return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
        }

        int scan_rev(board::Board bb) {
            assert (bb != 0);
            bb |= bb >> 1;
            bb |= bb >> 2;
            bb |= bb >> 4;
            bb |= bb >> 8;
            bb |= bb >> 16;
            bb |= bb >> 32;
            return index64[(bb * debruijn64) >> 58];
        }

        void for_each_bit(board::Board bb, std::function<void(Position)> func) {
            board::Board ms = board::empty;
            if(bb) {
                do {
                    int idx = bit_scan::scan_fwd(bb);
                    func(static_cast<Position>(idx));
                } while(bb &= bb-1);
            }
        }

    }
}