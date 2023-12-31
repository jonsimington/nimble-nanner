#include "fill.hpp"

namespace rus {
namespace fill {

board::Board soutFill(board::Board gen) {
    gen |= (gen >>  8);
    gen |= (gen >> 16);
    gen |= (gen >> 32);
    return gen;
}

board::Board nortFill(board::Board gen) {
    gen |= (gen <<  8);
    gen |= (gen << 16);
    gen |= (gen << 32);
    return gen;
}

board::Board eastFill(board::Board gen) {
    const board::Board pr0 = ~board::file('a');
    const board::Board pr1 = pr0 & (pr0 << 1);
    const board::Board pr2 = pr1 & (pr1 << 2);
    gen |= pr0 & (gen << 1);
    gen |= pr1 & (gen << 2);
    gen |= pr2 & (gen << 4);
    return gen;
}

board::Board noEaFill(board::Board gen) {
    const board::Board pr0 = ~board::file('a');
    const board::Board pr1 = pr0 & (pr0 << 9);
    const board::Board pr2 = pr1 & (pr1 << 18);
    gen |= pr0 & (gen << 9);
    gen |= pr1 & (gen << 18);
    gen |= pr2 & (gen << 36);
    return gen;
}

board::Board soEaFill(board::Board gen) {
    const board::Board pr0 = ~board::file('a');
    const board::Board pr1 = pr0 & (pr0 >> 7);
    const board::Board pr2 = pr1 & (pr1 >> 14);
    gen |= pr0 & (gen >> 7);
    gen |= pr1 & (gen >> 14);
    gen |= pr2 & (gen >> 28);
    return gen;
}

board::Board westFill(board::Board gen) {
    const board::Board pr0 = ~board::file('h');
    const board::Board pr1 = pr0 & (pr0 >> 1);
    const board::Board pr2 = pr1 & (pr1 >> 2);
    gen |= pr0 & (gen >> 1);
    gen |= pr1 & (gen >> 2);
    gen |= pr2 & (gen >> 4);
    return gen;
}

board::Board soWeFill(board::Board gen) {
    const board::Board pr0 = ~board::file('h');
    const board::Board pr1 = pr0 & (pr0 >> 9);
    const board::Board pr2 = pr1 & (pr1 >> 18);
    gen |= pr0 & (gen >> 9);
    gen |= pr1 & (gen >> 18);
    gen |= pr2 & (gen >> 36);
    return gen;
}

board::Board noWeFill(board::Board gen) {
    const board::Board pr0 = ~board::file('h');
    const board::Board pr1 = pr0 & (pr0 << 7);
    const board::Board pr2 = pr1 & (pr1 << 14);
    gen |= pr0 & (gen << 7);
    gen |= pr1 & (gen << 14);
    gen |= pr2 & (gen << 28);
    return gen;
}

}
}

#ifdef RUS_FILL_TEST

#include <cassert>
#include <iostream>

int main(int argc, char **argv) {

    // verify nortFill
    assert(rus::fill::nortFill(0x49) == 0x4949494949494949);
    assert(rus::fill::nortFill(0x4000000800020000) == 0x4a0a0a0a02020000);

    // verify soutFill
    assert(rus::fill::soutFill(0xa00005000000000) == 0xa0a0a5a5a5a5a5a);

    // verify eastFill

    // verify westFill

    // verify noEaFill
    assert(rus::fill::noEaFill(0x208000020) == 0x9048241208804020);

    // verify soEaFill
    assert(rus::fill::soEaFill(0x1000000440000001) == 0x1020408448902041);

    // verify soWeFill
    assert(rus::fill::soWeFill(0x1000000440000001) == 0x1008040643211009);

    // verify noWeFill
    assert(rus::fill::noWeFill(0x1040000000020080) == 0x3142040811224080);


    return 0;
}

#endif